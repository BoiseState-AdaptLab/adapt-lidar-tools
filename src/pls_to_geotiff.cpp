// File name: PlsToGeotiff.cpp
// Created on: March 2019
// Author: Ravi, Ahmad, Spencer

#include "LidarDriver.hpp"
#include "GTypes.h"
#include "P7_Client.h"
#include "P7_Trace.h"
#include <chrono>

/**
 * Closes loggers
 */
#define closeP7 \
    if (l_pTrace) {                     \
        l_pTrace->Unregister_Thread(0); \
        l_pTrace->Release();            \
        l_pTrace = NULL;                \
    }                                   \
                                        \
    if (l_pClient) {                    \
        l_pClient->Release();           \
        l_pClient = NULL;               \
    }

typedef std::chrono::high_resolution_clock Clock;

// Pls to Geotiff Driver
int main (int argc, char *argv[]) {

    //==========================================================================
    // Parse and validate command line args
    //==========================================================================

    CmdLine cmdLineArgs; // Command line object

    if (!cmdLineArgs.parse_args(argc, argv)) {
        return 1;
    }

    //==========================================================================
    // Set up logger
    //==========================================================================

    std::string client_options;

    IP7_Client *l_pClient = NULL;        // Main P7 manager object
    IP7_Trace *l_pTrace = NULL;          // A 'channel' to send logs through
    IP7_Trace::hModule l_hModule = NULL;

    // Setup crash handler, it allows P7 to flush to log output before we crash
    P7_Set_Crash_Handler();

    if (cmdLineArgs.useLog) {
        client_options = "/P7.Sink=FileTxt /P7.Dir=";
        client_options += cmdLineArgs.logDir;
    } else {
        client_options = "/P7.Sink=Console ";
    }

    client_options += "/P7.Trc.Verb=";
    client_options += cmdLineArgs.verbosity;

    l_pClient = P7_Create_Client(TM(client_options.c_str()));

    if (l_pClient == NULL) {
        closeP7
        return 1;
    }

    l_pTrace = P7_Create_Trace(l_pClient, TM("p7_trace"));

    if (l_pTrace == NULL) {
        closeP7
        return 1;
    }

    //Threads and modules must be registered.
    if (!l_pTrace->Register_Thread(TM("APP"), 0)) {
        closeP7
        return 1;
    }

    l_pTrace->Register_Module(TM("Main"), &l_hModule);

    //Share the tracer so other parts of the program can log with it.
    if (!l_pTrace->Share(TM("p7_trace"))) {
        closeP7
        return 1;
    }

    //=========================================================================
    // Run Pls to Geotiff Driver
    //=========================================================================

    LidarDriver driver; //driver object with tools
    FlightLineData rawData; //the raw data read from PLS + WVS files
    LidarVolume intermediateData; //the parsed data from the LIDAR waveforms

    //Collect start time
    Clock::time_point t1 = Clock::now();

    l_pTrace->P7_INFO(l_hModule, TM("Processing %s"),
            cmdLineArgs.getInputFileName(true).c_str());


    //ingest the raw flight data into an object
    driver.setup_flight_data(rawData, cmdLineArgs.getInputFileName(true));

    l_pTrace->P7_DEBUG(l_hModule, TM("driver.setup_flight_data returned"));


    //fit data
    driver.fit_data(rawData, intermediateData, cmdLineArgs);

    l_pTrace->P7_DEBUG(l_hModule, TM("driver.fit_data returned"));


    //Represents the output file format. This is used only to write data sets
    GDALDriver *driverTiff = driver.setup_gdal_driver();

    l_pTrace->P7_DEBUG(l_hModule,
            TM("driver.fit_data returned, will loop through products next."));


    // TODO: None of this should be in main - it should be abstracted away
    //produce the product(s)
    for(const int& prod : cmdLineArgs.selected_products){
        l_pTrace->P7_INFO(l_hModule, TM("Writing GeoTIFF %s"),
                cmdLineArgs.get_product_desc(prod).c_str());

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
        driver.produce_product(intermediateData, gdal_ds, prod);

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
    l_pTrace->P7_INFO(l_hModule, TM("All done! Time elapsed: %f seconds"),
            diff);


    closeP7

    return 0;
}
