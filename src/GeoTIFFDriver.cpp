#include "spdlog/spdlog.h"

//@@TODO sort
#include "GeoTIFFDriver.hpp"
#include "CmdLine.hpp"
#include "Common.hpp"

#include "PulseWavesProducer.hpp"
#include "GeoTIFFConsumer.hpp"
#include "GaussianFitter.hpp"
#include "FlightLineData.hpp"
#include "Peak.hpp"

constexpr double NO_DATA = -99999;

namespace GeoTIFF{

bool peakCalculations(PulseData& pulse, std::vector<Peak>& peaks, const Common::Options& options){
    int idx=1;
    for(Peak& peak : peaks){
        //Taken from GaussianFitter +567
        constexpr double neg4ln2 = -2.77258872223978123766892848583270627230200053744102101648;
        double c = peak.fwhm/Common::cToFWHM;
        peak.triggering_location = std::ceil(peak.location -
                std::sqrt((std::log(options.noiseLevel/peak.amp)*c*c)/neg4ln2));
        //@@TODO: There was a comment here previously: "TODO: fix to use correct function"
        peak.triggering_amp = peak.amp * std::exp(neg4ln2 * std::pow(peak.triggering_location-peak.location, 2) * (1/(c*c)));
        peak.rise_time = peak.location - peak.triggering_location;

        //Activation stuff, from FlightLineData::calc_xyz_activation
        //@@TODO the old version removed peaks that had a large triggering amp, and logged invalid activations
        peak.x_activation = peak.triggering_location * pulse.gpsInfo.dx + pulse.gpsInfo.x_first;
        peak.y_activation = peak.triggering_location * pulse.gpsInfo.dy + pulse.gpsInfo.y_first;
        peak.z_activation = peak.triggering_location * pulse.gpsInfo.dz + pulse.gpsInfo.z_first;
        peak.position_in_wave = idx++;
    }

    if(!peaks.empty()){
        peaks.back().is_final_peak=true;
    }

    //Backscatter stuff, from LidarDriver::peak_calculations. Depends on activations
    if (options.calcBackscatter && !pulse.outgoingIndex.empty()){
        std::vector<GaussianFitter::Gaussian> gaussians;
        Common::fitWaveform(pulse.outgoingIndex, pulse.outgoingAmplitude, options, gaussians);

        //For every returning wave peak, calculate the backscatter coefficient
        for(Peak& peak : peaks){
            if (gaussians.empty()){
                peak.backscatter_coefficient = NO_DATA;
            } else {
                peak.calcBackscatter(gaussians.front().a,
                        gaussians.front().c * Common::cToFWHM, options.calibrationConstant,
                        pulse.gpsInfo.x_anchor, pulse.gpsInfo.y_anchor, pulse.gpsInfo.z_anchor);
            }
            if (peak.backscatter_coefficient == INFINITY){  //@@TODO is this special, or should we be using some of the proper functions like std::isfinite?
                peak.backscatter_coefficient = NO_DATA;
            }
        }
    }

    //Check if each peak has a rise time
    for(Peak& peak : peaks){
        peak.rise_time = peak.rise_time < 0 ? NO_DATA : peak.rise_time;
    }

    return true;
}

int mainProxy(int argc, char* argv[]){
    spdlog::set_pattern("[%t][%^%=8l%$] %v");   //@@TODO

    CmdLine cmdline;
    if(!cmdline.parseArguments(argc, argv)){
        spdlog::error("Unable to parse command line options, or help requested");
        return 1;
    }


    std::vector<PeakProducts::Product> products = cmdline.products;
    Common::Options options = cmdline.fitterOptions;

    PulseWavesProducer producer(cmdline.getPLSFilename());

    const FlightLineData& dataRef = producer.getFlightLineData();


    std::string fileName = cmdline.getTrimmedFileName();
    std::string::size_type lastSlash = fileName.find_last_of("/\\");
    if(lastSlash != std::string::npos){ //@@TODO: This probably belongs somewhere else
        fileName = fileName.substr(lastSlash+1);
    }

    GeoTIFFConsumer consumer(dataRef.bb_x_min, dataRef.bb_x_max, dataRef.bb_y_min, dataRef.bb_y_max, fileName, dataRef.geog_cs, dataRef.utm, products);

    Common::processData(producer, consumer, peakCalculations, options);

    spdlog::info("Finished");


    return 0;
}

}   //namespace GeoTIFF
