/*
 * File name: LidarVolume.cpp
 * Created on: 08-November-2017
 * Author: ravi
 */

#ifndef LIDARVOLUME_HPP_
#define LIDARVOLUME_HPP_

public:
  //The min and max fields describing the boundary box(bb) that includes the 
  //first & last points of the sampled parts of the returning waveforms of 
  //all pulses
  double bb_x_min;
  double bb_y_min;
  double bb_z_min;
  double bb_x_max;
  double bb_y_max;
  double bb_z_max;

  //floor of the bb_mins
  int i_min;
  int j_min;
  int k_min;
  //ceil of the bb_maxes
  int i_max;
  int j_max;
  int k_max;

  //extent = i,j,k maxes - mins
  int x_extent;
  int y_extent;
  int z_extent;

  //Read and store the mins and maxes from the header, calculate and store the
  //i, j,k values and the extents
  CalculateBoundingBox(PULSEreader* pReader);


#endif /* LIDARVOLUME_HPP_ */

