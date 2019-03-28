//
// Created by arezaii on 3/24/19.
//

#include "LidarDriver.hpp"

const double NO_DATA = -99999.99;
const double MAX_ELEV = 99999.99;

/**
 * setup the gdal dataset (file) with metadata
 * @param tiff_driver pointer to the GTiff driver
 * @param filename the filename for the output
 * @param band_desc the description for the band data
 * @param x_idx_extent the x extent value
 * @param y_idx_extent the y extent value
 * @return a pointer to a GDALDataset object with the provided metadata
 */

GDALDataset * LidarDriver::setup_gdal_ds(GDALDriver *tiff_driver, std::string filename, std::string band_desc, int
x_idx_extent,
                            int y_idx_extent){
	GDALDataset * gdal_ds;
	gdal_ds = tiff_driver->Create(filename.c_str(), x_idx_extent, y_idx_extent, 1, GDT_Float32, NULL);
	gdal_ds->GetRasterBand(1)->SetNoDataValue(NO_DATA);
	gdal_ds->GetRasterBand(1)->SetDescription(band_desc.c_str());
	return gdal_ds;
}

/**
 * creates FlightLineData object from the raw data in the input file
 * @param data the FlightLineData object to store the raw data in
 * @param inputFileName the file name and path of the input file
 */
void LidarDriver::setup_flight_data(FlightLineData &data, std::string inputFileName){
	data.setFlightLineData(inputFileName);
}

/**
 * fits the raw data using either gaussian or first difference fitting
 * @param raw_data reference to FlightLineData object that holds raw data
 * @param fitted_data reference to LidarVolume object to store fit data in
 * @param useGaussianFitting flag to indicate fitting type
 */
