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
  int KnownScannerId = 1; 
  double KnownWaveLength = 1064;
  double KnownOutgoingPulseWidth = 4.822761;
  int KnownScanPattern = 2;
  int KnownNumberOfMirrorFacets = 4;
  double KnownScanFrequency = 146.666672;
  double KnownScanAngleMin = 0.00001;
  double KnownScanAngleMax = 359.999969;
  double KnownPulseFrequency = 400;
  double KnownBeamDiameterAtExitAperture = 10;
  double KnownBeamDivergence = 0.5;
  double KnownMinimalRange = 660.48053;
  double KnownMaximalRange = 1860.605225;
  
  std::string fileName = "bin/140823_183115_1_clipped_test.pls";
  ASSERT_ANY_THROW (fld.setFlightLineData(fileName));

  std::string fileName1 = "etc/140823_183115_1_clipped_test.dat";
  ASSERT_ANY_THROW (fld.setFlightLineData(fileName1));

  std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";
  ASSERT_NO_THROW (fld.setFlightLineData(fileNameCorrect));
 
  ASSERT_EQ (Known_scanner_id, fld.scanner_id);
  ASSERT_DOUBLE_EQ (KnownWaveLength, fld.wave_length);
  ASSERT_NEAR (KnownOutgoingPulseWidth, fld.outgoing_pulse_width,
              0.0000005);
  ASSERT_EQ (KnownScanPattern, fld.scan_pattern);
  ASSERT_EQ (KnownNumberOfMirrorFacets, fld.number_of_mirror_facets);
  ASSERT_NEAR (KnownScanFrequency, fld.scan_frequency, 0.000001);
  ASSERT_NEAR (KnownScanAngleMin, fld.scan_angle_min, 0.0000001);
  ASSERT_NEAR (KnownScanAngleMax, fld.scan_angle_max, 0.0000005);
  ASSERT_DOUBLE_EQ (KnownPulseFrequency, fld.pulse_frequency);
  ASSERT_DOUBLE_EQ (KnownBeamDiameterAtExitAperture, 
                   fld.beam_diameter_at_exit_aperture);
  ASSERT_NEAR (KnownBeamDivergence, fld.beam_divergence, 0.01);
  ASSERT_NEAR (KnownMinimalRange, fld.minimal_range, 0.00001);
  ASSERT_NEAR (KnownMaximalRange, fld.maximal_range, 0.000001);
}
