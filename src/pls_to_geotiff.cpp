// File name: PlsToGeotiff.cpp
// Created on: March 2019
// Author: Ravi, Ahmad, Spencer

#include "LidarDriver.hpp"
#include <chrono>

// Activity level must be defined before spdlog is included.
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"

typedef std::chrono::high_resolution_clock Clock;

// Lidar driver
int main (int argc, char *argv[]) {

    // Setting up logger
    spdlog::set_pattern("[%^%=8l%$] %v");
        // Sets new pattern for timestamp

    LidarDriver driver; //driver object with tools
    CmdLine cmdLineArgs; //command line options
    FlightLineData rawData; //the raw data read from PLS + WVS files
    LidarVolume intermediateData; //the parsed data from the LIDAR waveforms

    // Parse and validate the command line args
    if(!cmdLineArgs.parse_args(argc,argv)){
        return 1;
    }

    //Collect start time
    Clock::time_point t1 = Clock::now();

    spdlog::info("Processing {}", cmdLineArgs.getInputFileName(true));

    //ingest the raw flight data into an object
    if (rawData.setFlightLineData(cmdLineArgs.getInputFileName(true))) {
        return 1;
    }

    spdlog::debug("driver.setup_flight_data returned");

    //calculate size in memory of the tif products
    driver.calc_product_size(rawData, cmdLineArgs.selected_products.size());

    spdlog::debug("driver.calc_product_size_returned");

    //fit data
    driver.fit_data(rawData, intermediateData, cmdLineArgs);

    spdlog::debug("driver.fit_data returned");

    //Represents the output file format. This is used only to write data sets
    GDALDriver *driverTiff = driver.setup_gdal_driver();

    spdlog::debug("driver.fit_data returned, will loop through products next.");

    // TODO: None of this should be in main - it should be abstracted away
    //produce the product(s)
    for(const int& prod : cmdLineArgs.selected_products){
        std::cout << "Writing GeoTIFF "<< cmdLineArgs.get_product_desc(prod) 
            << std::endl;
        //represents the tiff file
        GDALDataset *gdal_ds;
        //Setup gdal dataset for this product
        gdal_ds = driver.setup_gdal_ds(driverTiff, 
                cmdLineArgs.get_output_filename(prod).c_str(),
                cmdLineArgs.get_product_desc(prod),
                intermediateData.x_idx_extent,
                intermediateData.y_idx_extent);

        //orient the tiff correctly
        driver.geo_orient_gdal(intermediateData,gdal_ds,
                rawData.geog_cs, rawData.utm);
        //write the tiff data
        driver.produce_product(intermediateData, gdal_ds,
                cmdLineArgs.get_calculation_code(prod),
                cmdLineArgs.get_peaks_code(prod),
                cmdLineArgs.get_variable_code(prod));

        //kill it with fire!
        GDALClose((GDALDatasetH) gdal_ds);
    }
    GDALDestroyDriverManager();
    intermediateData.deallocateMemory();
    rawData.closeFlightLineData();

    //Get end time
    Clock::time_point t2 = Clock::now();

    //Compute total run time and convert to appropriate units
    double diff = std::chrono::duration_cast<std::chrono::seconds>(t2-t1).
        count();
    spdlog::info("All done! Time elapsed: {} seconds", diff);

    return 0;
}
