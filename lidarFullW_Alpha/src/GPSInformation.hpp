/*
 * File name: GPSInformation.hpp
 * Created on: 20-June-2017
 * Author: ravi
 */

#ifndef GPSINFORMATION_HPP_
#define GPSINFORMATION_HPP_

#include <iostream>
#include "pulsereader.hpp"
#include "pulsewriter.hpp"

class GPSInformation{

private:
  //possible parameters
  long long gpsIndex;
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


public:  
  GPSInformation();  
  void populateGPS(PULSEreader *pReader, long long index);
  void displayGPSData();
};

#endif /* GPSINFORMATION_HPP_ */
