// File name: lidarDriver.cpp
// Created on: 17-May-2017
// Author: ravi

#include "CmdLine.hpp"
#include "FlightLineData.hpp"
#include "WaveGPSInformation.hpp"
#include "LidarVolume.hpp"
#include "PulseData.hpp"
#include "Peak.hpp"
#include "GaussianFitter.hpp"
#include <iostream>
#include <iomanip>
#include <vector>


// Lidar driver
int main (int argc, char *argv[]) {

  // Parse the command line args
  CmdLine cmdLineArgs;
  cmdLineArgs.parse(argc,argv);

  if(cmdLineArgs.printUsageMessage == true) {
    std::cout << cmdLineArgs.getUsageMessage() << std::endl;
    return 1;
  }

  std::cout << "\nProcessing  " << argv[2] << std::endl;
  // Create a flight line data object
  std::string fileName = cmdLineArgs.getInputFileName();
  FlightLineData rawData;
  rawData.setFlightLineData(fileName);

  LidarVolume intermediateData;
  intermediateData.setBoundingBox(rawData.bb_x_min,rawData.bb_x_max,
                                  rawData.bb_y_min,rawData.bb_y_max,
                                  rawData.bb_z_min,rawData.bb_z_max);
  intermediateData.allocateMemory();

  PulseData pd;
  std::ostringstream stream;
  GaussianFitter fitter;
  std::vector<Peak> peaks;
  
  #ifdef DEBUG
    std::cerr << "Start finding peaks. In " << __FILE__ << ":" << __LINE__ << std::endl;
  #endif 

  while(rawData.hasNextPulse()){
    // make sure that we have an empty vector
    for(int i=0; i<(int)peaks.size(); i++){
      peaks.pop_back();
    }
    // gets the raw data from the file
    rawData.getNextPulse(&pd);
    try{
      // as long as the pulse has a returning wave it finds
      // the peaks in that wave
      if(!pd.returningIdx.empty()){

        // FOR TESTING PURPOSES
        // for(i=0; i<(int)pd.returningWave.size(); i++){
        //   std::cout << pd.returningWave[i] << " " ;
        // }

        // Smooth the data and test result
        fitter.smoothing_expt(&pd.returningWave);
        int peak_count = fitter.find_peaks(&peaks, pd.returningWave,
                                          pd.returningIdx);

        // foreach peak - find activation point
        //              - calculate x,y,z
        peak_count = rawData.calc_xyz_activation(&peaks);

        // give each peak to lidarVolume
        for(int i=0;i<peak_count;i++){
          intermediateData.insert_peak(&peaks[i]);
        }
      }
    }
    catch (const char* msg){
      std::cerr << msg << std::endl;
    }

    // FOR TESTING PURPOSES
    #ifdef DEBUG
      pd.displayPulseData(&stream);
      std::cout << stream.str() << std::endl;
      stream.str("");
    #endif

  }
  
  #ifdef DEBUG
    std::cerr << "Total: " << fitter.get_total() << std::endl;
    std::cerr << "Pass: " << fitter.get_pass() << std::endl;
    std::cerr << "Fail: " << fitter.get_fail() << std::endl;
  #endif

  #ifdef DEBUG
    std::cerr << "Peak finding complete. Going to start writing GeoTIF. In lidarDriver:94" << std::endl; 
  #endif
  
  // Save the image to a geotiff file
  // The 'title' string is stored as part of the file
  std::cout << "Writing GeoTIFF " << std::endl;
  intermediateData.toTif("tifTest.tif", rawData.geog_cs, rawData.utm);
  std::cout << "All done!\n " << std::endl;
  
  return 0;
}

