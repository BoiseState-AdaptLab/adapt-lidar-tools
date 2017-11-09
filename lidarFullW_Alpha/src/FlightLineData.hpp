/*
 * File name: FlightLineData.hpp
 * Created on: 09- November-2017
 * Author: ravi
 */

#ifndef FLIGHTLINEDATAUNITTESTS_HPP_
#define FLIGHTLINEDATAUNITTESTS_HPP_

#include <iostream>
#include <fstream>
#include "pulsereader.hpp"
#include "pulsewriter.hpp"

class FlightLineData{

public:
  //The min and max fields describing the boundary box(bb) that includes the 
  //first & last points of the sampled parts of the returning waveforms of 
  //all pulses
  int pulse_bb_x_min;
  int pulse_bb_y_min;
  int pulse_bb_z_min;
  int pulse_bb_x_max;
  int pulse_bb_y_max;
  int pulse_bb_z_max;  

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

  FlightLineData();
  
  void setFlightLineData(std::string fileName);
  void getFlightLineData();
};

#endif /* FLIGHTLINEDATAUNITTESTS_HPP_ */