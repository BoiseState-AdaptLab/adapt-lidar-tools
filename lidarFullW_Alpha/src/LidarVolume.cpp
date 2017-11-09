/*
 * File name: LidarVolume.cpp
 * Created on: 08-November-2017
 * Author: ravi
 */

#include "LidarVolume.hpp"
#include <math.h>


//Default constructor
LidarVolume::LidarVolume(){
  //initialize to zero
  pulse_bb_x_min = 0;
  pulse_bb_y_min = 0;
  pulse_bb_z_min = 0;
  pulse_bb_x_max = 0;
  pulse_bb_y_max = 0;
  pulse_bb_z_max = 0;

  i_min = 0;
  j_min = 0;
  k_min = 0;
  i_max = 0;
  j_max = 0;
  k_max = 0;

  pulse_x_extent = 0;
  pulse_y_extent = 0;
  pulse_z_extent = 0;
}

void LidarVolume::calculateBoundingBox(PULSEreader *pReader){
  //std::cout << pReader->header.min_x << std::endl;
  //std::cout << pReader->header.min_x << std::endl;
  //std::cout << pReader->header.min_x << std::endl;
  

  pulse_bb_x_min = (floor(pReader->header.min_x) - 10);
  pulse_bb_y_min = (floor(pReader->header.min_y) - 10);
  pulse_bb_z_min = (floor(pReader->header.min_z) - 10);
  pulse_bb_x_max = (ceil(pReader->header.max_x) + 10);
  pulse_bb_y_max = (ceil(pReader->header.max_y) + 10);
  pulse_bb_z_max = (ceil(pReader->header.max_z) + 10);

  //i_min = floor(bb_x_min) - 10;
  //j_min = floor(bb_y_min) - 10;
 // k_min = floor(bb_z_min) - 10;
 // i_max = ceil(bb_x_max) + 10;
 // j_max = ceil(bb_y_max) + 10;
 // k_max = ceil(bb_z_max) + 10;

  pulse_x_extent = pulse_bb_x_max - pulse_bb_x_min;
  pulse_y_extent = pulse_bb_y_max - pulse_bb_y_min;
  pulse_z_extent = pulse_bb_z_max - pulse_bb_z_min;
}