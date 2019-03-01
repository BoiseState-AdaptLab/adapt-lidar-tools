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
void parse_data(FlightLineData &, std::string inputFileName);
void fit_data(FlightLineData &, LidarVolume &, bool useGaussianFitting);
void produce_product(FlightLineData &, LidarVolume &, std::string outputFilename, bool maxElevationFlag);

// Lidar driver
int main (int argc, char *argv[]) {

    CmdLine cmdLineArgs;

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
  parse_data(rawData, cmdLineArgs.getInputFileName());

  //fit data
  fit_data(rawData,intermediateData, cmdLineArgs.useGaussianFitting);

  //produce the product
  produce_product(rawData, intermediateData, cmdLineArgs.get_output_filename(), cmdLineArgs.max_elevation_flag);

  //Get end time
  //Compute total run time and convert to appropriate units
  Clock::time_point t2 = Clock::now();
  double diff = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
  std::cout << "All done!\nTime elapsed: " << diff << " seconds\n" << std::endl;

  return 0;
}


/**
 * parses the raw data from input file into a FlightLineData object
 * @param data the FlightLineData object to store the raw data in
 */
void parse_data(FlightLineData &data, std::string inputFileName){
    data.setFlightLineData(inputFileName);
}


/**
 * fits the raw data using either gaussian or first difference fitting
 * @param raw_data reference to FlightLineData object that holds raw data
 * @param fitted_data reference to LidarVolume object to store fit data in
 */
void fit_data(FlightLineData &raw_data, LidarVolume &fitted_data, bool useGaussianFitting) {
    PulseData pd;
    std::ostringstream stream;
    GaussianFitter fitter;
    std::vector<Peak> peaks;

    fitted_data.setBoundingBox(raw_data.bb_x_min, raw_data.bb_x_max,
                               raw_data.bb_y_min, raw_data.bb_y_max,
                               raw_data.bb_z_min, raw_data.bb_z_max);
    fitted_data.allocateMemory();

    #ifdef DEBUG
        std::cerr << "Start finding peaks. In " << __FILE__ << ":" << __LINE__ << std::endl;
    #endif

    std::string fit_type = useGaussianFitting ? "gaussian fitting" : "first difference";
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
                if (useGaussianFitting == false) {
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

/**
 * create the output as specified by the command line arguments
 * @param raw_data the parsed FlightLineData
 * @param fitted_data the fitted LidarVolume data
 */

void produce_product(FlightLineData &raw_data, LidarVolume &fitted_data, std::string outputFilename, bool maxElevationFlag){
    #ifdef DEBUG
        std::cerr << "Peak finding complete. Going to start writing GeoTIF. In lidarDriver:94" << std::endl;
    #endif

    // Save the image to a geotiff file
    // The 'title' string is stored as part of the file
    std::cout << "Writing GeoTIFF " << std::endl;
    fitted_data.toTif(outputFilename, maxElevationFlag, raw_data.geog_cs, raw_data.utm);
}
