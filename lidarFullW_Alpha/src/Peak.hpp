//File name: Peak.hpp
//Created on: 15-December-2017
//Author: olschanowsky

#ifndef PEAK_HPP_
#define PEAK_HPP_

class Peak{

  double amp;         //Ampitude
  double location;    //The time location of peak occurence
  double fwhm;        //Full width at half maximum - the width of the peak

  //the time locations of the half maximum(amp/2)
  double fwhm_t_positive;   
  double fwhm_t_negative;

  int position;  //Given 'n' peaks, the position of this peak

  int triggering_amp;      //Peak triggering amplitude - noise level + 1
  int triggering_location; //Peak activation time - calc using triggering_amp

  //(triggering_location) * dx + x_first
  int x_activation;

  //(triggering_location) * dy + y_first
  int y_activation;

  //(peak_triggering_location) * dz + z_first
  int z_activation;

};

#endif /* PEAK_HPP_ */
