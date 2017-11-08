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
  bb_x_min = 0;
  bb_y_min = 0;
  bb_z_min = 0;
  bb_x_max = 0;
  bb_y_max = 0;
  bb_z_max = 0;

  i_min = 0;
  j_min = 0;
  k_min = 0;
  i_max = 0;
  j_max = 0;
  k_max = 0;

  x_extent = 0;
  y_extent = 0;
  z_extent = 0;
}

void LidarVolume::CalculateBoundingBox(PULSEreader *pReader){
  bb_x_min = pReader->header.min_x;
  bb_y_min = pReader->header.min_y;
  bb_z_min = pReader->header.min_z;
  bb_x_max = pReader->header.min_x;
  bb_y_max = pReader->header.min_y;
  bb_z_max = pReader->header.min_z;

  i_min = floor(bb_x_min) - 10;
  j_min = floor(bb_y_min) - 10;
  k_min = floor(bb_z_min) - 10;
  i_max = ceil(bb_x_max) + 10;
  j_max = ceil(bb_y_max) + 10;
  k_max = ceil(bb_z_max) + 10;

  x_extent = i_max - i_min;
  y_extent = j_max - j_min;
  z_extent = k_max - k_min;
}