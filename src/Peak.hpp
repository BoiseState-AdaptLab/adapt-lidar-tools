//File name: Peak.hpp
//Created on: 15-December-2017
//Author: olschanowsky

#ifndef PEAK_HPP_
#define PEAK_HPP_

#include <string>
#include <vector>

class Peak{

    public:
        double amp;         //Amplitude
        double location;    //The time location of peak occurence
        double fwhm;        //Full width at half maximum - the width of the peak

        //the time locations of the half maximum(amp/2)
        double fwhm_t_positive;
        double fwhm_t_negative;

        //Keeps track of whether this is final peak in wave
        bool is_final_peak;

        //Given 'n'peaks, the position of this peak
        int position_in_wave;


        int triggering_idx;

        //Peak triggering amplitude - noise level + 1
        int triggering_amp;

        //Peak activation time - calc using triggering_amp
        int triggering_location;

        //(triggering_location) * dx + x_first
        double x_activation;

        //(triggering_location) * dy + y_first
        double y_activation;

        //(peak_triggering_location) * dz + z_first
        // Elevation
        double z_activation;

        //holds peak location in the xyz space
        double x, y, z;

        //Stores rise time of the peak
        double rise_time;

        //Stores the backscatter coefficient at that peak
        double backscatter_coefficient;

        //Default constructor
        Peak();

        //Calculates backscatter coefficient
        void calcBackscatter(double emitted_amp, double emitted_fwhm,
                              double calibration_constant, double x_anchor,
                              double y_anchor, double z_anchor);

        //Creates list of variables specified by varlist in string form
        void toString(std::string& str, std::vector<int> varlist);
};

#endif /* PEAK_HPP_ */
