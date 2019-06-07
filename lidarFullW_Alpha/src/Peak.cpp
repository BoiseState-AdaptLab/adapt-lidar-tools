//File name: Peak.cpp
//Created on: 26-January-2018
//Author: ravi

#include "math.h"
#include "Peak.hpp"
#include <iostream>

//Default constructor
Peak::Peak(){
    amp = 0;
    location = 0;
    fwhm = 0;

    fwhm_t_positive = 0;
    fwhm_t_negative = 0;

    is_final_peak = false;
    position_in_wave = 0;

    triggering_amp = 0;
    triggering_location = 0;

    x_activation = 0;
    y_activation = 0;
    z_activation = 0;

    x = 0.;
    y = 0.;
    z = 0.;

    backscatter_coefficient = -1;
}

/*
 * Provides a calculation of the backscatter coefficient
 * @param emitted_amp Amplitude of the emitted pulse
 * @param emitted_fwhm Fwhm of the emitted pulse
 * @param calibration_constant Calibration constant to be used
 * @param x,y,z_anchor The x,y,z location of the lidar module
 * @returns the backscatter coefficient at this peak,
 * returns -1 if no emited wave peak was found
 */
void Peak::calcBackscatter(double emitted_amp, double emitted_fwhm,
                            double calibration_constant, double x_anchor,
                            double y_anchor, double z_anchor){
    //Variables:
    //Pulse Width of outgoing wave (W_o)
    //Amplitude of outgoing wave (A_o)
    //Pulse Width of returning wave (W_r)
    //Amplitude of returning wave (A_r)
    //Range (R)
    //Standard Deviation of pulse width (s)
    //Atmospheric Transmission Factor (n_atm)
    //Atmoshperic Attenuation Coefficient (a)
    //Calibration Coefficient (C)


    //R = distance from the lidar module's position to the
    //activation point
    double range = sqrt(pow(x_anchor - x_activation, 2) + 
        pow(y_anchor - y_activation, 2) + pow(z_anchor - z_activation, 2));
    
    //s = sqrt(W_o^2 + W_r^2)
    double standard_dev = sqrt(pow(fwhm, 2) + pow(emitted_fwhm, 2));

    //n_atm = 10^(-2*R*a/10,000)
    //a = -.073*log(R) + .7226
    double a = -.073*log(range) + .7226;
    double n_atm = pow(10, -2*range*a/10000);
     
    //Backscatter Coefficient = C * (R^2*A_r*s) / (A_o*n_atm)
    backscatter_coefficient =  calibration_constant * 
        (pow(range, 2)*amp*standard_dev) / (emitted_amp*n_atm);

#ifdef debug    
       std::cout << "Outgoing Amplitude = " << emitted_amp
            << std::endl;
        std::cout << "Returning Amplitude = " << amp << std::endl;
        std::cout << "Range = " << range << std::endl;
        std::cout << "Standard Deviation = " << standard_dev << std::endl;
        std::cout << "Atmoshperic Transmission Factor = " << n_atm << std::endl;
        std::cout << "Backscatter Coefficient = " << backscatter_coefficient << std::endl;
#endif
}

