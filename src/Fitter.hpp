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
     * @@TODO [verify] It is better for the parameters of a guess to be too small than too large.
     *
     * @param indexData     The indices of the amplitude data. Must be the same length as amplitudeData.
     * @param amplitudeData The amplitude data of the curve to fit. Must be the same length as indexData.
     * @param guesses       A set of starting Gaussians to begin fitting from. The final fitting results will be placed in this vector, overwriting the original guesses
     * @return bool         True if fitter completed without issues. False if there is no waveform data, no peaks, or other error.
     */
    bool fitGaussians(const std::vector<double>& indexData, const std::vector<double>& amplitudeData, std::vector<Gaussian>& guesses);
} // namespace Fitter
#endif  //ADAPTLIDAR_FITTER_HPP
