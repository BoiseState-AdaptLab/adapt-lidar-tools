#ifndef ADAPTLIDAR_PREPROCESS_HPP
#define ADAPTLIDAR_PREPROCESS_HPP
#include <vector>

//Functions to preprocess the waveform to make the fitting better.
namespace PreProcess{

    /**
     * Reduces noise in the amplitudeData based off of the noise level
     * @param amplitudeData Data to reduce noise in
     * @param noiseLevel    Cutoff value, values above this will not be reduced.
     * @param aggression    How aggressively the noise is reduced. Higher values induce a larger change on the data.
     */
    void reduceNoise(std::vector<int>& amplitudeData, int noiseLevel, double aggression=1.);

    /**
     * Performs a three-point moving average on the data. Affects smaller values exponentially more, relative to the maximum value
     * @param indexData     Index data
     * @param amplitudeData Amplitude data
     */
    void smoothData(const std::vector<int>& indexData, std::vector<int>& amplitudeData);

} // namespace PreProcess
#endif // ADAPTLIDAR_PREPROCESS_HPP
