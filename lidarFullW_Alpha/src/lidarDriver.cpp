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

int main (int argc, char *argv[]){

  CmdLine cmdLineArgs;
  cmdLineArgs.parse(argc,argv);
  
  if(cmdLineArgs.printUsageMessage == true){
    std::cout << cmdLineArgs.getUsageMessage() << std::endl;
  }
  else{
    std::string fileName = cmdLineArgs.getInputFileName();
    
    // Write Scanner information to a file
    ScannerInformation scannerInfo;
    scannerInfo.setScannerInformation(fileName);
    scannerInfo.getScannerInformation();  
  
    /*
     * This section opens the pls file and gets it ready
     * to be read
     */
    PULSEreadOpener pOpener;
    PULSEreader *pReader;
    WAVESsampling *sampling;
    pOpener.set_file_name(fileName.c_str());
    pReader = pOpener.open();

    GPSInformation gpsInfo; //This object holds the GPS information

    int maxCount = 60;
    long long pulseIndex = 0; // Index

    //Holds all the pulses with outgoing and returning wave data
    std::vector<PulseData*> pulses;
    PulseData* pd;  //Pointer to a PulseData obj that can be added to pulses
    
    while(pReader->read_pulse()){
      // std::cout << "\nIndex is: " << pulseIndex << std::endl;
      
      gpsInfo.populateGPS(pReader);
      // gpsInfo.displayGPSData();

      //Read the waves
      if(pReader->read_waves()){
        pd = new PulseData(); //Initialize a new pointer at each pulseIndex
        for(int i = 0; i < pReader->waves->get_number_of_samplings(); i++){
          sampling = pReader->waves->get_sampling(i);

          //Based on the type of wave, populate data
          if(sampling->get_type() == PULSEWAVES_OUTGOING){
            pd->populateOutgoing(sampling, maxCount, pulseIndex);

          }
          else if(sampling->get_type() == PULSEWAVES_RETURNING){
            pd->populateReturning(sampling, maxCount, pulseIndex);
          }
          else{
            std::cout << "Unknown type: " << sampling->get_type() \
                      << std::endl;
          }
        }
      }

      //No waves
      else{
        std::cout <<"NO DATA!\n" << std::endl;
      }
      pulses.push_back(pd);
      pulseIndex++;
    }

    std::cout << "Total no of pulses: " << pulses.size() << std::endl;

    for(int i = 0; i < (int)pulses.size(); i++){
      if(cmdLineArgs.getPeakFlag() == false){
        pulses[i]->calculateFirstDifference();
        pulses[i]->calculateSecondDifference();
        pulses[i]->calculateSmoothSecondDifference();
        pulses[i]->findPeaks(pulses[i]->smoothSecondDifference);
      }
      else{        
        pulses[i]->findPeaks(pulses[i]->returningWave);
      }
    }

    if(cmdLineArgs.getPeakFlag() == true){
      std::ofstream outfile;
      outfile.open("peaksAndLocations.csv");
      outfile << "Pulse Index" << ",";
      outfile << "Peak Value & location --->"<< "\n";
      for(int i = 0; i < (int)pulses.size(); i++){
        outfile << i << ",";
        for(int j = 0; j<(int)pulses[i]->peaks.size(); j++){
          outfile << pulses[i]->peaks[j] << " at ";
          outfile << pulses[i]->peaksLocation[j] << ",";
        }
        outfile << "\n";
      }
    }
    else{
      std::ofstream outfile;
      outfile.open("peaksAndLocations.csv");
      outfile << "Pulse Index" << ",";
      outfile << "Peak Value & location --->"<< "\n";
    }


    return 0;
  }
}
