#include "spdlog/spdlog.h"

#include "GeoTIFFDriver.hpp"
#include "CmdLine.hpp"
#include "Common.hpp"

#include "PulseWavesProducer.hpp"
#include "GeoTIFFConsumer.hpp"

constexpr double NO_DATA = -99999;

namespace GeoTIFF{

    void peakCalculations(PulseData& pulse, std::vector<Peak>& peaks, int noiseLevel, bool useNLSFitting, bool calcBackscatter, double calibConstant){
    if (calcBackscatter){
        if (pulse.outgoingIndex.empty()){
            return;
        }

        std::vector<Peak> results;
        Common::fitWaveform(pulse.outgoingIndex, pulse.outgoingAmplitude, noiseLevel, useNLSFitting, results);
        
        //For every returning wave peak, calculate the backscatter coefficient
        for(Peak& peak : peaks){
            if (results.empty()){
                peak.backscatter_coefficient = NO_DATA;
            } else {
                peak.calcBackscatter(results[0].amp,
                        results.at(0).fwhm, calibConstant,
                        pulse.gpsInfo.x_anchor, pulse.gpsInfo.y_anchor, pulse.gpsInfo.z_anchor);
            }
            if (peak.backscatter_coefficient == INFINITY){
                peak.backscatter_coefficient = NO_DATA;
            }
        }
    }

    //Check if each peak has a rise time
    for(Peak& peak : peaks){
        peak.rise_time = peak.rise_time < 0 ? NO_DATA : peak.rise_time;
    }
    }

int mainProxy(int argc, char* argv[]){
    //Old code
    spdlog::set_pattern("[%t][%^%=8l%$] %v");

    CmdLine cmdline;
    if(!cmdline.parse_args(argc, argv)){
        spdlog::error("Unable to parse command line options");
        return 1;
    }

    // Set verbosity
    // why
    if (cmdline.verb == "trace") {
        spdlog::set_level(spdlog::level::trace);
    } else if (cmdline.verb == "debug") {
        spdlog::set_level(spdlog::level::debug);
    } else if (cmdline.verb == "info") {
        spdlog::set_level(spdlog::level::info);
    } else if (cmdline.verb == "warn") {
        spdlog::set_level(spdlog::level::warn);
    } else if (cmdline.verb == "error") {
        spdlog::set_level(spdlog::level::err);
    } else if (cmdline.verb == "critical") {
        spdlog::set_level(spdlog::level::critical);
    }

    PulseWavesProducer producer(cmdline.getInputFileName(true));
    GeoTIFFConsumer consumer;
    Common::Options options;

    //@@TODO setup stuff

    auto func = [&producer, &consumer, options](std::vector<Peak>& peaks, PulseData& data){
        producer.postProcess(peaks, data);
        peakCalculations(data, peaks, options.noiseLevel, options.nlsFitting, true, 45.0);  //@TODO
        spdlog::trace("Called");
    };

    options.numThreads=8;
    options.wavesPerThread=20;

    Common::processData(producer, consumer, func, options);

    spdlog::critical("Finished");


    return 0;
}

}   //namespace GeoTIFF
