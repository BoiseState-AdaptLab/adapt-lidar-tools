/*
 * File name: GPSInformation_unittests.cpp
 * Created on: 17-June-2017
 * Author: ravi
 *
 * This test requres the sample 140823_183115_1_clipped_test.pls file 
 * and its corresponding wvs file to be in the etc directory to run
 *
 */

#include "gtest/gtest.h"
#include "FullWaveformIngestion.hpp"
#include "GPSInformation.hpp"
#include "pulsereader.hpp"
#include "pulsewriter.hpp"


class GPSInformationTest: public testing::Test{
  protected:

  GPSInformation gps;

  // function to set up space used by all tests
  virtual void SetUp(){
  }

};

/*
* Given the true values of the actual anchor, target and deviation,
* we read the oulse file and compare the scanned values to the true values
*/
TEST_F(GPSInformationTest, checkValues){

  // Known GPS information
  long long TruthGpsIndex;
  double TruthGpsTime;
  double TruthXAnchor, TruthYAnchor, TruthZAnchor;
  double TruthXTarget, TruthYTarget, TruthZTarget;
  double TruthXDeviation, TurthYDeviation, TruthZDeviation;
  double TruthXFirst, TurthYFirst, TruthZFirst;
  double TruthXLast, TurthYLast, TruthZLast;
  unsigned char TruthEdge;
  unsigned char TruthFacet;
  unsigned char TruthScanDirection;
  unsigned char TruthIntensity;
  
  std::string fileName = "bin/140823_183115_1_clipped_test.pls";
  ASSERT_ANY_THROW (scannerInfo.setScannerInformation(fileName));

  std::string fileName1 = "etc/140823_183115_1_clipped_test.dat";
  ASSERT_ANY_THROW (scannerInfo.setScannerInformation(fileName1));

  std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";
  ASSERT_NO_THROW (scannerInfo.setScannerInformation(fileNameCorrect));


  /*
   * This section reads the wave and GPS file
   */
  FullWaveformIngestion ingester;
  long long int noOfPulses = ingester.getNumberOfPulses(fileName);
  GPSInformation gpsInfo[noOfPulses];

  PULSEreadOpener pOpener;
  PULSEreader *pReader;
  pOpener.set_file_name(fileName.c_str());
  pReader = pOpener.open();
  long long pulseIndex = 0;

  pReader->seek(0);
  while(pReader->read_pulse()) {
    gpsInfo[pulseIndex].populateGPS(pReader, pulseIndex);

    pulseIndex++;
  }
 
  /****************************************************************************
  * 
  * Begin index = 0 tests
  * 
  ****************************************************************************/
  TruthGpsIndex = 0;
  TruthGpsTime = 66689.303202;
  TruthXAnchor = 516324.56; 
  TruthYAnchor = 4767809.865;
  TruthZAnchor = 2835.406;
  TruthXTarget = 516302.312; 
  TruthYTarget = 4767831.894; 
  TruthZTarget = 2688.858;
  TruthXDeviation = 0; 
  TurthYDeviation = 0; 
  TruthZDeviation = 0;
  TruthXFirst = 516211.940623; 
  TurthYFirst = 4767921.375796; 
  TruthZFirst = 2093.580011;
  TruthXLast = 516210.627993; 
  TurthYLast = 4767922.675509; 
  TruthZLast = 2084.933649;
  TruthEdge = 0;
  TruthFacet = 0;
  TruthScanDirection = 1;
  TruthIntensity = 0;

  ASSERT_EQ (TruthGpsIndex, gpsInfo[0].gpsIndex);
  ASSERT_DOUBLE_EQ (TruthGpsTime, gpsInfo[0].xAnchor);
  ASSERT_NEAR (TruthXAnchor, gpsInfo[0].xAnchor,0.0000005);
  ASSERT_NEAR (TruthYAnchor, gpsInfo[0].yAnchor,0.0000005);
  ASSERT_DOUBLE_EQ (TruthZAnchor, gpsInfo[0].zAnchor);
  ASSERT_DOUBLE_EQ (TruthXTarget, gpsInfo[0].xTarget);
  ASSERT_NEAR (TruthYTarget, gpsInfo[0].yTarget, 0.000001);
  ASSERT_NEAR (TruthZTarget, gpsInfo[0].zTarget, 0.0000001);
  ASSERT_NEAR (TruthXFirst, gpsInfo[0].xFirst, 0.0000005);
  ASSERT_DOUBLE_EQ (TurthYFirst, gpsInfo[0].yFirst);
  ASSERT_DOUBLE_EQ (TruthZFirst, gpsInfo[0].zFirst);
  ASSERT_NEAR (TruthXLast, gpsInfo[0].xLast, 0.01);
  ASSERT_NEAR (TurthYLast, gpsInfo[0].yLast, 0.00001);
  ASSERT_NEAR (TruthZLast, gpsInfo[0].zLast, 0.000001);
  ASSERT_EQ (TruthEdge, gpsInfo[0].edge);
  ASSERT_EQ (TruthFacet, gpsInfo[0].facet);
  ASSERT_EQ (TruthScanDirection, gpsInfo[0].scanDirection);
  ASSERT_EQ (TruthIntensity, gpsInfo[0].intensity);

  /****************************************************************************
  * 
  * Begin index = 1 tests
  * 
  ****************************************************************************/
  TruthGpsIndex = 0;
  TruthGpsTime = 66689.303202;
  TruthXAnchor = 516324.56; 
  TruthYAnchor = 4767809.865;
  TruthZAnchor = 2835.406;
  TruthXTarget = 516302.312; 
  TruthYTarget = 4767831.894; 
  TruthZTarget = 2688.858;
  TruthXDeviation = 0; 
  TurthYDeviation = 0; 
  TruthZDeviation = 0;
  TruthXFirst = 516211.940623; 
  TurthYFirst = 4767921.375796; 
  TruthZFirst = 2093.580011;
  TruthXLast = 516210.627993; 
  TurthYLast = 4767922.675509; 
  TruthZLast = 2084.933649;
  TruthEdge = 0;
  TruthFacet = 0;
  TruthScanDirection = 1;
  TruthIntensity = 0;

  ASSERT_EQ (TruthGpsIndex, gpsInfo[1].gpsIndex);
  ASSERT_DOUBLE_EQ (TruthGpsTime, gpsInfo[1].xAnchor);
  ASSERT_NEAR (TruthXAnchor, gpsInfo[1].xAnchor,0.0000005);
  ASSERT_NEAR (TruthYAnchor, gpsInfo[1].yAnchor,0.0000005);
  ASSERT_DOUBLE_EQ (TruthZAnchor, gpsInfo[1].zAnchor);
  ASSERT_DOUBLE_EQ (TruthXTarget, gpsInfo[1].xTarget);
  ASSERT_NEAR (TruthYTarget, gpsInfo[1].yTarget, 0.000001);
  ASSERT_NEAR (TruthZTarget, gpsInfo[1].zTarget, 0.0000001);
  ASSERT_NEAR (TruthXFirst, gpsInfo[1].xFirst, 0.0000005);
  ASSERT_DOUBLE_EQ (TurthYFirst, gpsInfo[1].yFirst);
  ASSERT_DOUBLE_EQ (TruthZFirst, gpsInfo[1].zFirst);
  ASSERT_NEAR (TruthXLast, gpsInfo[1].xLast, 0.01);
  ASSERT_NEAR (TurthYLast, gpsInfo[1].yLast, 0.00001);
  ASSERT_NEAR (TruthZLast, gpsInfo[1].zLast, 0.000001);
  ASSERT_EQ (TruthEdge, gpsInfo[1].edge);
  ASSERT_EQ (TruthFacet, gpsInfo[1].facet);
  ASSERT_EQ (TruthScanDirection, gpsInfo[1].scanDirection);
  ASSERT_EQ (TruthIntensity, gpsInfo[1].intensity);

  /****************************************************************************
  * 
  * Begin index = 2 tests
  * 
  ****************************************************************************/
  TruthGpsIndex = 0;
  TruthGpsTime = 66689.303202;
  TruthXAnchor = 516324.56; 
  TruthYAnchor = 4767809.865;
  TruthZAnchor = 2835.406;
  TruthXTarget = 516302.312; 
  TruthYTarget = 4767831.894; 
  TruthZTarget = 2688.858;
  TruthXDeviation = 0; 
  TurthYDeviation = 0; 
  TruthZDeviation = 0;
  TruthXFirst = 516211.940623; 
  TurthYFirst = 4767921.375796; 
  TruthZFirst = 2093.580011;
  TruthXLast = 516210.627993; 
  TurthYLast = 4767922.675509; 
  TruthZLast = 2084.933649;
  TruthEdge = 0;
  TruthFacet = 0;
  TruthScanDirection = 1;
  TruthIntensity = 0;

  ASSERT_EQ (TruthGpsIndex, gpsInfo[2].gpsIndex);
  ASSERT_DOUBLE_EQ (TruthGpsTime, gpsInfo[2].xAnchor);
  ASSERT_NEAR (TruthXAnchor, gpsInfo[2].xAnchor,0.0000005);
  ASSERT_NEAR (TruthYAnchor, gpsInfo[2].yAnchor,0.0000005);
  ASSERT_DOUBLE_EQ (TruthZAnchor, gpsInfo[2].zAnchor);
  ASSERT_DOUBLE_EQ (TruthXTarget, gpsInfo[2].xTarget);
  ASSERT_NEAR (TruthYTarget, gpsInfo[2].yTarget, 0.000001);
  ASSERT_NEAR (TruthZTarget, gpsInfo[2].zTarget, 0.0000001);
  ASSERT_NEAR (TruthXFirst, gpsInfo[2].xFirst, 0.0000005);
  ASSERT_DOUBLE_EQ (TurthYFirst, gpsInfo[2].yFirst);
  ASSERT_DOUBLE_EQ (TruthZFirst, gpsInfo[2].zFirst);
  ASSERT_NEAR (TruthXLast, gpsInfo[2].xLast, 0.01);
  ASSERT_NEAR (TurthYLast, gpsInfo[2].yLast, 0.00001);
  ASSERT_NEAR (TruthZLast, gpsInfo[2].zLast, 0.000001);
  ASSERT_EQ (TruthEdge, gpsInfo[2].edge);
  ASSERT_EQ (TruthFacet, gpsInfo[2].facet);
  ASSERT_EQ (TruthScanDirection, gpsInfo[2].scanDirection);
  ASSERT_EQ (TruthIntensity, gpsInfo[2].intensity);

  /****************************************************************************
  * 
  * Begin index = 3 tests
  * 
  ****************************************************************************/
  TruthGpsIndex = 0;
  TruthGpsTime = 66689.303202;
  TruthXAnchor = 516324.56; 
  TruthYAnchor = 4767809.865;
  TruthZAnchor = 2835.406;
  TruthXTarget = 516302.312; 
  TruthYTarget = 4767831.894; 
  TruthZTarget = 2688.858;
  TruthXDeviation = 0; 
  TurthYDeviation = 0; 
  TruthZDeviation = 0;
  TruthXFirst = 516211.940623; 
  TurthYFirst = 4767921.375796; 
  TruthZFirst = 2093.580011;
  TruthXLast = 516210.627993; 
  TurthYLast = 4767922.675509; 
  TruthZLast = 2084.933649;
  TruthEdge = 0;
  TruthFacet = 0;
  TruthScanDirection = 1;
  TruthIntensity = 0;

  ASSERT_EQ (TruthGpsIndex, gpsInfo[3].gpsIndex);
  ASSERT_DOUBLE_EQ (TruthGpsTime, gpsInfo[3].xAnchor);
  ASSERT_NEAR (TruthXAnchor, gpsInfo[3].xAnchor,0.0000005);
  ASSERT_NEAR (TruthYAnchor, gpsInfo[3].yAnchor,0.0000005);
  ASSERT_DOUBLE_EQ (TruthZAnchor, gpsInfo[3].zAnchor);
  ASSERT_DOUBLE_EQ (TruthXTarget, gpsInfo[3].xTarget);
  ASSERT_NEAR (TruthYTarget, gpsInfo[3].yTarget, 0.000001);
  ASSERT_NEAR (TruthZTarget, gpsInfo[3].zTarget, 0.0000001);
  ASSERT_NEAR (TruthXFirst, gpsInfo[3].xFirst, 0.0000005);
  ASSERT_DOUBLE_EQ (TurthYFirst, gpsInfo[3].yFirst);
  ASSERT_DOUBLE_EQ (TruthZFirst, gpsInfo[3].zFirst);
  ASSERT_NEAR (TruthXLast, gpsInfo[3].xLast, 0.01);
  ASSERT_NEAR (TurthYLast, gpsInfo[3].yLast, 0.00001);
  ASSERT_NEAR (TruthZLast, gpsInfo[3].zLast, 0.000001);
  ASSERT_EQ (TruthEdge, gpsInfo[3].edge);
  ASSERT_EQ (TruthFacet, gpsInfo[3].facet);
  ASSERT_EQ (TruthScanDirection, gpsInfo[3].scanDirection);
  ASSERT_EQ (TruthIntensity, gpsInfo[3].intensity);
}
