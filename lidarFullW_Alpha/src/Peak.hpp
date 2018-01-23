/*
 * File name: Peak.hpp
 * Created on: 15-December-2017
 * Author: olschanowsky
 */

#ifndef PEAK_HPP_
#define PEAK_HPP_

double amp;         //Ampitude
double location;    //The time location of peak occurence
double fwhm;        //Full width half maximum - the width of the peak

int peak_position;  //Given 'n' peaks, the position of this peak

int peak_triggering_location; //Peak activation time - one ahead of the noise

//(peak_triggering_location) * dx + x_first
int peak_x_activation;

//(peak_triggering_location) * dy + y_first
int peak_y_activation;

//(peak_triggering_location) * dz + z_first
int peak_z_activation;

#endif /* PEAK_HPP_ */
