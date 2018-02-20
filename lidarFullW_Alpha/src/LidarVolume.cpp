// File name: LidarVolume.cpp
// Created on: 08-November-2017
// Author: ravi
 
#include "LidarVolume.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

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
  bb_i_max = (int) (ceil(bb_x_max_padded)) - (floor(bb_x_min_padded));
  bb_j_max = (int) (ceil(bb_y_max_padded)) - (floor(bb_y_min_padded));
  bb_k_max = (int) (ceil(bb_z_max_padded)) - (floor(bb_z_min_padded));

  i_extent = bb_i_max - bb_i_min + 1;
  j_extent = bb_j_max - bb_j_min + 1;
  k_extent = bb_k_max - bb_k_min + 1;
}


//allocate memory with a 3d array of the data
void LidarVolume::allocateMemory(){
  // we are going to allocate a 3D array of space that will hold peak 
  // information (we don't know how many per volume)
  volume = (std::vector<Peak>**) calloc (sizeof(std::vector<Peak>*),
                                         i_extent * j_extent * k_extent );
                                  
  raster = (int*)calloc(sizeof(int),i_extent*j_extent);
}


void LidarVolume::deallocateMemory(){
  delete volume;
  volume = NULL; //pointing dangling pointer to null
}


//k is most contiguous
//i is the least contiguous
int LidarVolume::position(int i, int j, int k){
  return k + (j* k_extent) + (i*k_extent*j_extent);
}


void LidarVolume::insert_peak(Peak *peak){
  unsigned int i = gps_to_voxel_x(peak->x_activation);
  unsigned int j = gps_to_voxel_y(peak->y_activation);
  unsigned int k = gps_to_voxel_z(peak->z_activation);
  unsigned long int p = position(i,j,k);

  if(volume[p] == NULL){
    volume[p] = new std::vector<Peak>();
  }
  volume[p]->push_back(*peak);
}


//Convert peak x, y and z values to 
//i, j and k which identifies the voxel space they belong to
int LidarVolume::gps_to_voxel_x(double x){
  int voxel_x = (x - bb_x_min_padded);
  return voxel_x;
}

int LidarVolume::gps_to_voxel_y(double y){
  int voxel_y = (y - bb_y_min_padded);
  return voxel_y;
}

int LidarVolume::gps_to_voxel_z(double z){
  int voxel_z = (z - bb_z_min_padded);
  return voxel_z;
}


void LidarVolume::rasterize(){

  int i,j,k;

  for(i=bb_i_min;i<bb_i_max;i++){
    for(j=bb_j_min;j<bb_j_max;j++){
      for(k=bb_k_max-1;k>=bb_k_min;k--){
        if(volume[position(i,j,k)] != NULL){
          raster[i*j_extent+j] = k;
          break;
        }
      }
    }
  }
}

void LidarVolume::display(){

  int i,j;
  for(i=bb_i_min;i<bb_i_max;i++){
    for(j=bb_j_min;j<bb_j_max;j++){
      printf("%d ",raster[i*j_extent+j]); 
    }
    printf("\n");
  }
  

  return;
}


