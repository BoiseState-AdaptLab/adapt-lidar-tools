// File name: PlsToCsv.cpp
// Created on: Jan 2020
// Author: Jared White, Floriana Ciaglia, Nick Prussen

#include "LidarDriver.hpp"
#include <chrono>

// Activity level must be included before spdlog
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"


typedef std::chrono::high_resolution_clock Clock;

std::string getPeaksProperty(const std::vector<Peak*>& peaks, int productID);

bool writeLinesToFile(std::string filename, std::vector<std::string> lines) {
    std::ofstream file (filename);
    if (file.is_open()) {
        for(const auto& line: lines){
            file << line;
            file << '\n';
        }
    } else {
        //File could not be opened
        return false;
    }
    return true;
}

// Csv-maker driver
int main (int argc, char *argv[]) {

    // Setting up logger
    spdlog::set_pattern("[%^%=8l%$] %v");
        // Sets new pattern for timestamp
    FlightLineData rawData;    // Wave inputs from pls file
    LidarDriver driver;         // Driver object with tools
    csv_CmdLine cmdLineArgs;    // Command line options

    // Parse and validate the command line args
    if(!cmdLineArgs.parse_args(argc,argv)){
        return 1;
    }

    std::string fname = cmdLineArgs.getInputFileName(true);

    // Collect start time
    Clock::time_point t1 = Clock::now();

    if (cmdLineArgs.log_diagnostics) {
        spdlog::info("Diagnostics enabled for this run");
    }

    spdlog::info("Processing {}", fname);

    // Initialize data input per CmdLine specification
    
    // ingest the raw flight data into an object
    if (rawData.setFlightLineData(cmdLineArgs.getInputFileName(true))) {
        return 1;
    }

    auto peaks = driver.fit_data_csv(rawData, cmdLineArgs);
    for(int product : cmdLineArgs.selected_products){
        std::string property = getPeaksProperty(peaks, product);
        std::string fileName = cmdLineArgs.get_output_filename(product);
        if(!writeLinesToFile(fileName, {property})){
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

std::string getPeaksProperty(const std::vector<Peak*>& peaks, int productID){
    std::string results;
    for(const auto& peak: peaks){
        std::string temp;
        peak->to_string(temp,{productID});
        results+=temp;
        results+=",";           
    }
    
    //this statement deletes the extra comma
    //at the end of the file
    if(results.length() > 0){
        results.erase(results.end() -1);
    }

    return results;
}
