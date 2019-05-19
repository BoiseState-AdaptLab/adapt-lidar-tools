//
// Created by arezaii on 3/24/19.
//

#include "LidarDriver.hpp"

#define DEBUG //Comment out to disable

/**
 * setup the gdal dataset (file) with metadata
 * @param tiff_driver pointer to the GTiff driver
 * @param filename the filename for the output
 * @param band_desc the description for the band data
 * @param x_idx_extent the x extent value
 * @param y_idx_extent the y extent value
 * @return a pointer to a GDALDataset object with the provided metadata
 */

GDALDataset* LidarDriver::setup_gdal_ds(GDALDriver *tiff_driver,
                                        std::string filename,
                                        std::string band_desc,
                                        int x_idx_extent, int y_idx_extent)
{
  GDALDataset * gdal_ds;
  gdal_ds = tiff_driver->Create(filename.c_str(), x_idx_extent, y_idx_extent,
                                1, GDT_Float32, NULL);
  gdal_ds->GetRasterBand(1)->SetNoDataValue(NO_DATA);
  gdal_ds->GetRasterBand(1)->SetDescription(band_desc.c_str());

  return gdal_ds;
}

/**
 * creates FlightLineData object from the raw data in the input file
 * @param data the FlightLineData object to store the raw data in
 * @param inputFileName the file name and path of the input file
 */
void LidarDriver::setup_flight_data(FlightLineData &data,
                                    std::string inputFileName)
{
#ifdef DEBUG
    std::cerr << "data.setFlightLineData about to be called..." << std::endl;
#endif

  data.setFlightLineData(inputFileName);

#ifdef DEBUG
    std::cerr << "data.setFlightLineData returned" << std::endl;
#endif
}

/**
 * fits the raw data using either gaussian or first difference fitting
 * @param raw_data reference to FlightLineData object that holds raw data
 * @param fitted_data reference to LidarVolume object to store fit data in
 * @param useGaussianFitting flag to indicate fitting type
 */
void LidarDriver::fit_data(FlightLineData &raw_data, LidarVolume &fitted_data,
                           bool useGaussianFitting) 
{
  PulseData pd;
  std::ostringstream stream;
  GaussianFitter fitter;
  std::vector<Peak> peaks;
  int peak_count = 0;

#ifdef DEBUG
std::cerr << "Start finding peaks. In " << __FILE__ << ":" << __LINE__ 
          << std::endl;
#endif

  //setup the lidar volume bounding and allocate memory
  setup_lidar_volume(raw_data, fitted_data);

  //message the user
  std::string fit_type=useGaussianFitting?"gaussian fitting":"first difference";
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
	peaks.clear();

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
 * @return -1 if the pulse was empty, otherwise the peak count
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

	return peak_count;
}

/**
 * write the fitted lidar volume data to the given GDAL dataset
 * @param fitted_data the populated lidar volume
 * @param gdal_ds pointer to a prepared dataset to populate
 * @param prod_id the id (product type) of the product to generate
 */
