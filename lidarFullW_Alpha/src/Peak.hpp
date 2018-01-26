//File name: Peak.hpp
//Created on: 15-December-2017
//Author: olschanowsky

#ifndef PEAK_HPP_
#define PEAK_HPP_

double amp;         //Ampitude
double location;    //The time location of peak occurence
double fwhm;        //Full width half maximum - the width of the peak

int position;  //Given 'n' peaks, the position of this peak

int triggering_location; //Peak activation time - one ahead of the noise

//(triggering_location) * dx + x_first
int x_activation;

//(triggering_location) * dy + y_first
int y_activation;

//(peak_triggering_location) * dz + z_first
int z_activation;

#endif /* PEAK_HPP_ */
