//
// Created by arezaii on 3/24/19.
//

#ifndef LIDARDRIVER_LIDARDRIVER_HPP
#define LIDARDRIVER_LIDARDRIVER_HPP

#include "CmdLine.hpp"
#include "FlightLineData.hpp"
#include "WaveGPSInformation.hpp"
#include "LidarVolume.hpp"
#include "PulseData.hpp"
#include "Peak.hpp"
#include "GaussianFitter.hpp"
#include <iostream>
#include <iomanip>
#include <vector>

const double NO_DATA = -99999.99;
const double MAX_ELEV = 99999.99;

class LidarDriver {
    public:

        void setup_flight_data(FlightLineData &data, std::string inputFileName);

        void fit_data(FlightLineData &raw_data, LidarVolume &fitted_data,
                bool useGaussianFitting, double calibration_constant);
        void produce_product(LidarVolume &fitted_data, GDALDataset *gdal_ds, int prod_id);
        void setup_lidar_volume(FlightLineData &raw_data, LidarVolume &lidar_volume);
        int parse_pulse(PulseData &pulse, std::vector<Peak*> &peaks, Peak* emitted_pulse,
                GaussianFitter &fitter, bool use_gaussian_fitting, int &peak_count);
        void add_peaks_to_volume(LidarVolume &lidar_volume, std::vector<Peak*> &peaks, int peak_count);

        void produce_product(
                LidarVolume &fitted_data, GDALDataset *gdal_ds, int prod_id);

        void setup_lidar_volume(
                FlightLineData &raw_data, LidarVolume &lidar_volume);

        int parse_pulse(
                PulseData &pulse, std::vector<Peak*> &peaks,
                GaussianFitter &fitter, bool use_gaussian_fitting,
                int &peak_count);

        void add_peaks_to_volume(
                LidarVolume &lidar_volume, std::vector<Peak*> &peaks,
                int peak_count);

>>>>>>> 6e7cd9d5dd754b0019c88653bf88ede45cb57e78
        GDALDriver * setup_gdal_driver();

        GDALDataset * setup_gdal_ds(GDALDriver *tiff_driver,
                std::string filename, std::string band_desc,
                int x_idx_extent, int y_idx_extent);

        void geo_orient_gdal(LidarVolume &fitted_data,
                GDALDataset *gdal_ds, std::string geog_cs, int utm);

        float get_extreme_diff(std::vector<Peak*> *peaks, char peak_property);

        float get_extreme(std::vector<Peak*> *peaks, bool max_flag,
                int peak_pos, char peak_property);

        float get_mean(std::vector<Peak*> *peaks, int peak_pos,
                char peak_property);

        double get_deviation(std::vector<Peak*> *peaks, double avg,
                int peak_pos, char peak_property);

        float get_peak_property(Peak *peak, char peak_property);

        double get_skewtosis(std::vector<Peak*> *peaks, double avg, double dev,
                int peak_pos, char peak_property, int power);
};


#endif //LIDARDRIVER_LIDARDRIVER_HPP
