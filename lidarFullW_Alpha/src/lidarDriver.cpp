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
  intermediateData.allocateMemory();

  PulseData pd;
  std::ostringstream stream;
  GaussianFitter fitter;
  std::vector<Peak> peaks;
  int count = 1;
  while(rawData.hasNextPulse()){
    // make sure that we have an empty vector
    for(int i=0;i<peaks.size();i++){
      peaks.pop_back();
    }
    // gets the raw data from the file
    rawData.getNextPulse(&pd);
    try{
      if(!pd.returningIdx.empty()){
        // as long as the pulse has a returning wave it finds
        // the peaks in that wave
        int i;
        for(i=0;i<pd.returningWave.size();i++){
          std::cout << pd.returningWave[i] << " " ;
        }
        int peak_count = fitter.findPeaks(&peaks, pd.returningWave,
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

    pd.displayPulseData(&stream);
    std::cout << stream.str() << std::endl;
    stream.str("");

  }
  // Lidar Volume is full and complete
  // Rasterize it
    // at this point we have all of the data and we need to
    // provide an image of the 3D space.
    // the user can tell us which type of rasterization to use, but for now
    // we are just going to offer one.
    intermediateData.rasterize();
    std::cout<< "This is our display" << std::endl;
    intermediateData.display();

    // Save the image to a PNG file
    // The 'title' string is stored as part of the PNG file
    printf("Saving PNG\n");
    intermediateData.toPng("testfile.png");

  return 0;
}
