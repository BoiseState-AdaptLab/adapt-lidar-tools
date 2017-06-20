/*
 * File name: lidarDriver.cpp
 * Created on: 17-May-2017
 * Author: ravi
 */
#include <iostream>
#include "cmdLine.hpp"
#include "ScannerInformation.hpp"

using namespace std;

int main (int argc, char *argv[]){

  CmdLineArgs cla;
  cla.parse(argc,argv);
  
  if(cla.printUsageMessage == true || cla.exceptionFlag == true){
    std::cout << cla.getUsageMessage() << std::endl;
  }
  else{
    ScannerInformation scannerInfo;
    scannerInfo.setScannerInformation(cla.getInputFileName());
    scannerInfo.getScannerInformation();
  }

  return 0;
}
