/*
 * File name: lidarDriver.cpp
 * Created on: 17-May-2017
 * Author: ravi
 */
#include <iostream>
#include <iomanip>
#include "CmdLine.hpp"
#include "ScannerInformation.hpp"
#include "GPSInformation.hpp"

using namespace std;

int main (int argc, char *argv[]){

  CmdLine cmdLineArgs;
  cmdLineArgs.parse(argc,argv);
  
  if(cmdLineArgs.printUsageMessage == true){
    std::cout << cmdLineArgs.getUsageMessage() << std::endl;
  }
  else{
    ScannerInformation scannerInfo;
    scannerInfo.setScannerInformation(cmdLineArgs.getInputFileName());
    scannerInfo.getScannerInformation();
    
    GPSInformation gpsInfo;
    std::cout << "No of pulses: " << gpsInfo.getNumberOfPulses \
                                    (cmdLineArgs.getInputFileName()) <<std::endl;
    gpsInfo.writeGPSInformation();
    
  }

  return 0;
}
