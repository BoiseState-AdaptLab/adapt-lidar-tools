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
 * Calculates the memory needed to store all the requested products
 * @param data the FlightLineData object containing bounding box information
 * @param num_products the number of products that will be produced
 */
void LidarDriver::calc_product_size(FlightLineData &data, int num_products){
    //This keeps track of units
    size_t i = 0;
    double bytes = -1;
    std::vector<std::string> units = {"B","KB","MB","GB"};

    //If we get bytes < 0 then we had an overflow
    for (i = 0; bytes < 0 and i < units.size(); i++){
        //Number of floats we are storing per product, based on the bounding box
        double vals_per_product = std::ceil(data.bb_x_max - data.bb_x_min) *
            std::ceil(data.bb_y_max - data.bb_y_min);
        //The number of bytes allocated for each float value
        float bytes_per_val = sizeof(float);
        //Find conversion from bytes to current prefix (kilo,mega,giga)
        int prefix_conversion = i == 0 ? 1 : pow(1024,i);
        //Use dimensional analysis to cancel out all units except for bytes
        bytes = bytes_per_val * vals_per_product * num_products / prefix_conversion;

        spdlog::error("Values per product: {}", vals_per_product);
        spdlog::trace("Bytes per value (float): {}", bytes_per_val);
        spdlog::trace("Conversion to {}. Divide by {}", units.at(i), prefix_conversion);
        spdlog::trace("Total bytes needed: {}", bytes);

    }

    //Find the best unit
    for (i = 0; i < units.size() && bytes >= 1024; i ++){
        bytes /= 1024;
    }

    //Round to two decimals
    bytes = floorf(bytes * 100) / 100;

    spdlog::trace("{} Tif files will require approximately {} {}", num_products,
                  bytes, units.at(i));
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
    fitter.noise_level = cmdLine.noise_level;
    if (cmdLine.max_amp_multiplier != 0.0)
        fitter.max_amp_multiplier = cmdLine.max_amp_multiplier;
    std::vector<Peak*> peaks;
    int peak_count = 0;

    spdlog::debug("Start finding peaks. In {}:{}", __FILE__, __LINE__);

    //setup the lidar volume bounding and allocate memory
    setup_lidar_volume(raw_data, fitted_data);

    //message the user
    std::string fit_type=cmdLine.useGaussianFitting?"gaussian fitting":
        "first difference";
    spdlog::info("Finding peaks with {}", fit_type);

    //Initialize variables to store max and min xyz values of the data
    //bool first = true;
    //int bb_x_min, bb_x_max, bb_y_min, bb_y_max, bb_z_min, bb_z_max;

    //parse each pulse
    while (raw_data.hasNextPulse()) {
        // make sure that we have an empty vector
        peaks.clear();

        // gets the raw data from the file
        raw_data.getNextPulse(&pd);

        //Check if the xyz of the last data point in the waveform is a new
        //max or min
        /*int x = raw_data.current_wave_gps_info.x_last;
        int y = raw_data.current_wave_gps_info.y_last;
        int z = raw_data.current_wave_gps_info.z_last;
        if (first){
            bb_x_min = bb_x_max = x;
            bb_y_min = bb_y_max = y;
            bb_z_min = bb_z_max = z;
            first = false;
        } else {
            bb_x_min = bb_x_min > x ? x : bb_x_min;
            bb_x_max = bb_x_max < x ? x : bb_x_max;
            bb_y_min = bb_y_min > y ? y : bb_y_min;
            bb_y_max = bb_y_max < y ? y : bb_y_max;
            bb_z_min = bb_z_min > z ? z : bb_z_min;
            bb_z_max = bb_z_max < z ? z : bb_z_max;
        }*/
       
        //Skip all the empty returning waveforms
        if (pd.returningIdx.size == 0){
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

    //Print the bounding box and our extreme x,y,z coordinates
    /*std::cout << "Bounding Box: " << fitted_data.bb_x_min << "-" <<
        fitted_data.bb_x_max << " X " << fitted_data.bb_y_min << "-" <<
        fitted_data.bb_y_max << " X " << fitted_data.bb_z_min << "-" <<
        fitted_data.bb_z_max << std::endl;
    std::cout << "Extreme XYZ: " << bb_x_min << "-" << bb_x_max << " X " <<
        bb_y_min << "-" << bb_y_max << " X " << bb_z_min << "-" << bb_z_max <<
        std::endl;*/

    spdlog::debug("Total: {}", fitter.get_total());
    spdlog::debug("Pass: {}", fitter.get_pass());
    spdlog::debug("Fail: {}", fitter.get_fail());
}

void log_raw_data(struct vector idx, struct vector wave) {
    //Print raw wave
    std::stringstream idxstr;
    std::stringstream wavestr;
    std::copy(idx.buffer, idx.buffer + idx.size, std::ostream_iterator<int>(idxstr, " "));
    std::copy(wave.buffer, wave.buffer + wave.size,
            std::ostream_iterator<int>(wavestr, " "));
    spdlog::trace("raw pulse idx: {}; raw pulse wave: {}", idxstr.str(),
            wavestr.str());
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

    bool log_diagnostics = cmdLine.log_diagnostics;

    if (log_diagnostics) fitter.setDiagnostics(true);

    //message the user
    std::string fit_type=cmdLine.useGaussianFitting?"gaussian fitting":
        "first difference";
    spdlog::info("Finding peaks with {}", fit_type);

    //parse each pulse
    while (raw_data.hasNextPulse()) {
        peaks.clear();

        // gets the raw data from the file
        raw_data.getNextPulse(&pd);

        // Skip all the empty returning waveforms
        if (pd.returningIdx.size == 0){
            if (log_diagnostics) {
                spdlog::trace("pulse idx was empty, skipping");
            }
            continue;
        }

        // Log data
        if (log_diagnostics) {
            log_raw_data(pd.returningIdx, pd.returningWave);
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
            spdlog::error("{}", msg);
        }

        // make sure that we have an empty vector and string
        for (auto i = peaks.begin(); i != peaks.end(); ++i) {
            delete (*i);
        }
    }
}

/**
 * Overloads fit_data_csv for TxtWaveReader support
 * @param raw_data reference to TxtWaveReader object that holds raw data
 * @param strings a place where peak to_string calls will be stored
 * @param csv_CmdLine object that knows what data we want from peaks
 */
void LidarDriver::fit_data_csv(TxtWaveReader &raw_data,
        std::vector<std::string*> &strings, csv_CmdLine &cmdLine) 
{
    std::ostringstream stream;
    GaussianFitter fitter;
    std::vector<Peak*> peaks;

    bool log_diagnostics = cmdLine.log_diagnostics;

    if (log_diagnostics) fitter.setDiagnostics(true);

    //message the user
    std::string fit_type=cmdLine.useGaussianFitting?"gaussian fitting":
        "first difference";
    spdlog::info("Finding peaks with {}", fit_type);

    //parse each pulse
    while (raw_data.next_wave()) {

        //Skip all the empty returning waveforms
        if (raw_data.idx.empty()){
            if (log_diagnostics) {
                spdlog::trace("pulse idx was empty, skipping");
            }
            continue;
        }

        struct vector idx;
        idx.buffer = raw_data.idx.data();
        idx.size = raw_data.idx.size();
        idx.capacity = raw_data.idx.capacity();
        struct vector wave;
        wave.buffer = raw_data.wave.data();
        wave.size = raw_data.wave.size();
        wave.capacity = raw_data.wave.capacity();
        // Log data
        if (log_diagnostics) {
            log_raw_data(idx, wave);
        }

        try {

            // Smooth the data and test result
            fitter.smoothing_expt(&idx);

            // Check parameter for using gaussian fitting or first differencing
            if (cmdLine.useGaussianFitting) {
                fitter.find_peaks(&peaks, wave,
                                     idx, 200);
            } else {
                fitter.guess_peaks(&peaks, wave,
                                     idx);
            }

            // xyz calc isn't available for TxtWaveReader

            // for each peak we will call to_string and append them together
            std::string *str = new std::string;
            this->peaks_to_string(*str, cmdLine, peaks);
            strings.push_back(str);
        } catch (const char *msg) {
            spdlog::error("{}", msg);
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
        if (pulse.outgoingIdx.size == 0){
            return;
        }
        //Go through fitting process with emitted waveform
        fitter.smoothing_expt(&pulse.outgoingWave);

        std::vector<Peak*> emitted_peaks;

        if (cmdLine.useGaussianFitting){
            fitter.find_peaks(&emitted_peaks, pulse.outgoingWave,
                    pulse.outgoingIdx, 200);
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
 * @param prod_calc the code of the calculation to use
 * @param prod_peaks the code of the peaks to use
 * @param prod_var the code the variable to use
 */
void LidarDriver::produce_product(LidarVolume &fitted_data,
        GDALDataset *gdal_ds, int prod_calc, int prod_peaks, int prod_var)
{
    CPLErr retval;
    
    //indexes
    int x, y;
    //place to hold the elevations
    float *pixel_values = (float *) calloc(fitted_data.x_idx_extent,
                                           sizeof(float));
    float avg = 0 ;
    float dev = 0;

    spdlog::error("Entering write image loop. In {} : {}", __FILE__, __LINE__);



    //loop through every pixel position
    for (y = fitted_data.y_idx_extent - 1; y >= 0; y--) {
        for (x = 0; x < fitted_data.x_idx_extent; x++) {
            //get the vector of found peaks at this pixel
            std::vector<Peak*> *peaks =
                fitted_data.volume[fitted_data.position(y, x)];
            //decide what to do with the peak data at this pixel
            switch (prod_calc) {
                case 0: //max
                    pixel_values[x] = get_extreme(peaks, true, prod_peaks,
                        prod_var);
                    break;
                case 1: //min
                    pixel_values[x] = get_extreme(peaks, false, prod_peaks,
                        prod_var);
                    break;
                case 2: //mean
                    pixel_values[x] = get_mean( peaks, prod_peaks, prod_var);
                    break;
                case 3: //std-dev
                    avg = get_mean(peaks,prod_peaks,prod_var);
                    pixel_values[x] = get_deviation(peaks, avg, prod_peaks,
                        prod_var);
                    break;
                case 4: //skewness
                    avg = get_mean(peaks, prod_peaks, prod_var);
                    dev = get_deviation(peaks, avg, prod_peaks, prod_var);
                    pixel_values[x] = get_skewtosis(peaks, avg, dev, prod_peaks,
                        prod_var, 3);
                    break;
                case 5: //kurtosis
                    avg = get_mean(peaks, prod_peaks, prod_var);
                    dev = get_deviation(peaks, avg, prod_peaks, prod_var);
                    pixel_values[x] = get_skewtosis(peaks, avg, dev, prod_peaks,
                        prod_var, 4);
                    break;
                default:
                    //std::cout << "Product #" << prod_id << " not implemented"
                    //<< std::endl;
                    break;
            }
        }

        spdlog::error("In writeImage loop. Writing band: {}, {}. In {} : {}", x, y,
                       __FILE__, __LINE__);

        //add the pixel values to the raster, one column at a time
        // Refer to http://www.gdal.org/classGDALRasterBand.html
        retval = gdal_ds->GetRasterBand(1)->RasterIO(GF_Write, 0,
                fitted_data.y_idx_extent - y - 1, fitted_data.x_idx_extent, 1,
                pixel_values, fitted_data.x_idx_extent, 1, GDT_Float32, 0, 0,
                NULL);
        if (retval != CE_None) {
            spdlog::error("Error during writing band: 1 ");
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
        case 0: //elevation
            return peak->z_activation;
        case 1: //amplitude
            return peak->amp;
        case 2: //pulse width
            return peak->fwhm;
        case 3: //rise time
            return peak->rise_time;
        case 4: //backscatter coefficient
            return peak->backscatter_coefficient;
        default:
            break;
    }

    spdlog::critical("No implemented peak property for identifier {}. Result of"
            " property request undefined (returning 0)", peak_property);
    return 0;
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
    inverse = sqrt(inverse * E);
    return std::isfinite(inverse) ? inverse : NO_DATA;
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
    if(peaks==NULL || peaks->empty() || dev == NO_DATA){
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
    double inverse = 1.0 / static_cast<double>(peak_count-1);
    inverse = (inverse * G) / pow(dev,power);
    return std::isfinite(inverse) ? inverse : NO_DATA;
}
