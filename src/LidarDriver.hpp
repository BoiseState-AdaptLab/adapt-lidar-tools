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
#include "csv_CmdLine.hpp"

const double NO_DATA = -99999;
const double MAX_ELEV = 99999.99;

class LidarDriver {
    public:

        void setup_flight_data(FlightLineData &data, std::string inputFileName);

        void fit_data(FlightLineData &raw_data, LidarVolume &fitted_data,
                CmdLine &cmdLine);

        void fit_data_csv(FlightLineData &raw_data,
                          std::vector<std::string*> &strings,
                          csv_CmdLine &cmdLine);

        void peaks_to_string(std::string &str, csv_CmdLine &cmdLine,
                             std::vector<Peak*> &peaks);

        void produce_product(LidarVolume &fitted_data, GDALDataset *gdal_ds,
                int prod_id);

        void setup_lidar_volume(FlightLineData &raw_data,
                LidarVolume &lidar_volume);

        void peak_calculations(PulseData &pulse, std::vector<Peak*> &peaks,
                GaussianFitter &fitter, CmdLine &cmdLine,
                WaveGPSInformation &gps_info);

        void add_peaks_to_volume(LidarVolume &lidar_volume,
                std::vector<Peak*> &peaks, int peak_count);

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
