//
// Created by arezaii on 3/24/19.
//
#include "LidarDriver.hpp"
#include "spdlog/spdlog.h"

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
    spdlog::debug("data.setFlightLineData about to be called...");

    data.setFlightLineData(inputFileName);

    spdlog::debug("data.setFlightLineData returned");
}

/**
 * fits the raw data using either gaussian or first difference fitting
 * @param raw_data reference to FlightLineData object that holds raw data
 * @param fitted_data reference to LidarVolume object to store fit data in
 * @param useGaussianFitting flag to indicate fitting type
 */
void LidarDriver::fit_data(FlightLineData &raw_data, LidarVolume &fitted_data,
        CmdLine &cmdLine) 
{
    PulseData pd;
    std::ostringstream stream;
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    int peak_count = 0;

    spdlog::debug("Start finding peaks. In {}:{}", __FILE__, __LINE__);

    //setup the lidar volume bounding and allocate memory
    setup_lidar_volume(raw_data, fitted_data);

    //message the user
    std::string fit_type=cmdLine.useGaussianFitting?"gaussian fitting":
        "first difference";
    spdlog::info("Finding peaks with {}", fit_type);

    //parse each pulse
    while (raw_data.hasNextPulse()) {
        // make sure that we have an empty vector
        peaks.clear();

        // gets the raw data from the file
        raw_data.getNextPulse(&pd);
       
        //Skip all the empty returning waveforms
        if (pd.returningIdx.empty()){
            continue;
        }
        try {
            // Smooth the data and test result
            fitter.smoothing_expt(&pd.returningWave);
           
            // Check parameter for using gaussian fitting or first differencing
            if (cmdLine.useGaussianFitting) {
                peak_count = fitter.find_peaks(&peaks, pd.returningWave,
                                     pd.returningIdx, 200);
            } else {
                peak_count = fitter.guess_peaks(&peaks, pd.returningWave,
                                     pd.returningIdx);
            }
 
            // for each peak - find the activation point
            //               - calculate x,y,z
            peak_count = raw_data.calc_xyz_activation(&peaks);
           
            // Calculate all requested information - Backscatter Coefficient
            // - Energy at % Height  - Height at % Energy
            peak_calculations(pd, peaks, fitter, cmdLine,
                       raw_data.current_wave_gps_info);

            add_peaks_to_volume(fitted_data, peaks, peak_count);
        } catch (const char *msg) {
            std::cerr << msg << std::endl;
        }
    }
    peaks.clear();

    spdlog::debug("Total: {}", fitter.get_total());
    spdlog::debug("Pass: {}", fitter.get_pass());
    spdlog::debug("Fail: {}", fitter.get_fail());
}

/**
 * Fits the raw data using either gaussian or first difference fitting,
 * and sends each wave of peaks to be written to a CSV file.
 * @param raw_data reference to FlightLineData object that holds raw data
 * @param strings a place where peak to_string calls will be stored
 * @param csv_CmdLine object that knows what data we want from peaks
 */
void LidarDriver::fit_data_csv(FlightLineData &raw_data,
        std::vector<std::string*> &strings, csv_CmdLine &cmdLine) 
{
    PulseData pd;
    std::ostringstream stream;
    GaussianFitter fitter;
    std::vector<Peak*> peaks;

    //message the user
    std::string fit_type=cmdLine.useGaussianFitting?"gaussian fitting":
        "first difference";
    std::cerr << "Finding peaks with " << fit_type << std::endl;

    //parse each pulse
    while (raw_data.hasNextPulse()) {
        peaks.clear();

        // gets the raw data from the file
        raw_data.getNextPulse(&pd);

        //Skip all the empty returning waveforms
        if (pd.returningIdx.empty()){
            continue;
        }
        try {
            // Smooth the data and test result
            fitter.smoothing_expt(&pd.returningWave);

            // Check parameter for using gaussian fitting or first differencing
            if (cmdLine.useGaussianFitting) {
                fitter.find_peaks(&peaks, pd.returningWave,
                                     pd.returningIdx, 200);
            } else {
                fitter.guess_peaks(&peaks, pd.returningWave,
                                     pd.returningIdx);
            }

            // for each peak - find the activation point
            //               - calculate x,y,z
            raw_data.calc_xyz_activation(&peaks);

            // for each peak we will call to_string and append them together
            std::string *str = new std::string;
            this->peaks_to_string(*str, cmdLine, peaks);
            strings.push_back(str);
        } catch (const char *msg) {
            std::cerr << msg << std::endl;
        }

        // make sure that we have an empty vector and string
        for (auto i = peaks.begin(); i != peaks.end(); ++i) {
            delete (*i);
        }
    }
}

