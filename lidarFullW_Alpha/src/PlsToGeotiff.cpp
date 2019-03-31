// File name: lidarDriver.cpp
// Created on: 17-May-2017
// Author: ravi

#include "LidarDriver.hpp"
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;


// Lidar driver
int main (int argc, char *argv[]) {

	LidarDriver driver;

    CmdLine cmdLineArgs;
	FlightLineData rawData;
	LidarVolume intermediateData;

  // Parse and validate the command line args
    if(!cmdLineArgs.parse_args(argc,argv)){
        exit(1);
    }

  //Collect start time
  Clock::time_point t1 = Clock::now();

  std::cout << "\nProcessing  " << cmdLineArgs.getInputFileName().c_str() << std::endl;

  //ingest the raw flight data into an object
  driver.setup_flight_data(rawData, cmdLineArgs.getInputFileName());

  //fit data
  driver.fit_data(rawData,intermediateData, cmdLineArgs.useGaussianFitting);

  //Represents the output file format. This is used only to write data sets
  GDALDriver *driverTiff = driver.setup_gdal_driver();

  //produce the product(s)
  for(const int& prod : cmdLineArgs.selected_products){
	  std::cout << "Writing GeoTIFF "<< cmdLineArgs.get_product_desc(prod) << std::endl;
	  //represents the tiff file
	  GDALDataset *gdal_ds;
	  //Setup gdal dataset for this product
	  gdal_ds = driver.setup_gdal_ds(driverTiff, cmdLineArgs.get_output_filename(prod).c_str(),cmdLineArgs
			  .get_product_desc(prod), intermediateData.x_idx_extent,intermediateData.y_idx_extent);

	  //orient the tiff correctly
	  driver.geo_orient_gdal(intermediateData,gdal_ds, rawData.geog_cs, rawData.utm);

	  //write the tiff data
	  driver.produce_product(intermediateData, gdal_ds, prod);

	  //kill it with fire!
	  GDALClose((GDALDatasetH) gdal_ds);
  }

  GDALDestroyDriverManager();


  //Get end time
  Clock::time_point t2 = Clock::now();

  //Compute total run time and convert to appropriate units
  double diff = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
  std::cout << "All done!\nTime elapsed: " << diff << " seconds\n" << std::endl;

  return 0;
}
