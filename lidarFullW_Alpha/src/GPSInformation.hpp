/*
 * File name: GPSInformation.hpp
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

private:
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

  PULSEreadOpener pOpener;
  PULSEreader *pReader;
  PULSEscanner scanner;

public:
  

  GPSInformation();  
  void setGPSInformation();
  void writeToFileGPSInformation(std::string fileName);

};

#endif /* GPSINFORMATION_HPP_ */
