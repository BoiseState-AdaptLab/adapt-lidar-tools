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

private:
  //possible parameters
  int scannerId[10]; 
  double waveLength[10];
  double outgoingPulseWidth[10];
  int scanPattern[10];
  int numberOfMirrorFacets[10];
  double scanFrequency[10];
  double scanAngleMin[10];
  double scanAngleMax[10];
  double pulseFrequency[10];
  double beamDiameterAtExitAperture[10];
  double beamDivergence[10];
  double minimalRange[10];
  double maximalRange[10];

  PULSEreadOpener pOpener;
  PULSEreader *pReader;
  PULSEscanner scanner;

public:
  ScannerInformation();
  
  void setScannerInformation(std::string fileName);
  void getScannerInformation();
};

#endif /* SCANNERINFORMATION_HPP_ */