void LidarDriver::produce_product(LidarVolume &fitted_data, GDALDataset *gdal_ds, int prod_id) {
	CPLErr retval;
	//indexes
	int x, y;
	//place to hold the elevations
	float *pixel_values = (float *) calloc(fitted_data.x_idx_extent, sizeof(float));
	float avg = 0 ;
	float dev = 0;
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
				case 1 : //max all elevation
					pixel_values[x] = get_extreme(peaks, true,2,'z');
					break;
				case 2 : //min all elevation
					pixel_values[x] = get_extreme(peaks, false,2,'z');
					break;
				case 3 : //mean all elevation
					pixel_values[x] = get_mean(peaks,2,'z');
					break;
				case 4: //std-dev all elevation
					avg = get_mean(peaks, 2,'z');
					pixel_values[x] = get_deviation(peaks, avg, 2, 'z');
					break;
				case 5: //skewness all elevation
					avg = get_mean(peaks, 2,'z');
					dev = get_deviation(peaks, avg, 2, 'z');
					pixel_values[x] = get_skewtosis(peaks, avg,dev, 2, 'z',3);
					break;
				case 6: //kurtosis all elevation
					avg = get_mean(peaks, 2,'z');
					dev = get_deviation(peaks, avg, 2, 'z');
					pixel_values[x] = get_skewtosis(peaks, avg,dev, 2, 'z',4);
					break;
				case 7: //max first elevation
					pixel_values[x] = get_extreme(peaks, true,0,'z');
					break;
				case 8: //min first elevation
					pixel_values[x] = get_extreme(peaks, false,0,'z');
					break;
				case 9 : //mean first elevation
					pixel_values[x] = get_mean(peaks,0,'z');
					break;
				case 10: //std-dev first elevation
					avg = get_mean(peaks, 0,'z');
					pixel_values[x] = get_deviation(peaks, avg, 0, 'z');
					break;
				case 11: //skewness first elevation
					avg = get_mean(peaks, 0,'z');
					dev = get_deviation(peaks, avg, 0, 'z');
					pixel_values[x] = get_skewtosis(peaks, avg,dev, 0, 'z',3);
					break;
				case 12: //kurtosis first elevation
					avg = get_mean(peaks, 0,'z');
					dev = get_deviation(peaks, avg, 0, 'z');
					pixel_values[x] = get_skewtosis(peaks, avg,dev, 0, 'z',4);
					break;
				case 13: //max last elevation
					pixel_values[x] = get_extreme(peaks, true,1,'z');
					break;
				case 14: //min last elevation
					pixel_values[x] = get_extreme(peaks, false,1,'z');
					break;
				case 15: //mean last elevation
					pixel_values[x] = get_mean(peaks,1,'z');
					break;
				case 16: //std-dev last elevation
					avg = get_mean(peaks, 1,'z');
					pixel_values[x] = get_deviation(peaks, avg, 1, 'z');
					break;
				case 17: //skewness last elevation
					avg = get_mean(peaks, 1,'z');
					dev = get_deviation(peaks, avg, 1, 'z');
					pixel_values[x] = get_skewtosis(peaks, avg,dev, 1, 'z',3);
					break;
				case 18: //kurtosis last elevation
					avg = get_mean(peaks, 1,'z');
					dev = get_deviation(peaks, avg, 1, 'z');
					pixel_values[x] = get_skewtosis(peaks, avg,dev, 1, 'z',4);
					break;
				case 19 : //max all amplitude
					pixel_values[x] = get_extreme(peaks, true, 2, 'a');
					break;
				case 20 : //min all amplitude
					pixel_values[x] = get_extreme(peaks, false, 2,'a');
					break;
				case 21 : //mean all amplitude
					pixel_values[x] = get_mean(peaks,2,'a');
					break;
				case 22: //std-dev all amplitude
					avg = get_mean(peaks, 2,'a');
					pixel_values[x] = get_deviation(peaks, avg, 2, 'a');
					break;
				case 23: //skewness all amplitude
					avg = get_mean(peaks, 2,'a');
					dev = get_deviation(peaks, avg, 2, 'a');
					pixel_values[x] = get_skewtosis(peaks, avg,dev, 2, 'a',3);
					break;
				case 24: //kurtosis all amplitude
					avg = get_mean(peaks, 2,'a');
					dev = get_deviation(peaks, avg, 2, 'a');
					pixel_values[x] = get_skewtosis(peaks, avg,dev, 2, 'a',4);
					break;
				case 25: //max first amplitude
					pixel_values[x] = get_extreme(peaks,true,0,'a');
					break;
				case 26: //min first amplitude
					pixel_values[x] = get_extreme(peaks,false,0,'a');
					break;
				case 27: //mean first amplitude
					pixel_values[x] = get_mean(peaks,0,'a');
					break;
				case 28: //std-dev first amplitude
					avg = get_mean(peaks, 0,'a');
					pixel_values[x] = get_deviation(peaks, avg, 0, 'a');
					break;
				case 29: //skewness first amplitude
					avg = get_mean(peaks, 0,'a');
					dev = get_deviation(peaks, avg, 0, 'a');
					pixel_values[x] = get_skewtosis(peaks, avg,dev, 0, 'a',3);
					break;
				case 30: //kurtosis first amplitude
					avg = get_mean(peaks, 0,'a');
					dev = get_deviation(peaks, avg, 0, 'a');
					pixel_values[x] = get_skewtosis(peaks, avg,dev, 0, 'a',4);
					break;
				case 31: //max last amplitude
					pixel_values[x] = get_extreme(peaks,true,1,'a');
					break;
				case 32: //min last amplitude
					pixel_values[x] = get_extreme(peaks,false,1,'a');
					break;
				case 33: //mean last amplitude
					pixel_values[x] = get_mean(peaks,1,'a');
					break;
				case 34: //std-dev last amplitude
					avg = get_mean(peaks, 1,'a');
					pixel_values[x] = get_deviation(peaks, avg, 1, 'a');
					break;
				case 35: //skewness last amplitude
					avg = get_mean(peaks, 1,'a');
					dev = get_deviation(peaks, avg, 1, 'a');
					pixel_values[x] = get_skewtosis(peaks, avg,dev, 1, 'a',3);
					break;
				case 36: //kurtosis last amplitude
					avg = get_mean(peaks, 1,'a');
					dev = get_deviation(peaks, avg, 1, 'a');
					pixel_values[x] = get_skewtosis(peaks, avg,dev, 1, 'a',4);
					break;
				case 37 : //max all width
					pixel_values[x] = get_extreme(peaks, true,2,'w');
					break;
				case 38 : //min all width
					pixel_values[x] = get_extreme(peaks, false,2,'w');
					break;
				case 39: //mean all width
					pixel_values[x] = get_mean(peaks,2,'w');
					break;
				case 40: //std-dev all width
					avg = get_mean(peaks, 2,'w');
					pixel_values[x] = get_deviation(peaks, avg, 2, 'w');
					break;
				case 41: //skewness all width
					avg = get_mean(peaks, 2,'w');
					dev = get_deviation(peaks, avg, 2, 'w');
					pixel_values[x] = get_skewtosis(peaks, avg,dev, 2, 'w',3);
					break;
				case 42: //kurtosis all width
					avg = get_mean(peaks, 2,'w');
					dev = get_deviation(peaks, avg, 2, 'w');
					pixel_values[x] = get_skewtosis(peaks, avg,dev, 2, 'w',4);
					break;
				case 43: //max first width
					pixel_values[x] = get_extreme(peaks, true,0,'w');
					break;
				case 44: //min first width
					pixel_values[x] = get_extreme(peaks, false,0,'w');
					break;
				case 45 : //mean first width
					pixel_values[x] = get_mean(peaks,0,'w');
					break;
				case 46: //std-dev first width
					avg = get_mean(peaks, 0,'w');
					pixel_values[x] = get_deviation(peaks, avg, 0, 'w');
					break;
				case 47: //skewness first width
					avg = get_mean(peaks, 0,'w');
					dev = get_deviation(peaks, avg, 0, 'w');
					pixel_values[x] = get_skewtosis(peaks, avg,dev, 0, 'w',3);
					break;
				case 48: //kurtosis first width
					avg = get_mean(peaks, 0,'w');
					dev = get_deviation(peaks, avg, 0, 'w');
					pixel_values[x] = get_skewtosis(peaks, avg,dev, 0, 'w',4);
					break;
				case 49: //max last width
					pixel_values[x] = get_extreme(peaks, true,1,'w');
					break;
				case 50: //min last width
					pixel_values[x] = get_extreme(peaks, false,1,'w');
					break;
				case 51: //mean last width
					pixel_values[x] = get_mean(peaks,1,'w');
					break;
				case 52: //std-dev last width
					avg = get_mean(peaks, 1,'w');
					pixel_values[x] = get_deviation(peaks, avg, 1, 'w');
					break;
				case 53: //skewness last width
					avg = get_mean(peaks, 1,'w');
					dev = get_deviation(peaks, avg, 1, 'w');
					pixel_values[x] = get_skewtosis(peaks, avg,dev, 1, 'w',3);
					break;
				case 54: //kurtosis last width
					avg = get_mean(peaks, 1,'w');
					dev = get_deviation(peaks, avg, 1, 'w');
					pixel_values[x] = get_skewtosis(peaks, avg,dev, 1, 'w',4);
					break;
				default:
					//std::cout << "Product #" << prod_id << " not implemented" << std::endl;
					break;
			}
		}
