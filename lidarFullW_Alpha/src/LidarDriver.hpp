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


class LidarDriver {
public:
	void setup_flight_data(FlightLineData &data, std::string inputFileName);
	void fit_data(FlightLineData &, LidarVolume &, bool useGaussianFitting);
	void produce_product(LidarVolume &, GDALDataset *, int );
	void setup_lidar_volume(FlightLineData &, LidarVolume &);
	int parse_pulse(PulseData &, std::vector<Peak> &, GaussianFitter &, bool , int &);
	void add_peaks_to_volume(LidarVolume &,std::vector<Peak> &, int );
	GDALDriver * setup_gdal_driver();
	GDALDataset * setup_gdal_ds(GDALDriver *tiff_driver, std::string filename, std::string band_desc,
			int x_idx_extent, int y_idx_extent);
	void geo_orient_gdal(LidarVolume &, GDALDataset *, std::string , int );
	float get_z_activation_extreme(std::vector<Peak> *peaks, bool);
	float get_z_activation_diff(std::vector<Peak> *peaks);
	float get_amplitude_extreme(std::vector<Peak> *peaks, bool max_flag, int return_pos);
	float get_amplitude_mean(std::vector<Peak> *peaks, int return_pos);
};


#endif //LIDARDRIVER_LIDARDRIVER_HPP
