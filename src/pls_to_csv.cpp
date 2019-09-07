// File name: PlsToCsv.cpp
// Created on: March 2019
// Author: Ravi, Ahmad, Spencer

#include "TxtWaveReader.hpp"
#include "LidarDriver.hpp"
#include "CsvWriter.hpp"
#include <chrono>

// Activity level must be included before spdlog
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"

typedef std::chrono::high_resolution_clock Clock;


// Csv-maker driver
int main (int argc, char *argv[]) {

    // Setting up logger
    spdlog::set_pattern("[%^%=8l%$] %v");
        // Sets new pattern for timestamp

    TxtWaveReader rawData0;     // Wave inputs from txt file
    FlightLineData rawData1;    // Wave inputs from pls file
    LidarDriver driver;         // Driver object with tools
    csv_CmdLine cmdLineArgs;    // Command line options
    CsvWriter writer;           // Writes CSV file
    writer.setLines(new std::vector<std::string*>); //Data stored here

    // Parse and validate the command line args
    if(!cmdLineArgs.parse_args(argc,argv)){
        return 1;
    }

    bool is_txt = cmdLineArgs.is_txt;
    std::string fname = cmdLineArgs.getInputFileName(true);

    // Collect start time
    Clock::time_point t1 = Clock::now();

    spdlog::info("Processing {}", fname);

    // Initialize data input per CmdLine specification
    if (is_txt) {
        spdlog::info("txt file recognized");
        if (rawData0.open_file(fname.c_str())) {
            spdlog::critical("Opening txt file '{}' failed", fname);
            return 1;
        }

        driver.fit_data_csv(rawData0, *writer.getLines(), cmdLineArgs);
    } else {
        // ingest the raw flight data into an object
        if (rawData1.setFlightLineData(cmdLineArgs.getInputFileName(true))) {
            return 1;
        }

        driver.fit_data_csv(rawData1, *writer.getLines(), cmdLineArgs);
    }

    // Write data to file
    writer.write_to_csv(cmdLineArgs.get_output_filename(1));

    // Free memory
    if (!is_txt) {
        rawData1.closeFlightLineData();
    }

    writer.freeLines();

    // Get end time
    Clock::time_point t2 = Clock::now();

    // Compute total run time and convert to appropriate units
    double diff = std::chrono::duration_cast<std::chrono::seconds>(t2-t1).
        count();
    spdlog::info("All done!\nTime elapsed: {} seconds", diff);

    return 0;
}