/**
 * Takes a vector of peaks from a single waveform and concats them together
 * @param str string will be put here
 * @param cmdLine csv_CmdLine object
 * @param peaks vector of peaks to process
 */
void LidarDriver::peaks_to_string(std::string &str, csv_CmdLine &cmdLine,
                                  std::vector<Peak*> &peaks) {
    size_t n = 1;
    std::vector<int> prods = cmdLine.selected_products;
    for (auto i = peaks.begin(); i != peaks.end(); ++i) {
        (*i)->to_string(str, prods);

        if (n < peaks.size()) {
            str += ", ";
        }

        n++;
    }
}

/**
 * setup the bounding and allocate memory for the LidarVolume
 * @param raw_data the flight light data to get values from
 * @param lidar_volume the lidar volume object to allocate
 */
void LidarDriver::setup_lidar_volume(FlightLineData &raw_data,
        LidarVolume &lidar_volume){
    lidar_volume.setBoundingBox(raw_data.bb_x_min, raw_data.bb_x_max,
            raw_data.bb_y_min, raw_data.bb_y_max,
            raw_data.bb_z_min, raw_data.bb_z_max);
    lidar_volume.allocateMemory();
}

/**
 * @param peaks the vector of peaks to add to the lidar volume
 * @param peak_count the count of peaks in the vector ?peaks.size()?
 */
void LidarDriver::add_peaks_to_volume(LidarVolume &lidar_volume,
        std::vector<Peak*> &peaks, int peak_count){
    // give each peak to lidarVolume
    for (int i = 0; i < peak_count; i++) {
        lidar_volume.insert_peak(peaks[i]);
    }
}

/**
 * Calculates the requested information at each peak
 * @param pulse the pulse wave to parse
 * @param peaks the vector containing the returning waveform peaks
 * @param fitter the gaussian fitter object to use for smoothing and fitting
 * @param cmdLine command line object used to supply information to calculations
 * @param gps_info contains gps information of the lidar module
 * @param peak_count count of found peaks returned
 */
void LidarDriver::peak_calculations(PulseData &pulse, std::vector<Peak*> &peaks,
                            GaussianFitter &fitter, CmdLine &cmdLine,
                            WaveGPSInformation &gps_info){
    // Backscatter coefficient
    if (cmdLine.calcBackscatter){
        if (pulse.outgoingIdx.size() == 0){
            return;
        }
        //Go through fitting process with emitted waveform
        fitter.smoothing_expt(&pulse.outgoingWave);

        std::vector<Peak*> emitted_peaks;

        if (cmdLine.useGaussianFitting){
            fitter.find_peaks(&emitted_peaks, pulse.outgoingWave,
                    pulse.outgoingIdx, 10);
        } else {
            fitter.guess_peaks(&emitted_peaks, pulse.outgoingWave,
                    pulse.outgoingIdx);
        }

        //For every returning wave peak, calculate the backscatter coefficient
        for (auto it = peaks.begin(); it != peaks.end(); ++it){
            if (emitted_peaks.size() == 0){
                (*it)->backscatter_coefficient = NO_DATA;
            } else {
                (*it)->calcBackscatter(emitted_peaks.at(0)->amp,
                        emitted_peaks.at(0)->fwhm, cmdLine.calibration_constant,
                        gps_info.x_anchor, gps_info.y_anchor, gps_info.z_anchor);
            }
            if ((*it)->backscatter_coefficient == INFINITY){
                (*it)->backscatter_coefficient = NO_DATA;
            }
        }
    }

    //Check if each peak has a rise time
    for (auto it = peaks.begin(); it != peaks.end(); ++it){
        (*it)->rise_time = (*it)->rise_time < 0 ? NO_DATA : (*it)->rise_time;
    }

    // This is where we should calculate everything that we need to 
    // know about the peak
    // Heights at percent energy
    // Energy at percent heighths
}

