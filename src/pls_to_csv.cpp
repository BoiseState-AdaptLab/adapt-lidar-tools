// File name: PlsToCsv.cpp
// Created on: March 2019
// Author: Ravi, Ahmad, Spencer

#include "LidarDriver.hpp"
#include "CsvWriter.hpp"
#include <chrono>

#include "spdlog/spdlog.h"

typedef std::chrono::high_resolution_clock Clock;


// Csv-maker driver
int main (int argc, char *argv[]) {

    LidarDriver driver;          //driver object with tools
    csv_CmdLine cmdLineArgs;     //command line options
    FlightLineData rawData;      //the raw data read from PLS + WVS files
    CsvWriter writer;            //Writes CSV file
    writer.setLines(new std::vector<std::string*>); //Data stored here

    // Parse and validate the command line args
    if(!cmdLineArgs.parse_args(argc,argv)){
        return 1;
    }

    // Initialize data input per CmdLine specification
    if (cmdLineArgs.is_txt) {
        spdlog::info("txt file recognized");
        return 0;
    }

    // Collect start time
    Clock::time_point t1 = Clock::now();

    spdlog::info("Processing {}", cmdLineArgs.getInputFileName(true).c_str());

    // ingest the raw flight data into an object
    if (rawData.setFlightLineData(cmdLineArgs.getInputFileName(true))) {
        return 1;
    }

    // fit data
    driver.fit_data_csv(rawData, *writer.getLines(), cmdLineArgs);

    // Write data to file
    writer.write_to_csv(cmdLineArgs.get_output_filename(1));

    // Free memory
    rawData.closeFlightLineData();

    writer.freeLines();

    // Get end time
    Clock::time_point t2 = Clock::now();

    // Compute total run time and convert to appropriate units
    double diff = std::chrono::duration_cast<std::chrono::seconds>(t2-t1).
        count();
    spdlog::info("All done!\nTime elapsed: {} seconds", diff);

    return 0;
}
