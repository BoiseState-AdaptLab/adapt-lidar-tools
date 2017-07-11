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
  


  std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";

  /*
   * This section reads the wave and GPS file
   */
  //FullWaveformIngestion ingester;
  //long long int noOfPulses = ingester.getNumberOfPulses(fileNameCorrect);
  GPSInformation gpsInfo;

  PULSEreadOpener pOpener;
  PULSEreader *pReader;
  pOpener.set_file_name(fileNameCorrect.c_str());
  pReader = pOpener.open();
  long long pulseIndex = 0;

  if(pReader->read_pulse()) {
    gpsInfo.populateGPS(pReader, pulseIndex);
    pulseIndex++;
 
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

    ASSERT_EQ (TruthGpsIndex, gpsInfo.gpsIndex);
    ASSERT_DOUBLE_EQ (TruthGpsTime, gpsInfo.xAnchor);
    ASSERT_NEAR (TruthXAnchor, gpsInfo.xAnchor,0.0000005);
    ASSERT_NEAR (TruthYAnchor, gpsInfo.yAnchor,0.0000005);
    ASSERT_DOUBLE_EQ (TruthZAnchor, gpsInfo.zAnchor);
    ASSERT_DOUBLE_EQ (TruthXTarget, gpsInfo.xTarget);
    ASSERT_NEAR (TruthYTarget, gpsInfo.yTarget, 0.000001);
    ASSERT_NEAR (TruthZTarget, gpsInfo.zTarget, 0.0000001);
    ASSERT_NEAR (TruthXFirst, gpsInfo.xFirst, 0.0000005);
    ASSERT_DOUBLE_EQ (TurthYFirst, gpsInfo.yFirst);
    ASSERT_DOUBLE_EQ (TruthZFirst, gpsInfo.zFirst);
    ASSERT_NEAR (TruthXLast, gpsInfo.xLast, 0.01);
    ASSERT_NEAR (TurthYLast, gpsInfo.yLast, 0.00001);
    ASSERT_NEAR (TruthZLast, gpsInfo.zLast, 0.000001);
    ASSERT_EQ (TruthEdge, gpsInfo.edge);
    ASSERT_EQ (TruthFacet, gpsInfo.facet);
    ASSERT_EQ (TruthScanDirection, gpsInfo.scanDirection);
    ASSERT_EQ (TruthIntensity, gpsInfo.intensity);
  }
  /****************************************************************************
  * 
  * Begin index = 1 tests
  * 
  ****************************************************************************/
  if(pReader->read_pulse()) {
    gpsInfo.populateGPS(pReader, pulseIndex);
    pulseIndex++;
 
  TruthGpsIndex = 1;
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

  ASSERT_EQ (TruthGpsIndex, gpsInfo.gpsIndex);
  ASSERT_DOUBLE_EQ (TruthGpsTime, gpsInfo.xAnchor);
  ASSERT_NEAR (TruthXAnchor, gpsInfo.xAnchor,0.0000005);
  ASSERT_NEAR (TruthYAnchor, gpsInfo.yAnchor,0.0000005);
  ASSERT_DOUBLE_EQ (TruthZAnchor, gpsInfo.zAnchor);
  ASSERT_DOUBLE_EQ (TruthXTarget, gpsInfo.xTarget);
  ASSERT_NEAR (TruthYTarget, gpsInfo.yTarget, 0.000001);
  ASSERT_NEAR (TruthZTarget, gpsInfo.zTarget, 0.0000001);
  ASSERT_NEAR (TruthXFirst, gpsInfo.xFirst, 0.0000005);
  ASSERT_DOUBLE_EQ (TurthYFirst, gpsInfo.yFirst);
  ASSERT_DOUBLE_EQ (TruthZFirst, gpsInfo.zFirst);
  ASSERT_NEAR (TruthXLast, gpsInfo.xLast, 0.01);
  ASSERT_NEAR (TurthYLast, gpsInfo.yLast, 0.00001);
  ASSERT_NEAR (TruthZLast, gpsInfo.zLast, 0.000001);
  ASSERT_EQ (TruthEdge, gpsInfo.edge);
  ASSERT_EQ (TruthFacet, gpsInfo.facet);
  ASSERT_EQ (TruthScanDirection, gpsInfo.scanDirection);
  ASSERT_EQ (TruthIntensity, gpsInfo.intensity);
}
  /****************************************************************************
  * 
  * Begin index = 2 tests
  * 
  ****************************************************************************/
  if(pReader->read_pulse()) {
    gpsInfo.populateGPS(pReader, pulseIndex);
    pulseIndex++;
 
  TruthGpsIndex = 2;
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

  ASSERT_EQ (TruthGpsIndex, gpsInfo.gpsIndex);
  ASSERT_DOUBLE_EQ (TruthGpsTime, gpsInfo.xAnchor);
  ASSERT_NEAR (TruthXAnchor, gpsInfo.xAnchor,0.0000005);
  ASSERT_NEAR (TruthYAnchor, gpsInfo.yAnchor,0.0000005);
  ASSERT_DOUBLE_EQ (TruthZAnchor, gpsInfo.zAnchor);
  ASSERT_DOUBLE_EQ (TruthXTarget, gpsInfo.xTarget);
  ASSERT_NEAR (TruthYTarget, gpsInfo.yTarget, 0.000001);
  ASSERT_NEAR (TruthZTarget, gpsInfo.zTarget, 0.0000001);
  ASSERT_NEAR (TruthXFirst, gpsInfo.xFirst, 0.0000005);
  ASSERT_DOUBLE_EQ (TurthYFirst, gpsInfo.yFirst);
  ASSERT_DOUBLE_EQ (TruthZFirst, gpsInfo.zFirst);
  ASSERT_NEAR (TruthXLast, gpsInfo.xLast, 0.01);
  ASSERT_NEAR (TurthYLast, gpsInfo.yLast, 0.00001);
  ASSERT_NEAR (TruthZLast, gpsInfo.zLast, 0.000001);
  ASSERT_EQ (TruthEdge, gpsInfo.edge);
  ASSERT_EQ (TruthFacet, gpsInfo.facet);
  ASSERT_EQ (TruthScanDirection, gpsInfo.scanDirection);
  ASSERT_EQ (TruthIntensity, gpsInfo.intensity);
}
  /****************************************************************************
  * 
  * Begin index = 3 tests
  * 
  ****************************************************************************/
  if(pReader->read_pulse()) {
    gpsInfo.populateGPS(pReader, pulseIndex);
    pulseIndex++;
 
  TruthGpsIndex = 3;
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

  ASSERT_EQ (TruthGpsIndex, gpsInfo.gpsIndex);
  ASSERT_DOUBLE_EQ (TruthGpsTime, gpsInfo.xAnchor);
  ASSERT_NEAR (TruthXAnchor, gpsInfo.xAnchor,0.0000005);
  ASSERT_NEAR (TruthYAnchor, gpsInfo.yAnchor,0.0000005);
  ASSERT_DOUBLE_EQ (TruthZAnchor, gpsInfo.zAnchor);
  ASSERT_DOUBLE_EQ (TruthXTarget, gpsInfo.xTarget);
  ASSERT_NEAR (TruthYTarget, gpsInfo.yTarget, 0.000001);
  ASSERT_NEAR (TruthZTarget, gpsInfo.zTarget, 0.0000001);
  ASSERT_NEAR (TruthXFirst, gpsInfo.xFirst, 0.0000005);
  ASSERT_DOUBLE_EQ (TurthYFirst, gpsInfo.yFirst);
  ASSERT_DOUBLE_EQ (TruthZFirst, gpsInfo.zFirst);
  ASSERT_NEAR (TruthXLast, gpsInfo.xLast, 0.01);
  ASSERT_NEAR (TurthYLast, gpsInfo.yLast, 0.00001);
  ASSERT_NEAR (TruthZLast, gpsInfo.zLast, 0.000001);
  ASSERT_EQ (TruthEdge, gpsInfo.edge);
  ASSERT_EQ (TruthFacet, gpsInfo.facet);
  ASSERT_EQ (TruthScanDirection, gpsInfo.scanDirection);
  ASSERT_EQ (TruthIntensity, gpsInfo.intensity);
}
}