/**
 * write the fitted lidar volume data to the given GDAL dataset
 * @param fitted_data the populated lidar volume
 * @param gdal_ds pointer to a prepared dataset to populate
 * @param prod_id the id (product type) of the product to generate
 */
void LidarDriver::produce_product(LidarVolume &fitted_data,
        GDALDataset *gdal_ds, int prod_id)
{
    CPLErr retval;
    //determinr product variable, 'z' = elevation, 'a' = amplitude,
    //'w' = width, 'b' = backscatter coefficient
    char prod_var = prod_id <= 18 ? 'z' : prod_id <= 36 ? 'a' : prod_id <= 54 ?
        'w' : prod_id <= 72 ? 'r' : prod_id <= 90 ? 'b' : '-';
    //determine what data to use, 0 = first, 1 = last, 2 = all
    int prod_data = ((prod_id - 1) % 18) / 6;
    //Make sure it is a valid product
    if (prod_var == '-') {
        //std::cout << "Product # " << prod_id << " Not yet implemented" << 
        //    std::endl;
        return;
    }
    //indexes
    int x, y;
    //place to hold the elevations
    float *pixel_values = (float *) calloc(fitted_data.x_idx_extent,
                                           sizeof(float));
    float avg = 0 ;
    float dev = 0;
#ifdef DEBUG
    std::cerr << "Entering write image loop. In "<< __FILE__ << ":" << __LINE__
        << std::endl;
#endif

    //loop through every pixel position
    for (y = fitted_data.y_idx_extent - 1; y >= 0; y--) {
        for (x = 0; x < fitted_data.x_idx_extent; x++) {
            //get the vector of found peaks at this pixel
            std::vector<Peak*> *peaks =
                fitted_data.volume[fitted_data.position(y, x)];
            //decide what to do with the peak data at this pixel
            switch (prod_id % 6) {
                case 1 : //max
                    pixel_values[x] = get_extreme(peaks, true, prod_data,
                        prod_var);
                    break;
                case 2 : //min
                    pixel_values[x] = get_extreme(peaks, false, prod_data,
                        prod_var);
                    break;
                case 3 : //mean
                    pixel_values[x] = get_mean( peaks, prod_data, prod_var);
                    break;
                case 4: //std-dev
                    avg = get_mean(peaks,prod_data,prod_var);
                    pixel_values[x] = get_deviation(peaks, avg, prod_data,
                        prod_var);
                    break;
                case 5: //skewness
                    avg = get_mean(peaks, prod_data, prod_var);
                    dev = get_deviation(peaks, avg, prod_data, prod_var);
                    pixel_values[x] = get_skewtosis(peaks, avg, dev, prod_data,
                        prod_var, 3);
                    break;
                case 0: //kurtosis
                    avg = get_mean(peaks, prod_data, prod_var);
                    dev = get_deviation(peaks, avg, prod_data, prod_var);
                    pixel_values[x] = get_skewtosis(peaks, avg, dev, prod_data,
                        prod_var, 4);
                    break;
                default:
                    //std::cout << "Product #" << prod_id << " not implemented"
                    //<< std::endl;
                    break;
            }
        }
#ifdef DEBUG
        std::cerr << "In writeImage loop. Writing band: "<< x << "," << y
            << ". In " << __FILE__ << ":" << __LINE__ << std::endl;
#endif
        //add the pixel values to the raster, one column at a time
        // Refer to http://www.gdal.org/classGDALRasterBand.html
        retval = gdal_ds->GetRasterBand(1)->RasterIO(GF_Write, 0,
                fitted_data.y_idx_extent - y - 1, fitted_data.x_idx_extent, 1,
                pixel_values, fitted_data.x_idx_extent, 1, GDT_Float32, 0, 0,
                NULL);
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
void LidarDriver::geo_orient_gdal(LidarVolume &fitted_data,
                                  GDALDataset *gdal_ds,
                                  std::string geog_cs, int utm)
{
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
    transform[5] = -1;  //Always -1

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
 * get the difference between max and min peak property data for a set of peaks
 * in one pass
 * @param peaks the peaks to evaluate
 * @param peak_property the property of the peaks to compare
 * @return the difference between the maximal and minimal element values, or
 * NO_DATA if no peaks
 */
float LidarDriver::get_extreme_diff(std::vector<Peak*> *peaks,
                                    char peak_property)
{
    float max_val = NO_DATA;
    float min_val = MAX_ELEV;
    float cur_val = 0;
    if(peaks==NULL || peaks->empty()){
        return NO_DATA;
    }
    for (std::vector<Peak*>::iterator it = peaks->begin();
            it != peaks->end(); ++it) {
        cur_val = get_peak_property(*it,peak_property);

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
 * get the extreme (max/min) value from a set of peaks, specify the property
 * and peak position (first, last, all)
 * @param peaks the set of peaks to process
 * @param max_flag flag to indicate max (true = max, false = min)
 * @param peak_pos specify if first, last, or all peaks should be included in
 * calculation (0=first, 1=last, 2=all)
 * @param peak_property the property of the peak to analyze (amplitude, width,
 * z-activation, etc..)
 * @return the extreme property value of the set of peaks with the specified 
 * filter
 */
float LidarDriver::get_extreme(std::vector<Peak*> *peaks, bool max_flag,
    int peak_pos, char peak_property){
    float max_val = NO_DATA;
    float min_val = MAX_ELEV;
    float cur_val = 0;
    bool no_countable_peaks = true;
    if(peaks==NULL || peaks->empty()){
        return NO_DATA;
    }
    for (std::vector<Peak*>::iterator it = peaks->begin();
            it != peaks->end(); ++it) {
        //check what type of returns to evaluate
        switch (peak_pos){
            case 0: //first
                if((*it)->position_in_wave!=1){
                    continue;
                }
                no_countable_peaks = false;
                break;
            case 1: //last
                if(!(*it)->is_final_peak){
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
        cur_val = get_peak_property(*it,peak_property);
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
 * get the average (mean) value from a set of peaks, specify the property
 * and peak position (first, last, all)
 * @param peaks the set of peaks to process
 * @param peak_pos specify if first, last, or all peaks should be included in
 * calculation (0=first, 1=last, 2=all)
 * @param peak_property the property of the peak to analyze (amplitude, width,
 * z-activation, etc..)
 * @return the mean property value of the set of peaks with the specified
 * filter
 */
float LidarDriver::get_mean(std::vector<Peak*> *peaks, int peak_pos,
                            char peak_property)
{
    double val_sum  = 0;
    int val_count = 0;
    bool no_countable_peaks = true;
    if(peaks==NULL || peaks->empty()){
        return NO_DATA;
    }
    for (std::vector<Peak*>::iterator it = peaks->begin();
            it != peaks->end(); ++it) {
        //check what type of returns to evaluate
        switch (peak_pos){
            case 0: //first
                if((*it)->position_in_wave!=1){
                    continue;
                }
                no_countable_peaks = false;
                break;
            case 1: //last
                if(!(*it)->is_final_peak){
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
        val_sum += get_peak_property(*it,peak_property);
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
 * @param peak_property the property of the peak to analyze (amplitude, width,
 * z-activation, etc..)
 * @return the property value of the peak
 */
float LidarDriver::get_peak_property(Peak *peak, char peak_property)
{
    switch (peak_property){
        case 'z':
            return peak->z_activation;
        case 'a':
            return peak->amp;
        case 'w':
            return peak->fwhm;
        case 'r':
            return peak->rise_time;
        case 'b':
            return peak->backscatter_coefficient;
        default:
            break;
    }
    //TODO: get rid of the exit statement
    std::cout << "CRITICAL ERROR! \
        No implemented peak property for identifier "<<peak_property<<"\n";
    exit(EXIT_FAILURE);
}

/**
 * get the standard deviation value from a set of peaks, specify the property
 * and peak position (first, last, all)
 * @param peaks the set of peaks to process
 * @param avg the average (mean) of the set
 * @param peak_pos specify if first, last, or all peaks should be included in
 * calculation (0=first, 1=last, 2=all)
 * @param peak_property the property of the peak to analyze (amplitude, width,
 * z-activation, etc..)
 * @return the standard deviation of the property value of the set of peaks with
 * the specified filter
 */
double LidarDriver::get_deviation(std::vector<Peak*> *peaks, double avg,
                                  int peak_pos, char peak_property)
{
    double E=0;
    float cur_val=0;
    int peak_count = 0;
    if(peaks==NULL || peaks->empty()){
        return NO_DATA;
    }
    for (std::vector<Peak*>::iterator it = peaks->begin();
            it != peaks->end(); ++it) {
        cur_val = get_peak_property(*it,peak_property);
        switch(peak_pos) {
            case 0: //first
                if ((*it)->position_in_wave == 1) {
                    peak_count++;
                    E += pow(static_cast<double>(cur_val) - avg, 2);
                }
                break;
            case 1: //last
                if ((*it)->is_final_peak) {
                    peak_count++;
                    E += pow(static_cast<double>(cur_val) - avg, 2);
                }
                break;
            case 2: //all
                peak_count++;
                E += pow(static_cast<double>(cur_val) - avg, 2);
        }
    }
    //No applicable data
    if (peak_count == 0){
        return NO_DATA;
    }
    double inverse = 1.0 / static_cast<double>(peak_count-1);
    return sqrt(inverse * E);
}


/**
 * get the skewness or kurtosis (3rd/4th degree) value from a set of peaks,
 * specify the property and peak position (first,last, all)
 * @param peaks the set of peaks to process
 * @param avg the average (mean) of the set
 * @param dev the standard deviation of the set
 * @param peak_pos specify if first, last, or all peaks should be included in
 * calculation (0=first, 1=last, 2=all)
 * @param peak_property the property of the peak to analyze (amplitude, width,
 * z-activation, etc..)
 * @param power the power of the quadratic in the calculation (3=skewness,
 * 4=kurtosis)
 * @return the skewness or kurtosis of the property value of the set of peaks
 * with the specified filter
 */
double LidarDriver::get_skewtosis(std::vector<Peak*> *peaks, double avg,
                                  double dev, int peak_pos, char peak_property,
                                  int power)
{
    double G=0;
    double cur_val = 0;
    int peak_count = 0;
    if(peaks==NULL || peaks->empty()){
        return NO_DATA;
    }
    //All data points were exactly the same so return normal distribution
    if(dev == 0){
        return 0;
    }
    for (std::vector<Peak*>::iterator it = peaks->begin();
            it != peaks->end(); ++it) {
        cur_val = get_peak_property(*it,peak_property);
        switch (peak_pos){
            case 0: //first peaks
                if ((*it)->position_in_wave == 1) {
                    peak_count++;
                    G += pow(static_cast<double>(cur_val) - avg, power);
                }
                break;
            case 1: //last peaks
                if ((*it)->is_final_peak) {
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
    //No applicable data
    if (peak_count == 0){
        return NO_DATA;
    }
    double inverse = 1.0 / static_cast<double>(peak_count);
    return (inverse * G) / pow(dev,power) ;
}
