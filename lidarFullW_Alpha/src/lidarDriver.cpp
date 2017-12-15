/*
 * File name: lidarDriver.cpp
 * Created on: 17-May-2017
 * Author: ravi
 */
 
#include <iostream>
#include <iomanip>
#include <vector>
#include "CmdLine.hpp"
#include "ScannerInformation.hpp"
#include "GPSInformation.hpp"
#include "PulseData.hpp"
#include "FullWaveformIngestion.hpp"
#include "pulsereader.hpp"
#include "pulsewriter.hpp"


using namespace std;

int main (int argc, char *argv[]) {

  // parse the command lines
  CmdLine cmdLineArgs;
  cmdLineArgs.parse(argc,argv);
  
  if(cmdLineArgs.printUsageMessage == true) {
    std::cout << cmdLineArgs.getUsageMessage() << std::endl;
    return 0;
  }

  // create a flight line data object
  std::string fileName = cmdLineArgs.getInputFileName();
  FlightLineData rawData;
  rawData.setFlightLineData(fileName);

  LidarVolume intermediateData;
  intermediateData.setBoundingBox(rawData.bb_x_min,rawData.bb_x_max,
                                  rawData.bb_y_min,rawData.bb_y_max,
                                  rawData.bb_z_min,rawData.bb_z_max);
  intermediateData.allocateSpace();

  
}
