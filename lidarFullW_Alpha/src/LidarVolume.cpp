// File name: LidarVolume.cpp
// Created on: 08-November-2017
// Author: ravi
 
#include "LidarVolume.hpp"
#include <math.h>
#include <stdlib.h>
#include "Peak.hpp"

//Default constructor
LidarVolume::LidarVolume(){
  //initialize to zero
  bb_x_min_padded = 0;
  bb_y_min_padded = 0;
  bb_z_min_padded = 0;
  bb_x_max_padded = 0;
  bb_y_max_padded = 0;
  bb_z_max_padded = 0;

  bb_i_min = 0;
  bb_j_min = 0;
  bb_k_min = 0;
  bb_i_max = 0;
  bb_j_max = 0;
  bb_k_max = 0;

  i_extent = 0;
  j_extent = 0;
  k_extent = 0;
  
  currentPeak = 0;
  numOfPeaks = 5;
  //peakData = NULL;  
}

void LidarVolume::setBoundingBox(double ld_xMin, double ld_xMax,
                                       double ld_yMin, double ld_yMax,
                                       double ld_zMin, double ld_zMax){  

  bb_x_min_padded = ld_xMin - 10;
  bb_y_min_padded = ld_yMin - 10;
  bb_z_min_padded = ld_zMin - 10;
  bb_x_max_padded = ld_xMax + 10;
  bb_y_max_padded = ld_yMax + 10;
  bb_z_max_padded = ld_zMax + 10;

  bb_i_min = 0;
  bb_j_min = 0;
  bb_k_min = 0;
  bb_i_max = (int)(ceil(bb_x_max_padded - bb_x_min_padded));
  bb_j_max = (int)(ceil(bb_y_max_padded - bb_y_min_padded));
  bb_k_max = (int)(ceil(bb_z_max_padded - bb_z_min_padded));

  i_extent = bb_i_max - bb_i_min + 1;
  j_extent = bb_j_max - bb_j_min + 1;
  k_extent = bb_k_max - bb_k_min + 1;
}


//allocate memory with a 3d array of the data
void LidarVolume::allocateMemory(){
  // we are going to allocate a 3D array of space that will hold peak 
  // information (we don't know how many per volume)
  volume = (std::vector<Peak>**) malloc (i_extent * j_extent * k_extent 
                                  * sizeof(std::vector<Peak>*));
}

void LidarVolume::deallocateMemory(){
  delete volume;
  volume = NULL; //pointing dangling pointer to null
}

int LidarVolume::position(int i, int j, int k){
  //return k + (j* len(k)) + (i*len(k)*len(j));
  return 0;
}

void LidarVolume::insertPeak(){

}


void LidarVolume::gps_to_voxel_x(){
  
}

void LidarVolume::gps_to_voxel_y(){
  
}

void LidarVolume::gps_to_voxel_z(){
  
}