#ifdef DEBUG
		std::cerr << "In writeImage loop. Writing band: "<< x << "," << y << ". In " << __FILE__ << ":" << __LINE__ << std::endl;
#endif
		//add the pixel values to the raster, one column at a time
		// Refer to http://www.gdal.org/classGDALRasterBand.html
		retval = gdal_ds->GetRasterBand(1)->RasterIO(GF_Write, 0, fitted_data.y_idx_extent - y - 1, fitted_data.x_idx_extent, 1, pixel_values,
		                                             fitted_data.x_idx_extent, 1, GDT_Float32, 0, 0, NULL);
		if (retval != CE_None) {
			std::cerr << "Error during writing band: 1 " << std::endl;
		}
	}

	free(pixel_values);

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
 * @param geog_cs the geog_cs string (Datum value WGS 84, etc)
 * @param utm the utm string (NAD 11, etc)
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
 * get the difference between max and min peak property data for a set of peaks in one pass
 * @param peaks the peaks to evaluate
 * @param peak_property the property of the peaks to compare
 * @return the difference between the maximal and minimal element values, or NO_DATA if no peaks
 */
float LidarDriver::get_extreme_diff(std::vector<Peak> *peaks, char peak_property){
	float max_val = NO_DATA;
	float min_val = MAX_ELEV;
	float cur_val = 0;
	if(peaks==NULL || peaks->empty()){
		return NO_DATA;
	}
	for (std::vector<Peak>::iterator it = peaks->begin();
	     it != peaks->end(); ++it) {
		cur_val = get_peak_property(&*it,peak_property);

		if (cur_val > max_val) {
			max_val = cur_val;
		}
		if (cur_val < min_val) {
			min_val = cur_val;
		}

	}

	return max_val - min_val;
}


