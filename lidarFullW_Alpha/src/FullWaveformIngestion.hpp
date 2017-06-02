/*
 * File name: cmd_line.hpp
 * Created on: 02-June-2017
 * Author: ravi
 */

#ifndef FULLWAVEFORMINGESTION_HPP_
#define FULLWAVEFORMINGESTION_HPP_

#include <iostream>
#include <fstream>
#include <getopt.h>
#include <sstream>
#include "pulsereader.hpp"
#include "pulsewriter.hpp"

class FullWaveformIngestion{

private:
  //possible parameters
  double time;
  // helpful stuff
  std::string usageMessage;
  std::string exeName;
  PULSEreadOpener pOpener;
  PULSEreader *pReader;

public:
  // help parameter
  FullWaveformIngestion();

  double calculateActualCoordinate(double coordinate, double scaleFactor, 
                                   double offset);
  double calculateDeviation(double anchor, double target);
  double calculateReturnLocation(double actualAnchor, double deviation, 
                                 double time);  
};

#endif /* FULLWAVEFORMINGESTION_HPP_ */
