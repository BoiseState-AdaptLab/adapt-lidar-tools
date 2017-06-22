/*
 * File name: ScannerInformation.hpp
 * Created on: 20-June-2017
 * Author: ravi
 */

#ifndef GPSINFORMATION_HPP_
#define GPSINFORMATION_HPP_

#include <iostream>
#include <fstream>
#include "pulsereader.hpp"
#include "pulsewriter.hpp"

class GPSInformation{

public:
  //possible parameters
  long long noOfPulses;
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

  PULSEreadOpener pOpener;
  PULSEreader *pReader;
  PULSEscanner scanner;

  GPSInformation();
  
  void setGPSInformation();
  long long getNumberOfPulses(std::string fileName);
  void writeGPSInformation();
  void setGPSInformation(int index);

};

#endif /* GPSINFORMATION_HPP_ */
