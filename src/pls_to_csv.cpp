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

void* threaded_fit_data_csv(void* args);

typedef std::chrono::high_resolution_clock Clock;


// Csv-maker driver
int main (int argc, char *argv[]) {

    // Setting up logger
    spdlog::set_pattern("[%^%=8l%$] %v");
        // Sets new pattern for timestamp
    int numThreads; 		// Number of threads to run fit_data_csv
    TxtWaveReader rawData0;     // Wave inputs from txt file
    FlightLineData rawData1;    // Wave inputs from pls file
    LidarDriver driver;         // Driver object with tools
    csv_CmdLine cmdLineArgs;    // Command line options
    CsvWriter writer;           // Writes CSV file
    writer.setLines(new std::vector<std::string*>); //Data stored here
    numThreads = 1;

    // Parse and validate the command line args
    if(!cmdLineArgs.parse_args(argc,argv)){
        return 1;
    }

    bool is_txt = cmdLineArgs.is_txt;
    std::string fname = cmdLineArgs.getInputFileName(true);

    // Collect start time
    Clock::time_point t1 = Clock::now();

    if (cmdLineArgs.log_diagnostics) {
        spdlog::info("Diagnostics enabled for this run");
    }

    spdlog::info("Processing {}", fname);

    // Initialize data input per CmdLine specification
    if (is_txt) {
        spdlog::debug("txt file recognized");
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
	if(numThreads <= 1) {
	    driver.fit_data_csv(rawData1, *writer.getLines(), cmdLineArgs);
	} else {
	    pthread_t t_ids[numThreads];
	    struct thread_args args[numThreads];
	    for (int i=0; i < numThreads; i++) {
	        args[i] = thread_args{&driver, &rawData1, *writer.getLines(), cmdLineArgs};
	        pthread_create(&t_ids[i], NULL, threaded_fit_data_csv, (void *)&args[i]);
	    }

            for (int i=0; i < numThreads; i++) {
		pthread_join(t_ids[i], NULL);
            }
	}
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

void* threaded_fit_data_csv(void* args) {
	thread_args *t_args = (thread_args *)args;
	LidarDriver *driver = t_args->threadDriver;
	FlightLineData *raw_data = t_args->raw_data;
	std::vector<std::string*> strings = t_args->strings;
	csv_CmdLine cmdLine = t_args->cmdLine;
	driver->fit_data_csv(*raw_data, strings, cmdLine);
	return 0;
}
