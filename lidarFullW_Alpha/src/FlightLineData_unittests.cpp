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
  int TruthScannerId = 1; 
  double TruthWaveLength = 1064;
  double TruthOutgoingPulseWidth = 4.822761;
  int TruthScanPattern = 2;
  int TruthNumberOfMirrorFacets = 4;
  double TruthScanFrequency = 146.666672;
  double TruthScanAngleMin = 0.00001;
  double TruthScanAngleMax = 359.999969;
  double TruthPulseFrequency = 400;
  double TruthBeamDiameterAtExitAperture = 10;
  double TruthBeamDivergence = 0.5;
  double TruthMinimalRange = 660.48053;
  double TruthMaximalRange = 1860.605225;
  
  std::string fileName = "bin/140823_183115_1_clipped_test.pls";
  ASSERT_ANY_THROW (fld.setFlightLineData(fileName));

  std::string fileName1 = "etc/140823_183115_1_clipped_test.dat";
  ASSERT_ANY_THROW (fld.setFlightLineData(fileName1));

  std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";
  ASSERT_NO_THROW (fld.setFlightLineData(fileNameCorrect));
 
  ASSERT_EQ (truth_scanner_id, fld.scanner_id);
  ASSERT_DOUBLE_EQ (TruthWaveLength, fld.wave_length);
  ASSERT_NEAR (TruthOutgoingPulseWidth, fld.outgoing_pulse_width,
              0.0000005);
  ASSERT_EQ (TruthScanPattern, fld.scan_pattern);
  ASSERT_EQ (TruthNumberOfMirrorFacets, fld.number_of_mirror_facets);
  ASSERT_NEAR (TruthScanFrequency, fld.scan_frequency, 0.000001);
  ASSERT_NEAR (TruthScanAngleMin, fld.scan_angle_min, 0.0000001);
  ASSERT_NEAR (TruthScanAngleMax, fld.scan_angle_max, 0.0000005);
  ASSERT_DOUBLE_EQ (TruthPulseFrequency, fld.pulse_frequency);
  ASSERT_DOUBLE_EQ (TruthBeamDiameterAtExitAperture, 
                   fld.beam_diameter_at_exit_aperture);
  ASSERT_NEAR (TruthBeamDivergence, fld.beam_divergence, 0.01);
  ASSERT_NEAR (TruthMinimalRange, fld.minimal_range, 0.00001);
  ASSERT_NEAR (TruthMaximalRange, fld.maximal_range, 0.000001);
}
