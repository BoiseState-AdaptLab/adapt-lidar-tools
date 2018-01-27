//File name: Peak.hpp
//Created on: 26-January-2018
//Author: ravi

#include "Peak.hpp"

//Default constructor
Peak::Peak(){
  amp = 0;        
  location = 0;   
  fwhm = 0;       

 
  fwhm_t_positive = 0;   
  fwhm_t_negative = 0;

  position = 0; 

  triggering_amp = 0;     
  triggering_location = 0;

 
  x_activation = 0;

 
  y_activation = 0;

 
  z_activation = 0;

}