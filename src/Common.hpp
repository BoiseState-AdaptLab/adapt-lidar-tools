#ifndef ADAPTLIDARTOOLS_COMMON_HPP
#define ADAPTLIDARTOOLS_COMMON_HPP

#include <future>
#include <functional>
#include <thread>
#include <vector>

#include "spdlog/spdlog.h"

#include "Peak.hpp"
#include "TaskThread.hpp"

namespace Common{

constexpr double cToFWHM = 2.354820045030949382023138652919399275494771378771641077045; //Multiplying c by this yields the FWHM

struct Options{
    int noiseLevel=9;
    bool nlsFitting = true;
    int numThreads = 1; //@@TODO opinion on signed v unsigned
    int wavesPerThread = 10;
};

/** Fits a varying number of waveforms. Waveforms are identified by their id, which is the first member of the results pair.
 * @param indicies      Index data of each waveform. One vector per waveform. Must have the same length as amplitudes.
 * @param amplitudes    Amplitude data of each waveform. One vector per waveform. Must have the same length as indicies.
 * @param noiseLevel    A peak must be at least this tall to be identified.
 * @param nlsFitting    Use NLS fitting in addition to the second differencing. Slower, but much more accurate.
 * @param results       Output vector to put peaks into
 * @@TODO This probably too abstract for any meaningful error message. It is better for the functions that we call to log the errors themselves, and for the vector to just be empty for that waveform.
 */
void fitWaveforms(const std::vector<std::vector<int>>& indices, std::vector<std::vector<int>>& amplitudes, int noiseLevel, bool nlsFitting, std::vector<std::vector<Peak>>& results);

/** Fits all pulses produced by producer, and feeds them into consumer. 
 * @param producer      The PulseProducer. See top of file for more info on type requirements.
 * @param consumer      The PeakConsumer. See top of file for more info on type requirements.
 * @param options       Options for the process, such as noise level, etc.
 */
template<typename PulseProducer, typename PeakConsumer>
void processData(PulseProducer& producer, PeakConsumer& consumer, const Options& options){
    //Thread pool to run our tasks in
    std::vector<TaskThread<void>> threadPool(options.numThreads);

    //This is simply the function signature of fitWaveforms
    std::vector<std::vector<std::future<void>>> futures;

    //Thead    Waves       Data
    std::vector<std::vector<std::vector<int>>> indices(options.numThreads, std::vector<std::vector<int>>(options.wavesPerThread));
    std::vector<std::vector<std::vector<int>>> amplitudes(options.numThreads, std::vector<std::vector<int>>(options.wavesPerThread));

    std::vector<std::vector<std::vector<Peak>>> peaks(options.numThreads);


    while(!producer.done()){
        futures.clear();
        for(int i = 0; i < options.numThreads; ++i){
            for(int j = 0; j < options.wavesPerThread; ++j){
                producer.producePulse(indices[i][j], amplitudes[i][j]);
            }

            std::packaged_task<void()> task(std::bind(fitWaveforms, indices[i], amplitudes[i], options.noiseLevel, options.nlsFitting, peaks[i]));
            futures.emplace_back(task.get_future());
            threadPool[i].queueTask(std::move(task));
            //@@TODO is may be desireable to calculate stats in a threaded fashion as well
        }

        for(int i = 0; i < options.numThreads; ++i){
            for(int j = 0; j < options.wavesPerThread; ++j){
                futures[i][j].wait();
                consumer.consumePeak(peaks[i][j], options);
            }
        }
    }
}


//Single threaded version
template<typename PulseProducer, typename PeakConsumer>
void processData_Single(PulseProducer& producer, PeakConsumer& consumer, const Options& options){
    std::vector<int> indexData;
    std::vector<std::vector<int>> amplitudeData(1); //Extra std::vector is since we pass them by non-const ref
    std::vector<std::vector<Peak>> peaks(1);

    while(!producer.done()){
        producer.producePulse(indexData, amplitudeData.front(), options.noiseLevel, options.nlsFitting, peaks.front());
        fitWaveforms({indexData}, amplitudeData, options.noiseLevel, options.nlsFitting, peaks);
        consumer.consumePeak(peaks.front(), options);
    }
}


}   // namespace Common

#endif  // ADAPTLIDARTOOLS_COMMON_HPP
