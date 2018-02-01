/*
 * File name: WaveGPSInformation.hpp
 * Created on: 20-June-2017
 * Author: ravi
 */

#ifndef WAVEGPSINFORMATION_HPP_
#define WAVEGPSINFORMATION_HPP_

#include <iostream>
#include "pulsereader.hpp"
#include "pulsewriter.hpp"

class WaveGPSInformation{

private:
public:  
  //possible parameters
  double gpsTime;
  double x_anchor, y_anchor, z_anchor;
  double x_target, y_target, z_target;
  double dx, dy, dz;  //deviation
  double x_first, y_first, z_first;
  double x_last, y_last, z_last;
  unsigned char edge;
  unsigned char facet;
  unsigned char scanDirection;
  unsigned char intensity;
  
  double xOffset, yOffset, zOffset;
  double xScale, yScale, zScale;
  double xReal_anchor, yReal_anchor, zReal_anchor;
  double xReal_target, yReal_target, zReal_target;
  double xReal_first, yReal_first, zReal_first;
  double xReal_last, yReal_last, zReal_last;
  double xRealMin, yRealMin, zRealMin;
  double xRealMax, yRealMax, zRealMax;
  
  WaveGPSInformation();  
  void populateGPS(PULSEreader *pReader);
  void displayGPSData();
};

#endif /* WAVEGPSINFORMATION_HPP_ */
