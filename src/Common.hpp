#ifndef ADAPTLIDARTOOLS_COMMON_HPP
#define ADAPTLIDARTOOLS_COMMON_HPP

#include <functional>
#include <future>
#include <vector>

#include "spdlog/spdlog.h"

#include "Peak.hpp"
#include "PulseData.hpp"
#include "TaskThread.hpp"
#include "GaussianFitter.hpp"

/** ===== TYPE REQUIREMENTS =====
 * An object P of type PulseProducer must:
 *      Be a class/struct
 *      Must have public member functions with these signatures:
 *          bool done() const;
 *              Returns true if there are more waves to process.
 *          void producePulse(PulseData&);
 *              Populates PulseData with the wave information.
 *
 * An object C of type PeakConsumer must:
 *      Be a class/struct
 *      Must have public member functions with these signatures:
 *          void consumePeaks(const std::vector<Peak>&, const PulseData&);
 *              Writes the peak to wherever applicable. Note that after the function returns, there are no guarantees on peaks
 *          void postProcess();
 *              Signifies that all peaks have been fed to the consumer via consumePeaks. The destructor will likely be called next.
 *              Use this to do any final analysis/processing.
 *
 * Note that technically these could be one class, but it more flexible if they remain separate.
 */

namespace Common{

constexpr double cToFWHM = 2.354820045030949382023138652919399275494771378771641077045; //Multiplying c by this yields the FWHM

struct Options{
    int noiseLevel=6;
    int minPeakAmp = 9;
    bool nlsFitting = true;
    bool reduceNoise=true;
    bool smoothData=true;
    bool calcBackscatter=false;
    double calibrationConstant=std::numeric_limits<double>::min();
    int numThreads = 8;
    int wavesPerThread = 50;
};

//@@TODO docs
bool fitWaveform(std::vector<int>& indexData, std::vector<int>& amplitudeData, const Options& options, std::vector<GaussianFitter::Gaussian>& results);

/* Fits a single waveform to a sum of Gaussians.
 * @param indexData     The index data of the wave
 * @param amplitudeData The amplitude data of the wave, will be smoothed
 * @param options       Fitting options
 * @param results       Output vector for the fitted peaks
 * @param postFunc      @@TODO
 */
template <typename PostFunc>
bool processWaveform(PulseData& data, const Options& options, std::vector<Peak>& results, PostFunc& postFunc){
    results.clear();
    std::vector<GaussianFitter::Gaussian> gaussians;
    bool result = fitWaveform(data.returningIndex, data.returningAmplitude, options, gaussians);

    if(!result) return false;

    for(const GaussianFitter::Gaussian& gaussian : gaussians){
        Peak peak;
        peak.amp = gaussian.a;
        peak.location = gaussian.b;
        peak.fwhm = cToFWHM * gaussian.c;
        results.push_back(peak);
    }

    result = postFunc(data, results, options);   //@@TODO: Does it need to know about the noise and whatnot?

    return result;
}

/** Fits all pulses produced by producer, and feeds them into consumer.
 * @param producer      The PulseProducer. See top of file for more info on type requirements.
 * @param consumer      The PeakConsumer. See top of file for more info on type requirements.
 * @param postFunc      The postFunc function. See top of file for requirements.
 * @param options       Options for the process, such as noise level, etc.
 */
template<typename PulseProducer, typename PeakConsumer, typename PostFunc>
void processData(PulseProducer& producer, PeakConsumer& consumer, PostFunc& postFunc, const Options& options){
    //Thread pool to run our tasks in
    std::vector<TaskThread<bool>> threadPool(options.numThreads);

    std::vector<std::future<bool>> futures;

    std::vector<std::vector<Peak>> peaks(options.numThreads*options.wavesPerThread);

    std::vector<PulseData> pulses(options.numThreads*options.wavesPerThread);

    int totalWaves=0;
    int emptyWaves=0;
    int failedWaves=0;
    int totalPeaks=0;

    spdlog::info("[Common] Beginning waveform processing");
    while(!producer.done()){
        futures.clear();
        int numPulses = 0; //This keeps track of how many pulses we actually receive, in case producer.done() == true halfway through
        for(int i = 0; i < options.numThreads && !producer.done(); ++i){
            for(int j = 0; j < options.wavesPerThread && !producer.done(); ++j, ++numPulses){
                totalWaves++;
                producer.producePulse(pulses[numPulses]); //Fill with pulse data
                if(pulses[numPulses].returningIndex.empty()){
                    emptyWaves++;
                    numPulses--;
                    continue;
                }

                //Create task to process waveform
                //We have to pass the postProcess function like this because it requires a reference to producer
                std::packaged_task<bool()> task
                    (std::bind(processWaveform<PostFunc>, std::ref(pulses[numPulses]), std::ref(options), std::ref(peaks[numPulses]), std::ref(postFunc)));
                //Get a future (allows us to wait for it to be done later)
                futures.emplace_back(task.get_future());

                //Move it into one of the taskthreads to start processing it
                threadPool[i].queueTask(std::move(task));
            }
        }

        for(int i = 0, index=0; i < options.numThreads && numPulses > 0; ++i){
            for(int j = 0; j < options.wavesPerThread && numPulses > 0; ++j, --numPulses, ++index){
                bool valid = futures[index].get();   //Wait for taskthread to process it.
                if(!valid){
                    failedWaves++;
                }else{
                    totalPeaks+=peaks[index].size();
                    consumer.consumePeaks(peaks[index], pulses[index]); //@@TODO if the task threw an exception, peaks[i][j] will probably be whatever was in there last time.
                }
            }
        }
    }

    spdlog::info("[Common] Finished processing waveforms. Post-processing peaks");
    consumer.postProcess();

    //Print out stats
    spdlog::info("[Common] Done post-processing peaks. Stats:");
    spdlog::info("[Common] \t{} Total Waves", totalWaves);
    spdlog::info("[Common] \t{} Empty Waves ({}% of total)", emptyWaves, (100.*emptyWaves)/totalWaves);
    spdlog::info("[Common] \t{} Failed waves ({}% of total)", failedWaves, (100.*failedWaves)/totalWaves);
    spdlog::info("[Common] \t{} Total Peaks", totalPeaks);
}

//Single threaded version
template<typename PulseProducer, typename PeakConsumer, typename PostFunc>
void processData_Single(PulseProducer& producer, PeakConsumer& consumer, PostFunc postFunc, const Options& options){
    PulseData data;
    std::vector<Peak> peaks;

    spdlog::info("[Common] Beginning waveform processing");
    while(!producer.done()){
        producer.producePulse(data);

        if(data.returningIndex.empty()) continue;

        processWaveform<PostFunc>(data, options, peaks, postFunc);

        if(!peaks.empty()){
            consumer.consumePeaks(peaks, data);
        }

    }
    spdlog::info("[Common] Finished processing waves. Beginning product generation");
    consumer.postProcess();
    spdlog::info("[Common] Finished product generation");
}

}   // namespace Common

#endif  // ADAPTLIDARTOOLS_COMMON_HPP
