//File name: Peak.hpp
//Created on: 15-December-2017
//Author: olschanowsky

#ifndef PEAK_HPP_
#define PEAK_HPP_

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

        //Given 'n' peaks, the position of this peak
        int position_in_wave;

        //Peak triggering amplitude - noise level + 1
        int triggering_amp;

        //Peak activation time - calc using triggering_amp
        int triggering_location;

        //(triggering_location) * dx + x_first
        double x_activation;

        //(triggering_location) * dy + y_first
        double y_activation;

        //(peak_triggering_location) * dz + z_first
        double z_activation;

        //holds location of the lidar module
        //only used in emitted wave peaks
        double x_anchor, y_anchor, z_anchor;

        //holds peak location in the xyz space
        double x, y, z;

        //Stores the calibration coonstant for the backscatter coefficient
        //only used in emitted wave peaks
        double calibration_constant;

        //Stores the backscatter coefficient at that peak
        double backscatter_coefficient;

        //Points to the emmitted pulse's peak information
        //Only used in returning wave peaks
        //Used to calculate the backscatter coefficient
        Peak* emitted_wave;

        //Default constructor
        Peak();
      
        //Calculates backscatter coefficient
        double calcBackscatter();
};

#endif /* PEAK_HPP_ */
