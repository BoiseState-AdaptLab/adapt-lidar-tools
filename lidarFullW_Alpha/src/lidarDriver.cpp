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
void setup_flight_data(FlightLineData &data, std::string inputFileName);
void fit_data(FlightLineData &, LidarVolume &, bool useGaussianFitting);
void produce_product(FlightLineData &, LidarVolume &, std::string outputFilename, bool maxElevationFlag);
void setup_lidar_volume(FlightLineData &, LidarVolume &);
int parse_pulse(PulseData &, std::vector<Peak> &, GaussianFitter &, bool , int &);
void add_peaks_to_volume(LidarVolume &,std::vector<Peak> &, int );

// Lidar driver
int main (int argc, char *argv[]) {

    CmdLine cmdLineArgs;
	FlightLineData rawData;
	LidarVolume intermediateData;

  // Parse and validate the command line args
    if(!cmdLineArgs.parse_args(argc,argv)){
        exit(1);
    }

  //Collect start time
  Clock::time_point t1 = Clock::now();

  std::cout << "\nProcessing  " << argv[2] << std::endl;

  //ingest the raw flight data into an object
  setup_flight_data(rawData, cmdLineArgs.getInputFileName());

  //fit data
  fit_data(rawData,intermediateData, cmdLineArgs.useGaussianFitting);

  //produce the product
  produce_product(rawData, intermediateData, cmdLineArgs.get_output_filename(), cmdLineArgs.max_elevation_flag);

  //Get end time
  Clock::time_point t2 = Clock::now();

  //Compute total run time and convert to appropriate units
  double diff = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
  std::cout << "All done!\nTime elapsed: " << diff << " seconds\n" << std::endl;

  return 0;
}


/**
 * creates FlightLineData object from the raw data in the input file
 * @param data the FlightLineData object to store the raw data in
 * @param inputFileName the file name and path of the input file
 */
void setup_flight_data(FlightLineData &data, std::string inputFileName){
    data.setFlightLineData(inputFileName);
}

/**
 * fits the raw data using either gaussian or first difference fitting
 * @param raw_data reference to FlightLineData object that holds raw data
 * @param fitted_data reference to LidarVolume object to store fit data in
 * @param useGaussianFitting flag to indicate fitting type
 */
void fit_data(FlightLineData &raw_data, LidarVolume &fitted_data, bool useGaussianFitting) {
    PulseData pd;
    std::ostringstream stream;
    GaussianFitter fitter;
    std::vector<Peak> peaks;
    int peak_count = 0;

    #ifdef DEBUG
        std::cerr << "Start finding peaks. In " << __FILE__ << ":" << __LINE__ << std::endl;
	#endif

        //setup the lidar volume bounding and allocate memory
	setup_lidar_volume(raw_data, fitted_data);

	//message the user
    std::string fit_type = useGaussianFitting ? "gaussian fitting" : "first difference";
    std::cerr << "Finding peaks with " << fit_type << std::endl;

    //parse each pulse
    while (raw_data.hasNextPulse()) {
        // make sure that we have an empty vector
	    peaks.clear();

        // gets the raw data from the file
        raw_data.getNextPulse(&pd);
        try {
            // as long as the pulse has a returning wave it finds
            // the peaks in that wave
			if(parse_pulse(pd, peaks, fitter, useGaussianFitting, peak_count)) {
				// foreach peak - find activation point
				//              - calculate x,y,z
				peak_count = raw_data.calc_xyz_activation(&peaks);

				add_peaks_to_volume(fitted_data, peaks, peak_count);
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
 * setup the bounding and allocate memory for the LidarVolume
 * @param raw_data the flight light data to get values from
 * @param lidar_volume the lidar volume object to allocate
 */
void setup_lidar_volume(FlightLineData &raw_data, LidarVolume &lidar_volume){
	lidar_volume.setBoundingBox(raw_data.bb_x_min, raw_data.bb_x_max,
	                            raw_data.bb_y_min, raw_data.bb_y_max,
	                            raw_data.bb_z_min, raw_data.bb_z_max);
	lidar_volume.allocateMemory();
}

/**
 * add the values from the peaks vector into the lidar volume object
 * @param lidar_volume the lidar volume to add peaks to
 * @param peaks the vector of peaks to add to the lidar volume
 * @param peak_count the count of peaks in the vector ?peaks.size()?
 */
void add_peaks_to_volume(LidarVolume &lidar_volume, std::vector<Peak> &peaks, int peak_count){
	// give each peak to lidarVolume
	for (int i = 0; i < peak_count; i++) {
		lidar_volume.insert_peak(&peaks[i]);
	}
}

/**
 * parse the individual pulse and find its peaks
 * @param pulse the pulse wave to parse
 * @param peaks the empty vector to add peaks to
 * @param fitter the gaussian fitter object to use for smoothing and fitting
 * @param use_gaussian_fitting flag to indicate fitter type
 * @param peak_count count of found peaks returned
 * @return -1 if the pulse was empty, otherwise 1
 */
int parse_pulse(PulseData &pulse, std::vector<Peak> &peaks, GaussianFitter &fitter, bool use_gaussian_fitting, int
&peak_count){

	if (pulse.returningIdx.empty()) {
		return -1;
	}

	// FOR TESTING PURPOSES
	// for(i=0; i<(int)pd.returningWave.size(); i++){
	//   std::cout << pd.returningWave[i] << " " ;
	// }

	// Smooth the data and test result
	fitter.smoothing_expt(&pulse.returningWave);

	// Check parameter for using gaussian fitting or estimating
	if (use_gaussian_fitting == false) {
		peak_count = fitter.guess_peaks(&peaks, pulse.returningWave,
		                                pulse.returningIdx);
	} else {
		peak_count = fitter.find_peaks(&peaks, pulse.returningWave,
		                               pulse.returningIdx);
	}

	return 1;
}

/**
 * create the output as specified by the command line arguments
 * @param raw_data the parsed FlightLineData
 * @param fitted_data the fitted LidarVolume data
 * @param outputFilename name for the output file
 * @param maxElevationFlag flag to indicate elevation type
 */
void produce_product(FlightLineData &raw_data, LidarVolume &fitted_data, std::string outputFilename, bool
maxElevationFlag){
    #ifdef DEBUG
        std::cerr << "Peak finding complete. Going to start writing GeoTIF. In lidarDriver:94" << std::endl;
    #endif

    // Save the image to a geotiff file
    // The 'title' string is stored as part of the file
    std::cout << "Writing GeoTIFF " << std::endl;
    fitted_data.toTif(outputFilename, maxElevationFlag, raw_data.geog_cs, raw_data.utm);
}
