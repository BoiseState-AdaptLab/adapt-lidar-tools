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

  pulse_bb_i_min = 0;
  pulse_bb_j_min = 0;
  pulse_bb_k_min = 0;
  pulse_bb_i_max = 0;
  pulse_bb_j_max = 0;
  pulse_bb_k_max = 0;

  pulse_x_extent = 0;
  pulse_y_extent = 0;
  pulse_z_extent = 0;
}

void LidarVolume::calculateBoundingBox(PULSEreader *pReader){
  //std::cout << pReader->header.min_x << std::endl;
  //std::cout << pReader->header.min_x << std::endl;
  //std::cout << pReader->header.min_x << std::endl;
  

  pulse_bb_x_min = pReader->header.min_x;
  pulse_bb_y_min = pReader->header.min_y;
  pulse_bb_z_min = pReader->header.min_z;
  pulse_bb_x_max = pReader->header.max_x;
  pulse_bb_y_max = pReader->header.max_y;
  pulse_bb_z_max = pReader->header.max_z;

  pulse_bb_i_min = (int) floor(pulse_bb_x_min) - 10;
  pulse_bb_j_min = (int) floor(pulse_bb_y_min) - 10;
  pulse_bb_k_min = (int) floor(pulse_bb_z_min) - 10;
  pulse_bb_i_max = (int) ceil(pulse_bb_x_max) + 10;
  pulse_bb_j_max = (int) ceil(pulse_bb_y_max) + 10;
  pulse_bb_k_max = (int) ceil(pulse_bb_z_max) + 10;

  pulse_x_extent = pulse_bb_i_max - pulse_bb_i_min;
  pulse_y_extent = pulse_bb_j_max - pulse_bb_j_min;
  pulse_z_extent = pulse_bb_k_max - pulse_bb_k_min;
}