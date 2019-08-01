// File name: PlsToCsv.cpp
// Created on: March 2019
// Author: Ravi, Ahmad, Spencer

#include "LidarDriver.hpp"
#include "CsvWriter.hpp"
#include <chrono>

//#define DEBUG

typedef std::chrono::high_resolution_clock Clock;


// Csv-maker driver
int main (int argc, char *argv[]) {

    LidarDriver driver; //driver object with tools
    csv_CmdLine cmdLineArgs; //command line options
    FlightLineData rawData; //the raw data read from PLS + WVS files
    CsvWriter writer; //Writes CSV file
    writer.setLines(new std::vector<std::string*>);//Data stored here

    // Parse and validate the command line args
    if(!cmdLineArgs.parse_args(argc,argv)){
        return 1;
    }

    //Collect start time
    Clock::time_point t1 = Clock::now();

    std::cout << "\nProcessing  " << cmdLineArgs.getInputFileName(true).c_str()
        << std::endl;

    //ingest the raw flight data into an object
    driver.setup_flight_data(rawData, cmdLineArgs.getInputFileName(true));

    //fit data
    driver.fit_data_csv(rawData, *writer.getLines(), cmdLineArgs);

    //Write data to file
    writer.write_to_csv(cmdLineArgs.get_output_filename(1));

    //Free memory
    rawData.closeFlightLineData();

    writer.freeLines();

    //Get end time
    Clock::time_point t2 = Clock::now();

    //Compute total run time and convert to appropriate units
    double diff = std::chrono::duration_cast<std::chrono::seconds>(t2-t1).
        count();
    std::cout << "All done!\nTime elapsed: " << diff << " seconds\n" <<
        std::endl;

    return 0;
}