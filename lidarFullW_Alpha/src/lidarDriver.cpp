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
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;
void parse_data(FlightLineData &);
void fit_data(FlightLineData &, LidarVolume &);
void produce_product(FlightLineData &, LidarVolume &);

CmdLine cmdLineArgs;

// Lidar driver
int main (int argc, char *argv[]) {

  // Parse and validate the command line args
    if(!cmdLineArgs.parse_args(argc,argv)){
        exit(1);
    }

  //Collect start time
  Clock::time_point t1 = Clock::now();

  std::cout << "\nProcessing  " << argv[2] << std::endl;

  // Create a flight line data object
  FlightLineData rawData;
  LidarVolume intermediateData;

  //parse the data
  parse_data(rawData);

  //fit data
  fit_data(rawData,intermediateData);

  //produce the product
  produce_product(rawData,intermediateData);
  
  //Get end time
  //Compute total run time and convert to appropriate units
  Clock::time_point t2 = Clock::now();
  double diff = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count(); 
  std::cout << "All done!\nTime elapsed: " << diff << " seconds\n" << std::endl;

  return 0;
}

void parse_data(FlightLineData &data){
    data.setFlightLineData(cmdLineArgs.getInputFileName());
}

void fit_data(FlightLineData &raw_data, LidarVolume &fitted_data) {
    fitted_data.setBoundingBox(raw_data.bb_x_min, raw_data.bb_x_max,
                               raw_data.bb_y_min, raw_data.bb_y_max,
                               raw_data.bb_z_min, raw_data.bb_z_max);
    fitted_data.allocateMemory();

    PulseData pd;
    std::ostringstream stream;
    GaussianFitter fitter;
    std::vector<Peak> peaks;

    #ifdef DEBUG
        std::cerr << "Start finding peaks. In " << __FILE__ << ":" << __LINE__ << std::endl;
    #endif

    std::string fit_type = cmdLineArgs.useGaussianFitting ? "gaussian fitting" : "first difference";
    std::cerr << "Finding peaks with " << fit_type << std::endl;

    while (raw_data.hasNextPulse()) {
        // make sure that we have an empty vector
        for (int i = 0; i < (int) peaks.size(); i++) {
            peaks.pop_back();
        }
        // gets the raw data from the file
        raw_data.getNextPulse(&pd);
        try {
            // as long as the pulse has a returning wave it finds
            // the peaks in that wave
            if (!pd.returningIdx.empty()) {

                // FOR TESTING PURPOSES
                // for(i=0; i<(int)pd.returningWave.size(); i++){
                //   std::cout << pd.returningWave[i] << " " ;
                // }

                // Smooth the data and test result
                fitter.smoothing_expt(&pd.returningWave);

                // Check parameter for using gaussian fitting or estimating
                int peak_count;
                if (cmdLineArgs.useGaussianFitting == false) {
                    peak_count = fitter.guess_peaks(&peaks, pd.returningWave,
                                                    pd.returningIdx);
                } else {
                    peak_count = fitter.find_peaks(&peaks, pd.returningWave,
                                                   pd.returningIdx);
                }

                // foreach peak - find activation point
                //              - calculate x,y,z
                peak_count = raw_data.calc_xyz_activation(&peaks);

                // give each peak to lidarVolume
                for (int i = 0; i < peak_count; i++) {
                    fitted_data.insert_peak(&peaks[i]);
                }
            }
        } catch (const char *msg) {
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

}

void produce_product(FlightLineData &raw_data, LidarVolume &fitted_data){
    #ifdef DEBUG
        std::cerr << "Peak finding complete. Going to start writing GeoTIF. In lidarDriver:94" << std::endl;
    #endif

    // Save the image to a geotiff file
    // The 'title' string is stored as part of the file
    std::cout << "Writing GeoTIFF " << std::endl;
    fitted_data.toTif(cmdLineArgs.get_output_filename(), raw_data.geog_cs, raw_data.utm);
}