/**
 * get the extreme (max/min) value from a set of peaks, specify the property and peak position (first, last, all)
 * @param peaks the set of peaks to process
 * @param max_flag flag to indicate max (true = max, false = min)
 * @param peak_pos specify if first, last, or all peaks should be included in calculation (0=first, 1=last, 2=all)
 * @param peak_property the property of the peak to analyze (amplitude, width, z-activation, etc..)
 * @return the extreme property value of the set of peaks with the specified filter
 */
float LidarDriver::get_extreme(std::vector<Peak> *peaks, bool max_flag, int peak_pos, char peak_property){
	float max_val = NO_DATA;
	float min_val = MAX_ELEV;
	float cur_val = 0;
	bool no_countable_peaks = true;
	if(peaks==NULL || peaks->empty()){
		return NO_DATA;
	}
	for (std::vector<Peak>::iterator it = peaks->begin();
	     it != peaks->end(); ++it) {
		//check what type of returns to evaluate
		switch (peak_pos){
			case 0: //first
				if(it->position_in_wave!=1){
					continue;
				}
				no_countable_peaks = false;
				break;
			case 1: //last
				if(!it->is_final_peak){
					continue;
				}
				no_countable_peaks = false;
				break;
			case 2: //all
				no_countable_peaks = false;
				break;
			default:
				break;
		}
		//get current value to evaluate
		cur_val = get_peak_property(&*it,peak_property);
		//check if max or min we want
		if (max_flag) {
			if (cur_val > max_val) {
				max_val = cur_val;
			}
		} else {
			if (cur_val < min_val) {
				min_val = cur_val;
			}
		}
	}
	if(no_countable_peaks){
		return NO_DATA;
	}else{
		return max_flag ? max_val : min_val;
	}

}

/**
 * get the average (mean) value from a set of peaks, specify the property and peak position (first, last, all)
 * @param peaks the set of peaks to process
 * @param peak_pos specify if first, last, or all peaks should be included in calculation (0=first, 1=last, 2=all)
 * @param peak_property the property of the peak to analyze (amplitude, width, z-activation, etc..)
 * @return the mean property value of the set of peaks with the specified filter
 */
float LidarDriver::get_mean(std::vector<Peak> *peaks, int peak_pos, char peak_property){
	double val_sum  = 0;
	int val_count = 0;
	bool no_countable_peaks = true;
	if(peaks==NULL || peaks->empty()){
		return NO_DATA;
	}
	for (std::vector<Peak>::iterator it = peaks->begin();
	     it != peaks->end(); ++it) {
		//check what type of returns to evaluate
		switch (peak_pos){
			case 0: //first
				if(it->position_in_wave!=1){
					continue;
				}
				no_countable_peaks = false;
				break;
			case 1: //last
				if(!it->is_final_peak){
					continue;
				}
				no_countable_peaks = false;
				break;
			case 2: //all
				no_countable_peaks = false;
				break;
			default:
				break;
		}
		val_sum += get_peak_property(&*it,peak_property);
		val_count ++;
	}
	if(no_countable_peaks){
		return NO_DATA;
	}else{
		return val_sum/val_count;
	}

}

