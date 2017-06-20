/*
 * File name: ScannerInformation.hpp
 * Created on: 15-June-2017
 * Author: ravi
 */

#ifndef SCANNERINFORMATION_HPP_
#define SCANNERINFORMATION_HPP_

#include <iostream>
#include <fstream>
#include "pulsereader.hpp"
#include "pulsewriter.hpp"

class ScannerInformation{

public:
  //possible parameters
  int scannerId; 
  double waveLength;
  double outgoingPulseWidth;
  int scanPattern;
  int numberOfMirrorFacets;
  double scanFrequency;
  double scanAngleMin;
  double scanAngleMax;
  double pulseFrequency;
  double beamDiameterAtExitAperture;
  double beamDivergence;
  double minimalRange;
  double maximalRange;

  PULSEreadOpener pOpener;
  PULSEreader *pReader;
  PULSEscanner scanner;

  ScannerInformation();
  
  void setScannerInformation(std::string fileName);
  void getScannerInformation();
};

#endif /* SCANNERINFORMATION_HPP_ */
