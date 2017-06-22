/*
 * File name: ScannerInformation_unittests.cpp
 * Created on: 17-June-2017
 * Author: ravi
 *
 * This test requres the sample 140823_183115_1_clipped_test.pls file 
 * and its corresponding wvs file to be in the etc directory to run
 *
 */

#include "gtest/gtest.h"
#include "GPSInformation.hpp"


class GPSInformationTest: public testing::Test{
  protected:

  GPSInformation gps;

  // function to set up space used by all tests
  virtual void SetUp(){
  }

};

/*
* Given the true values of the actual anchor, target and deviation,
* we calculate the actual anchor, actual target and actual deviation
* using FullWaveformIngestion functions
*/
TEST_F(ScannerInformationTest, checkValues){

  // Known GPS information
  double gpsTime;
  double xAnchor, yAnchor, zAnchor;
  double xTarget, yTarget, zTarget;
  double xDeviation, yDeviation, zDeviation;
  double xFirst, yFirst, zFirst;
  double xLast, yLast, zLast;
  unsigned char edge;
  unsigned char facet;
  unsigned char scanDirection;
  unsigned char intensity;
  
  std::string fileName = "bin/140823_183115_1_clipped_test.pls";
  ASSERT_ANY_THROW (scannerInfo.setScannerInformation(fileName));

  std::string fileName1 = "etc/140823_183115_1_clipped_test.dat";
  ASSERT_ANY_THROW (scannerInfo.setScannerInformation(fileName1));

  std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";
  ASSERT_NO_THROW (scannerInfo.setScannerInformation(fileNameCorrect));
 
  ASSERT_EQ (TruthScannerId, scannerInfo.scannerId);
  ASSERT_DOUBLE_EQ (TruthWaveLength, scannerInfo.waveLength);
  ASSERT_NEAR (TruthOutgoingPulseWidth, scannerInfo.outgoingPulseWidth,
              0.0000005);
  ASSERT_EQ (TruthScanPattern, scannerInfo.scanPattern);
  ASSERT_EQ (TruthNumberOfMirrorFacets, scannerInfo.numberOfMirrorFacets);
  ASSERT_NEAR (TruthScanFrequency, scannerInfo.scanFrequency, 0.000001);
  ASSERT_NEAR (TruthScanAngleMin, scannerInfo.scanAngleMin, 0.0000001);
  ASSERT_NEAR (TruthScanAngleMax, scannerInfo.scanAngleMax, 0.0000005);
  ASSERT_DOUBLE_EQ (TruthPulseFrequency, scannerInfo.pulseFrequency);
  ASSERT_DOUBLE_EQ (TruthBeamDiameterAtExitAperture, 
                   scannerInfo.beamDiameterAtExitAperture);
  ASSERT_NEAR (TruthBeamDivergence, scannerInfo.beamDivergence, 0.01);
  ASSERT_NEAR (TruthMinimalRange, scannerInfo.minimalRange, 0.00001);
  ASSERT_NEAR (TruthMaximalRange, scannerInfo.maximalRange, 0.000001);
}
