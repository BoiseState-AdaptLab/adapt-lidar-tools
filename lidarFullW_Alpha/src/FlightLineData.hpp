/*
 * File name: FlightLineData.hpp
 * Created on: 09- November-2017
 * Author: ravi
 */

#ifndef FLIGHTLINEDATAUNITTESTS_HPP_
#define FLIGHTLINEDATAUNITTESTS_HPP_

#include <iostream>
#include <fstream>
#include "pulsereader.hpp"
#include "pulsewriter.hpp"

class FlightLineData{

public:
  //The min and max fields describing the boundary box that includes the 
  //first & last points of the sampled parts of the returning waveforms of 
  //all pulses
  int bb_x_min;
  int bb_y_min;
  int bb_z_min;
  int bb_x_max;
  int bb_y_max;
  int bb_z_max;  

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

  PULSEreadOpener pOpener;
  PULSEreader *pReader;
  PULSEscanner scanner;

  FlightLineData();
  
  void setFlightLineData(std::string fileName);
  void FlightLineDataToCSV();
};

#endif /* FLIGHTLINEDATAUNITTESTS_HPP_ */
