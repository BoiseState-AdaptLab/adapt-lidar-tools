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

    bool is_final_peak; //Keeps track of whether this is final peak in wave

    //the time locations of the half maximum(amp/2)
    double fwhm_t_positive;
    double fwhm_t_negative;

    int position;  //Given 'n' peaks, the position of this peak

    int triggering_amp;      //Peak triggering amplitude - noise level + 1
    int triggering_location; //Peak activation time - calc using triggering_amp

    //(triggering_location) * dx + x_first
    double x_activation;

    //(triggering_location) * dy + y_first
    double y_activation;

    //(peak_triggering_location) * dz + z_first
    double z_activation;

    //holds peak location in the xyz space
    double x, y, z;

    //Default constructor
    Peak();

};

#endif /* PEAK_HPP_ */
