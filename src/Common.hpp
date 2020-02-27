#ifndef ADAPTLIDARTOOLS_COMMON_HPP
#define ADAPTLIDARTOOLS_COMMON_HPP

#include <functional>
#include <future>
#include <vector>

#include "spdlog/spdlog.h"

#include "Peak.hpp"
#include "PulseData.hpp"
#include "TaskThread.hpp"

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
    int noiseLevel=9;
    bool nlsFitting = true;
    int numThreads = 4; //@@TODO opinion on signed v unsigned
    int wavesPerThread = 10;
};

//@@TODO docs
void fitWaveform(const std::vector<int>& indexData, const std::vector<int>& amplitudeData, int noiseLevel, bool useNLSFitting, std::vector<Peak>& results);

/* Fits a single waveform to a sum of Gaussians.
 * @param indexData     The index data of the wave
 * @param amplitudeData The amplitude data of the wave, will be smoothed
 * @param noiseLevel    Cutoff value for peaks. Any peaks with amplitudes lower than this will not be processed.
 * @param useNLSFitting Use Nonlinear-least-squares fitting in addition to second differencing. Recommended.
 * @param results       Output vector for the fitted peaks
 */
template <typename AnalyzeFunc>
void processWaveform(PulseData& data, int noiseLevel, bool useNLSFitting, std::vector<Peak>& results, AnalyzeFunc& analyzeFunc){
    //Fitter::smoothAmplitude(amplitudeData);   //@@TODO write our own, current one is weird
    fitWaveform(data.returningIndex, data.returningAmplitude, noiseLevel, useNLSFitting, results);
    analyzeFunc(results, data);
}

/** Fits all pulses produced by producer, and feeds them into consumer.
 * @param producer      The PulseProducer. See top of file for more info on type requirements.
 * @param consumer      The PeakConsumer. See top of file for more info on type requirements.
 * @param analyzeFunc   The AnalyzeFunc function. See top of file for requirements.
 * @param options       Options for the process, such as noise level, etc.
 */
template<typename PulseProducer, typename PeakConsumer, typename AnalyzeFunc>
void processData(PulseProducer& producer, PeakConsumer& consumer, AnalyzeFunc& analyzeFunc, const Options& options){
    spdlog::trace("Starting data processing\n");
    //Thread pool to run our tasks in
    std::vector<TaskThread<void>> threadPool(options.numThreads);

    std::vector<std::future<void>> futures;

    std::vector<std::vector<Peak>> peaks(options.numThreads*options.wavesPerThread);

    std::vector<PulseData> pulses(options.numThreads*options.wavesPerThread);

    while(!producer.done()){
        futures.clear();
        int numPulses = 0; //This keeps track of how many pulses we actually receive, in case producer.done() == true halfway through
        for(int i = 0; i < options.numThreads && !producer.done(); ++i){
            for(int j = 0; j < options.wavesPerThread && !producer.done(); ++j, ++numPulses){
                producer.producePulse(pulses[numPulses]); //Fill with pulse data
                if(pulses[numPulses].returningIndex.empty()){
                    numPulses--;
                    continue;
                }

                //Create task to process waveform
                //We have to pass the postProcess function like this because it requires a reference to producer
                std::packaged_task<void()> task
                    (std::bind(processWaveform<AnalyzeFunc>, std::ref(pulses[numPulses]), options.noiseLevel, options.nlsFitting, std::ref(peaks[numPulses]), std::ref(analyzeFunc)));

                //Get a future (allows us to wait for it to be done later)
                futures.emplace_back(task.get_future());

                //@@TODO is may be desireable to calculate stats in a threaded fashion as well
                //Move it into one of the taskthreads to start processing it
                threadPool[i].queueTask(std::move(task));
            }
        }

        for(int i = 0, index=0; i < options.numThreads && numPulses > 0; ++i){
            for(int j = 0; j < options.wavesPerThread && numPulses > 0; ++j, --numPulses, ++index){
                futures[index].wait();   //Wait for taskthread to process it.
                consumer.consumePeaks(peaks[index], pulses[index]); //@@TODO if the task threw an exception, peaks[i][j] will probably be whatever was in there last time.
            }
        }
    }

    consumer.postProcess();
}

//Single threaded version
template<typename PulseProducer, typename PeakConsumer, typename AnalyzeFunc>
void processData_Single(PulseProducer& producer, PeakConsumer& consumer, AnalyzeFunc& analyzeFunc, const Options& options){
    PulseData data;
    std::vector<Peak> peaks;

    while(!producer.done()){
        producer.producePulse(data);

        if(data.returningIndex.empty()) continue;

        processWaveform<AnalyzeFunc>(data, options.noiseLevel, options.nlsFitting, peaks, analyzeFunc);
        consumer.consumePeaks(peaks, data);
    }
    consumer.postProcess();
}

}   // namespace Common

#endif  // ADAPTLIDARTOOLS_COMMON_HPP
