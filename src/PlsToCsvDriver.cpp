// File name: PlsToCsvDriver.cpp
// Created on: February 2020
// Author: Nicholas Prussen, Spencer Fleming


#include "PlsToCsvHeader.hpp"
#include <chrono>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"

//Clock for counting time processed
typedef std::chrono::high_resolution_clock Clock;


// This is the main driver
int main (int argc, char *argv[]) {

    // Setting up logger
    spdlog::set_pattern("[%^%=8l%$] %v");
    
    // Objects needed for processing
    FlightLineData rawData;     // Wave inputs from pls file
    PlsToCsvHelper helper;      // Helper functions
    csv_CmdLine cmdLineArgs;    // Command line options

    // Pass argv to CmdLine for parsing and validation
    if(!cmdLineArgs.parse_args(argc,argv)){
        return 1;
    }

    // Grab the filename for the pls file
    // TODO Need to remove the commandline arg
    std::string plsFileName = cmdLineArgs.getInputFileName(true);

    // Collect start time
    Clock::time_point t1 = Clock::now();
    
    spdlog::info("Processing {}", plsFileName);

    // Initialize data input per CmdLine specification
    
    // ingest the raw flight data into an object
    if (rawData.setFlightLineData(plsFileName)) {
        return 1;
    }

    std::vector<Peak*> peaks = helper.fit_data_csv(rawData, cmdLineArgs);
    for(int product : cmdLineArgs.selected_products){
        std::string property = helper.getPeaksProperty(peaks, product);
        std::string fileName = cmdLineArgs.get_output_filename(product);
        if(!helper.writeLinesToFile(fileName, {property})){
            spdlog::error("Failed to write to file {}", fileName);
        }
    }

    // Free memory
    rawData.closeFlightLineData();

    // Get end time
    Clock::time_point t2 = Clock::now();

    // Compute total run time and convert to appropriate units
    double diff = std::chrono::duration_cast<std::chrono::seconds>(t2-t1).
        count();
    spdlog::info("All done!\nTime elapsed: {} seconds", diff);

    return 0;
}