/**
 * get the property value from a peak, specify the property
 * @param peak the peak to extract data from
 * @param peak_property the property of the peak to analyze (amplitude, width, z-activation, etc..)
 * @return the property value of the peak
 */
float LidarDriver::get_peak_property(Peak *peak, char peak_property){
	switch (peak_property){
		case 'z':
			return peak->z_activation;
		case 'a':
			return peak->amp;
		case 'w':
			return peak->fwhm;
		default:
			break;
	}
	std::cout << "CRITICAL ERROR! \
                  No implemented peak property for identifier "<<peak_property<<"\n";
	exit(EXIT_FAILURE);
}

/**
 * get the standard deviation value from a set of peaks, specify the property and peak position (first, last, all)
 * @param peaks the set of peaks to process
 * @param avg the average (mean) of the set
 * @param peak_pos specify if first, last, or all peaks should be included in calculation (0=first, 1=last, 2=all)
 * @param peak_property the property of the peak to analyze (amplitude, width, z-activation, etc..)
 * @return the standard deviation of the property value of the set of peaks with the specified filter
 */
double LidarDriver::get_deviation(std::vector<Peak> *peaks, double avg, int peak_pos, char peak_property)
{
	double E=0;
	float cur_val=0;
	int peak_count = 0;
	if(peaks==NULL || peaks->empty()){
		return NO_DATA;
	}
	for (std::vector<Peak>::iterator it = peaks->begin();
	     it != peaks->end(); ++it) {
		cur_val = get_peak_property(&*it,peak_property);
		switch(peak_pos) {
			case 0: //first
				if (it->position_in_wave == 1) {
					peak_count++;
					E += pow(static_cast<double>(cur_val) - avg, 2);
				}
				break;
			case 1: //last
				if (it->is_final_peak) {
					peak_count++;
					E += pow(static_cast<double>(cur_val) - avg, 2);
				}
				break;
			case 2: //all
				peak_count++;
				E += pow(static_cast<double>(cur_val) - avg, 2);
		}
	}
	double inverse = 1.0 / static_cast<double>(peak_count);
	return sqrt(inverse * E);
}

/**
 * get the skewness or kurtosis (3rd/4th degree) value from a set of peaks, specify the property and peak position
 * (first,last, all)
 * @param peaks the set of peaks to process
 * @param avg the average (mean) of the set
 * @param dev the standard deviation of the set
 * @param peak_pos specify if first, last, or all peaks should be included in calculation (0=first, 1=last, 2=all)
 * @param peak_property the property of the peak to analyze (amplitude, width, z-activation, etc..)
 * @param power the power of the quadratic in the calculation (3=skewness, 4=kurtosis)
 * @return the skewness or kurtosis of the property value of the set of peaks with the specified filter
 */
double LidarDriver::get_skewtosis(std::vector<Peak> *peaks, double avg, double dev, int peak_pos, char
peak_property, int power){
	double G=0;
	double cur_val = 0;
	int peak_count = 0;
	if(peaks==NULL || peaks->empty()){
		return NO_DATA;
	}
	for (std::vector<Peak>::iterator it = peaks->begin();
	     it != peaks->end(); ++it) {
		cur_val = get_peak_property(&*it,peak_property);
		switch (peak_pos){
			case 0: //first peaks
				if (it->position_in_wave == 1) {
					peak_count++;
					G += pow(static_cast<double>(cur_val) - avg, power);
				}
				break;
			case 1: //last peaks
				if (it->is_final_peak) {
					peak_count++;
					G += pow(static_cast<double>(cur_val) - avg, power);
				}
				break;
			case 2: //all peaks
				G += pow(static_cast<double>(cur_val) - avg, power);
				peak_count ++;
				break;
			default:
				break;
		}

	}
	double inverse = 1.0 / static_cast<double>(peak_count);
	return (inverse * G) / pow(dev,power) ;
}
