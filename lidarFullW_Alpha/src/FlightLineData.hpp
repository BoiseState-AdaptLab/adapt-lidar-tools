// File name: FlightLineData.hpp
// Created on: 09-November-2017
// Author: ravi

#ifndef FLIGHTLINEDATA_HPP_
#define FLIGHTLINEDATA_HPP_

#include <iostream>
#include <fstream>
#include <vector>
#include "pulsereader.hpp"
#include "pulsewriter.hpp"
#include "PulseData.hpp"
#include "Peak.hpp"
#include "WaveGPSInformation.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <obstack.h>
#include <algorithm>


class FlightLineData{

  public:
    //The min and max fields describing the boundary box that includes the
    //first & last points of the sampled parts of the returning waveforms of
    //all pulses
    double bb_x_min;
    double bb_y_min;
    double bb_z_min;
    double bb_x_max;
    double bb_y_max;
    double bb_z_max;

    //Instrument information
    int scanner_id;
    double wave_length;
    double outgoing_pulse_width;
    int scan_pattern;
    int number_of_mirror_facets;
    double scan_frequency;
    double scan_angle_min;
    double scan_angle_max;
    double pulse_frequency;
    double beam_diameter_at_exit_aperture;
    double beam_divergence;
    double minimal_range;
    double maximal_range;

    std::string geoascii_params;
    std::string utm_str;
    std::string geog_cs;
    int utm;

    //Depends on whether there is a next pulse
    bool next_pulse_exists;

    //Stores pulse data one at a time
    std::vector<int> outgoing_time;
    std::vector<int> outgoing_wave;
    std::vector<int> returning_time;
    std::vector<int> returning_wave;

    WaveGPSInformation current_wave_gps_info;

    FlightLineData();
    void setFlightLineData(std::string fileName);
    void FlightLineDataToCSV();
    bool hasNextPulse();
    void getNextPulse(PulseData* pd);;
    int calc_xyz_activation(std::vector<Peak> *peaks);
	void closeFlightLineData(void);
	int parse_for_UTM_value(std::string input);

  private:
    PULSEreadOpener pOpener;
    PULSEreader *pReader;
    WAVESsampling *sampling;
    PULSEscanner scanner;

};

#endif /* FLIGHTLINEDATA_HPP_ */
