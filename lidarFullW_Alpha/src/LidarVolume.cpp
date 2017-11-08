/*
 * File name: LidarVolume.cpp
 * Created on: 08-November-2017
 * Author: ravi
 */

#include "PulseData.hpp"
#include "pulsereader.hpp"
#include "pulsewriter.hpp"
#include "ScanData.hpp"
#include <math.h>

//Default constructor
LidarVolume::LidarVolume(){
  //initialize to zero
  double bb_x_min = 0;
  double bb_y_min = 0;
  double bb_z_min = 0;
  double bb_x_max = 0;
  double bb_y_max = 0;
  double bb_z_max = 0;

  int i_min = 0;
  int j_min = 0;
  int k_min = 0;
  int i_max = 0;
  int j_max = 0;
  int k_max = 0;

  int x_extent = 0;
  int y_extent = 0;
  int z_extent = 0;
}

void LidarVolume::CalculateBoundingBox(PULSEreader* pReader){
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