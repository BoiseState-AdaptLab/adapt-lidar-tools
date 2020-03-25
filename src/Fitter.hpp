#ifndef ADAPTLIDAR_FITTER_HPP
#define ADAPTLIDAR_FITTER_HPP
#include <iostream>
#include <vector>

// Provides a method to fit a set of initial guesses to a curve. Curve should be compose of a sum of Gaussians.
namespace Fitter{

    /**
     * Compact representation of a Gaussian. Used because 95% of a Peak is irrelevant to the fitting
     */
    struct Gaussian{
        Gaussian() = default;
        Gaussian(double a, double b, double c) : a(a), b(b), c(c) {};   //Allow for {1,2,3} construction
        double a=0;
        double b=0;
        double c=0;
    };

    /**
     * Given reasonably accurate guesses, fits them to a curve denoted by {indexData_i, amplitudeData_i}.
     * The equation is a sum of Gaussians, fitted using GSL's NLS fitter.
     *
     * It is better for the parameters of a guess to be too small than too large.
     *
     * @param indexData     The indices of the amplitude data. Must be the same length as amplitudeData.
     * @param amplitudeData The amplitude data of the curve to fit. Must be the same length as indexData.
     * @param guesses       A set of starting Gaussians to begin fitting from. The final fitting results will be placed in this vector, overwriting the original guesses
     * @return bool         True if fitter completed without issues. False if there is no waveform data, no peaks, or other error.
     */
    bool fitGaussians(const std::vector<int>& indexData, const std::vector<int>& amplitudeData, std::vector<Gaussian>& guesses);

    /**
     * Guesses gaussians using second central finite differencing.
     *
     * @param indexData     The indices of the amplitude data. Must be the same length as amplitudeData.
     * @param amplitudeData The amplitude data of the wave. Must be the same length as indexData.
     * @param noiseLevel    Only count the peak if it's amplitude is above this number
     * @param guesses       Output vector to put guesses into. Empty if no guesses found
     */
    void guessGaussians(const std::vector<int>& indexData, const std::vector<int>& amplitudeData, int noiseLevel, std::vector<Gaussian>& guesses);

    /**
     * Reduces noise in the amplitudeData. It may be desirable to call this one with the standard noise level, and then again with a very small noise level, to effectively floor the small values.
     * @param amplitudeData Data to reduce noise in
     * @param noiseLevel    Cutoff value, values above this will not be reduced.
     * @param aggression    How aggressively the noise is reduced. Higher values induce a larger change on the data.
     */
    void reduceNoise(std::vector<int>& amplitudeData, int noiseLevel, double aggression=1.);

    /**
     * Smooth the amplitudeData, affecting smaller values exponentially more (relative to the max)
     * @param amplitudeData Data to be smoothed.
     */
    void smoothData(std::vector<int>& amplitudeData);

} // namespace Fitter
#endif  //ADAPTLIDAR_FITTER_HPP
