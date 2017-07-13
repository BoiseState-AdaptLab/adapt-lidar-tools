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
#include "AmplitudeData.hpp"
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
    
    FullWaveformIngestion ingester;
    long long int noOfPulses = ingester.getNumberOfPulses(fileName);
  
    /*
     * This section reads the wave and GPS file
     */
    PULSEreadOpener pOpener;
    PULSEreader *pReader;
    WAVESsampling *sampling;
    pOpener.set_file_name(fileName.c_str());
    pReader = pOpener.open();

    GPSInformation gpsInfo;
    AmplitudeData ad;
    int maxCount = 60;
    long long pulseIndex = 0;

    
      while(pReader->read_pulse()){
        std::cout << "\nIndex is: " << pulseIndex << std::endl;
        
        gpsInfo.populateGPS(pReader);
        gpsInfo.displayGPSData();
        
        if(pReader->read_waves()){
          for(int i = 0; i < pReader->waves->get_number_of_samplings(); i++){
            sampling = pReader->waves->get_sampling(i);

            for(int j = 0; j < sampling->get_number_of_segments(); j++ ){
              sampling->set_active_segment(j);
                
                for(int k = 0; k < maxCount; k++){
                  if(sampling->get_type() == PULSEWAVES_OUTGOING){
                    if(k >= sampling->get_number_of_samples()){
                      ad.outgoingWave.push_back(0);
                    } 
                    else{
                      ad.outgoingWave.push_back(sampling->get_sample(k));
                    }
                  }
                  else if(sampling->get_type() == PULSEWAVES_RETURNING){
                    if(k >= sampling->get_number_of_samples()){
                      ad.returningWave.push_back(0);
                    } 
                    else{
                      ad.returningWave.push_back(sampling->get_sample(k));
                    }
                  }
                }
            }

          }


          pulseIndex++;
        }
        
        else{
          /* No data??? */
          std::cout <<"NO DATA!\n" << std::endl;
        }
      }
  ad.displayAmplitudeData();

  return 0;
  }
}
