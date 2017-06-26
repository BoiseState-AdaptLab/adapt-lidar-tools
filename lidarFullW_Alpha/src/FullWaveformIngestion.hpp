/*
 * File name: FullWaveformIngestion.hpp
 * Created on: 02-June-2017
 * Author: ravi
 */

#ifndef FULLWAVEFORMINGESTION_HPP_
#define FULLWAVEFORMINGESTION_HPP_

#include <iostream>
#include <fstream>
#include "pulsereader.hpp"
#include "pulsewriter.hpp"

class FullWaveformIngestion{

private:
  //possible parameters
  double sampleUnitsBetAnchorAndTarget;
  long long numberOfPulses;

  PULSEreadOpener pOpener;
  PULSEreader *pReader;
  PULSEscanner scanner;  

public:
  FullWaveformIngestion();

  long long getNumberOfPulses(std::string fileName);

  double calculateActualCoordinate(double coordinate, double scaleFactor, 
                                   double offset);
  double calculateDeviation(double anchor, double target);
  double calculateReturnLocation(double actualAnchor, double deviation, 
                                 double time);  
};

#endif /* FULLWAVEFORMINGESTION_HPP_ */
