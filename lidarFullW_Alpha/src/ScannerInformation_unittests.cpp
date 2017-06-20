/*
 * File name: ScannerInformation_unittests.cpp
 * Created on: 17-June-2017
 * Author: ravi
 *
 * This test requres the sample 140823_183115_1_clipped_test.pls file 
 * and its corresponding WVS file to be in the bin directory to run
 *
 */

#include "gtest/gtest.h"
#include "ScannerInformation.hpp"


class ScannerInformationTest: public testing::Test {
  protected:

  ScannerInformation scannerInfo;

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
  ASSERT_ANY_THROW (scannerInfo.setScannerInformation(fileName));

  std::string fileName1 = "etc/140823_183115_1_clipped_test.dat";
  ASSERT_ANY_THROW (scannerInfo.setScannerInformation(fileName1));

  std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";
  ASSERT_NO_THROW (scannerInfo.setScannerInformation(fileNameCorrect));
 
  ASSERT_EQ(TruthScannerId, scannerInfo.scannerId);
  ASSERT_DOUBLE_EQ(TruthWaveLength, scannerInfo.waveLength);
  ASSERT_NEAR(TruthOutgoingPulseWidth, scannerInfo.outgoingPulseWidth,
              0.0000005);
  ASSERT_EQ(TruthScanPattern, scannerInfo.scanPattern);
  ASSERT_EQ(TruthNumberOfMirrorFacets, scannerInfo.numberOfMirrorFacets);
  ASSERT_NEAR(TruthScanFrequency, scannerInfo.scanFrequency, 0.000001);
  ASSERT_NEAR(TruthScanAngleMin, scannerInfo.scanAngleMin, 0.0000001);
  ASSERT_NEAR(TruthScanAngleMax, scannerInfo.scanAngleMax, 0.0000005);
  ASSERT_DOUBLE_EQ(TruthPulseFrequency, scannerInfo.pulseFrequency);
  ASSERT_DOUBLE_EQ(TruthBeamDiameterAtExitAperture, 
                   scannerInfo.beamDiameterAtExitAperture);
  ASSERT_NEAR(TruthBeamDivergence, scannerInfo.beamDivergence, 0.01);
  ASSERT_NEAR(TruthMinimalRange, scannerInfo.minimalRange, 0.00001);
  ASSERT_NEAR(TruthMaximalRange, scannerInfo.maximalRange, 0.000001);
}
