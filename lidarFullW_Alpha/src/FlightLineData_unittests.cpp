/*
 * File name: FlightLineData_unittests.cpp
 * Created on: 09- November-2017
 * Author: ravi
 */

#include "gtest/gtest.h"
#include "FlightLineData.hpp"


class FlightLineDataTest: public testing::Test{
  protected:

  FlightLineData fld;

  // function to set up space used by all tests
  virtual void SetUp(){
  }

};

/*
* Given the true values of the actual anchor, target and deviation,
* we calculate the actual anchor, actual target and actual deviation
* using FullWaveformIngestion functions
*/
TEST_F(FlightLineDataTest, testHeaderValues){

  // Known FlightLineData 
  int known_scanner_id = 1; 
  double known_wave_length = 1064;
  double known_outgoing_pulse_width = 4.822761;
  int known_scan_pattern = 2;
  int known_numer_of_mirror_facets = 4;
  double known_scan_frequency = 146.666672;
  double known_scan_angle_min = 0.00001;
  double known_scan_angle_max = 359.999969;
  double known_pulse_frequency = 400;
  double known_beam_diameter_at_exit_aperture = 10;
  double known_beam_divergence = 0.5;
  double known_minimal_range = 660.48053;
  double known_maximal_range = 1860.605225;
  
  std::string fileName = "bin/140823_183115_1_clipped_test.pls";
  ASSERT_ANY_THROW (fld.setFlightLineData(fileName));

  std::string fileName1 = "etc/140823_183115_1_clipped_test.dat";
  ASSERT_ANY_THROW (fld.setFlightLineData(fileName1));

  std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";
  ASSERT_NO_THROW (fld.setFlightLineData(fileNameCorrect));
 
  ASSERT_EQ (known_scanner_id, fld.scanner_id);
  ASSERT_DOUBLE_EQ (known_wave_length, fld.wave_length);
  ASSERT_NEAR (known_outgoing_pulse_width, fld.outgoing_pulse_width,
              0.0000005);
  ASSERT_EQ (known_scan_pattern, fld.scan_pattern);
  ASSERT_EQ (known_numer_of_mirror_facets, fld.number_of_mirror_facets);
  ASSERT_NEAR (known_scan_frequency, fld.scan_frequency, 0.000001);
  ASSERT_NEAR (known_scan_angle_min, fld.scan_angle_min, 0.0000001);
  ASSERT_NEAR (known_scan_angle_max, fld.scan_angle_max, 0.0000005);
  ASSERT_DOUBLE_EQ (known_pulse_frequency, fld.pulse_frequency);
  ASSERT_DOUBLE_EQ (known_beam_diameter_at_exit_aperture, 
                   fld.beam_diameter_at_exit_aperture);
  ASSERT_NEAR (known_beam_divergence, fld.beam_divergence, 0.01);
  ASSERT_NEAR (known_minimal_range, fld.minimal_range, 0.00001);
  ASSERT_NEAR (known_maximal_range, fld.maximal_range, 0.000001);
}