void LidarDriver::fit_data(FlightLineData &raw_data, LidarVolume &fitted_data, bool useGaussianFitting) {
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
void LidarDriver::setup_lidar_volume(FlightLineData &raw_data, LidarVolume &lidar_volume){
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
void LidarDriver::add_peaks_to_volume(LidarVolume &lidar_volume, std::vector<Peak> &peaks, int peak_count){
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
int LidarDriver::parse_pulse(PulseData &pulse, std::vector<Peak> &peaks, GaussianFitter &fitter, bool use_gaussian_fitting, int
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
 * write the fitted lidar volume data to the given GDAL dataset
 * @param fitted_data the populated lidar volume
 * @param gdal_ds pointer to a prepared dataset to populate
 * @param prod_id the id (type) of the product to generate
 */

void LidarDriver::produce_product(LidarVolume &fitted_data, GDALDataset *gdal_ds, int prod_id) {
	CPLErr retval;
	//indexes
	int x, y;
	//place to hold the elevations
	float *elevation = (float *) calloc(fitted_data.x_idx_extent, sizeof(float));

#ifdef DEBUG
	std::cerr << "Entering write image loop. In "<< __FILE__ << ":" << __LINE__ << std::endl;
#endif

	//loop through every pixel position
	for (y = fitted_data.y_idx_extent - 1; y >= 0; y--) {
		for (x = 0; x < fitted_data.x_idx_extent; x++) {
			//get the vector of found peaks at this pixel
			std::vector<Peak> *peaks = fitted_data.volume[fitted_data.position(y, x)];
			//decide what to do with the peak data at this pixel
			switch (prod_id) {
				case 1 : //max elev
					elevation[x] = get_z_activation_extreme(peaks, true);
					break;
				case 2 : //min elev
					elevation[x] = get_z_activation_extreme(peaks, false);
					break;
				case 3 : //max-min
					elevation[x] = get_z_activation_diff(peaks);
					break;
				default:
					//std::cout << "Product #" << prod_id << " not implemented" << std::endl;
					break;
			}
		}
#ifdef DEBUG
		std::cerr << "In writeImage loop. Writing band: "<< x << "," << y << ". In " << __FILE__ << ":" << __LINE__ << std::endl;
#endif
		//add the elevation data to the raster, one column at a time
		// Refer to http://www.gdal.org/classGDALRasterBand.html
		retval = gdal_ds->GetRasterBand(1)->RasterIO(GF_Write, 0, fitted_data.y_idx_extent - y - 1, fitted_data.x_idx_extent, 1, elevation,
		                                             fitted_data.x_idx_extent, 1, GDT_Float32, 0, 0, NULL);
		if (retval != CE_None) {
			std::cerr << "Error during writing band: 1 " << std::endl;
		}
	}

	free(elevation);

}

/**
 * setup the GDAL manager and get the GTiff driver
 * @return pointer to the GTiff driver
 */
GDALDriver * LidarDriver::setup_gdal_driver(){
	//bring up the driver for all GDAL interactions
	GDALAllRegister();
	return GetGDALDriverManager()->GetDriverByName("GTiff");
}


/**
 * set the orientation on the gdal file to align with the lidar volume data
 * @param fitted_data lidar volume with fitted data
 * @param gdal_ds the gdal dataset
 * @param geog_cs
 * @param utm
 */
void LidarDriver::geo_orient_gdal(LidarVolume &fitted_data, GDALDataset *gdal_ds, std::string geog_cs, int utm){
	//In a north up image, transform[1] is the pixel width, and transform[5] is
	//the pixel height. The upper left corner of the upper left pixel is at
	//position (transform[0],transform[3]).
	//  adfGeoTransform[0] /* top left x */
	//  adfGeoTransform[1] /* w-e pixel resolution */
	//  adfGeoTransform[2] /* 0 */
	//  adfGeoTransform[3] /* top left y */
	//  adfGeoTransform[4] /* 0 */
	//  adfGeoTransform[5] /* n-s pixel resolution (negative value) */
	double transform[6];
	transform[0] = fitted_data.bb_x_min;
	transform[1] = 1;
	transform[2] = 0;
	transform[3] = fitted_data.bb_y_max;
	transform[4] = 0;
	transform[5] = -1;	//Always -1

	OGRSpatialReference oSRS;
	char *pszSRS_WKT = NULL;
	gdal_ds->SetGeoTransform(transform);
	oSRS.SetUTM(utm, TRUE);
	oSRS.SetWellKnownGeogCS(geog_cs.c_str());
	oSRS.exportToWkt(&pszSRS_WKT);
	gdal_ds->SetProjection(pszSRS_WKT);
	CPLFree(pszSRS_WKT);
}

/**
 * get the difference between max and min z_activation data for a set of peaks
 * @param peaks the peaks to evaluate
 * @return the difference between the maximal and minimal element values, or NO_DATA if no peaks
 */
float LidarDriver::get_z_activation_diff(std::vector<Peak> *peaks){
	float max_z = NO_DATA;
	float min_z = MAX_ELEV;
	if(peaks==NULL || peaks->empty()){
		return NO_DATA;
	}
	for (std::vector<Peak>::iterator it = peaks->begin();
	     it != peaks->end(); ++it) {
		//check if max or min we want
		if ((float)it->z_activation > max_z) {
			max_z = (float) it->z_activation;
		}
		if ((float)it->z_activation < min_z) {
			min_z = (float) it->z_activation;
		}

	}

	return max_z - min_z;
}


/**
 * get the maximal or minimal peak from the vector
 * @param peaks set of peaks to evaluate
 * @param max_flag True = return maximum value, false = return minimum value
 * @return the smallest or largest value in the set, or NO_DATA if no peaks
 */
float LidarDriver::get_z_activation_extreme(std::vector<Peak> *peaks, bool max_flag){
	float max_z = NO_DATA;
	float min_z = MAX_ELEV;
	if(peaks==NULL || peaks->empty()){
		return NO_DATA;
	}
	for (std::vector<Peak>::iterator it = peaks->begin();
	     it != peaks->end(); ++it) {
		//check if max or min we want
		if (max_flag) {
			if ((float)it->z_activation > max_z) {
				max_z = (float) it->z_activation;
			}
		} else {
			if ((float)it->z_activation < min_z) {
				min_z = (float) it->z_activation;
			}
		}
	}
	return max_flag ? max_z : min_z;
}
