#ifndef ADAPTLIDARTOOLS_COMMON_HPP
#define ADAPTLIDARTOOLS_COMMON_HPP

#include <functional>
#include <future>
#include <vector>

#include "spdlog/spdlog.h"

#include "Peak.hpp"
#include "TaskThread.hpp"

namespace Common{

constexpr double cToFWHM = 2.354820045030949382023138652919399275494771378771641077045; //Multiplying c by this yields the FWHM

struct Options{
    int noiseLevel=9;
    bool nlsFitting = true;
    int numThreads = 4; //@@TODO opinion on signed v unsigned
    int wavesPerThread = 10;
};

/* Fits a single waveform to a sum of Gaussians.
 * @param indexData     The index data of the wave
 * @param amplitudeData The amplitude data of the wave, will be smoothed
 * @param noiseLevel    Cutoff value for peaks. Any peaks with amplitudes lower than this will not be processed.
 * @param useNLSFitting Use Nonlinear-least-squares fitting in addition to second differencing. Recommended.
 * @param results       Output vector for the fitted peaks
 */
void fitWaveform(const std::vector<int>& indexData, std::vector<int>& amplitudeData, int noiseLevel, bool useNLSFitting, std::vector<Peak>& results);

/** Fits all pulses produced by producer, and feeds them into consumer.
 * @param producer      The PulseProducer. See top of file for more info on type requirements.
 * @param consumer      The PeakConsumer. See top of file for more info on type requirements.
 * @param options       Options for the process, such as noise level, etc.
 */
template<typename PulseProducer, typename PeakConsumer>
void processData(PulseProducer& producer, PeakConsumer& consumer, const Options& options){
    //Thread pool to run our tasks in
    std::vector<TaskThread<void>> threadPool(options.numThreads);

    //  |Thread     |Per Thread
    std::vector<std::vector<std::future<void>>> futures;

    //  |Thead      |Waves      |Data
    std::vector<std::vector<std::vector<int>>> indices   (options.numThreads, std::vector<std::vector<int>>(options.wavesPerThread));
    std::vector<std::vector<std::vector<int>>> amplitudes(options.numThreads, std::vector<std::vector<int>>(options.wavesPerThread));
    std::vector<std::vector<std::vector<Peak>>> peaks    (options.numThreads);

    while(!producer.done()){
        int pulses = 0; //This keeps track of how many pulses we actually receive, in case producer.done() == true halfway through
        futures.clear();
        for(int i = 0; i < options.numThreads && !producer.done(); ++i){
            for(int j = 0; j < options.wavesPerThread && !producer.done(); ++j, ++pulses){
                producer.producePulse(indices[i][j], amplitudes[i][j]); //Fill with pulse data

                //Create task to process waveform
                std::packaged_task<void()> task(std::bind(fitWaveform, indices[i][j], amplitudes[i][j], options.noiseLevel, options.nlsFitting, peaks[i][j]));

                //Get a future (allows us to wait for it to be done later)
                futures[i].emplace_back(task.get_future());

                //@@TODO is may be desireable to calculate stats in a threaded fashion as well
                //Move it into one of the taskthreads to start processing it
                threadPool[i].queueTask(std::move(task));
            }
        }

        for(int i = 0; i < options.numThreads && pulses > 0; ++i){
            for(int j = 0; j < options.wavesPerThread && pulses > 0; ++j, --pulses){
                futures[i][j].wait();   //Wait for taskthread to process it.
                consumer.consumePeak(peaks[i][j], options); //@@TODO if the task threw an exception, peaks[i][j] will probably be whatever was in there last time.
            }
        }
    }
}

//Single threaded version
template<typename PulseProducer, typename PeakConsumer>
void processData_Single(PulseProducer& producer, PeakConsumer& consumer, const Options& options){
    std::vector<int> indexData;
    std::vector<int> amplitudeData;
    std::vector<Peak> peaks;

    while(!producer.done()){
        producer.producePulse(indexData, amplitudeData);
        fitWaveform(indexData, amplitudeData, options.noiseLevel, options.nlsFitting, peaks);
        consumer.consumePeak(peaks, options);
    }
}

}   // namespace Common

#endif  // ADAPTLIDARTOOLS_COMMON_HPP
