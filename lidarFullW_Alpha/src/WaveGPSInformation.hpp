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
  double xAnchor, yAnchor, zAnchor;
  double xTarget, yTarget, zTarget;
  double xDeviation, yDeviation, zDeviation;
  double xFirst, yFirst, zFirst;
  double xLast, yLast, zLast;
  unsigned char edge;
  unsigned char facet;
  unsigned char scanDirection;
  unsigned char intensity;
  
  double xOffset, yOffset, zOffset;
  double xScale, yScale, zScale;
  double xRealAnchor, yRealAnchor, zRealAnchor;
  double xRealTarget, yRealTarget, zRealTarget;
  double xRealFirst, yRealFirst, zRealFirst;
  double xRealLast, yRealLast, zRealLast;
  double xRealMin, yRealMin, zRealMin;
  double xRealMax, yRealMax, zRealMax;
  
  struct xyzCoordinates {
    double xCoordinate;
    double yCoordinate;
    double zCoordinate;
  };

  WaveGPSInformation();  
  void populateGPS(PULSEreader *pReader);
  void displayGPSData();
  void calculateTargetDeviations(); 
};

#endif /* WAVEGPSINFORMATION_HPP_ */
