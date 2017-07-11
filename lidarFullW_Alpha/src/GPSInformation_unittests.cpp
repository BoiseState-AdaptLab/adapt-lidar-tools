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
* Given the true values of the GPS file we read the pulse file and compare 
* the scanned values to the true values
*/
TEST_F(GPSInformationTest, checkValues){

  // Known GPS information
  long long TruthGpsIndex;
  double TruthGpsTime;
  double TruthXAnchor, TruthYAnchor, TruthZAnchor;
  double TruthXTarget, TruthYTarget, TruthZTarget;
  // double TruthXDeviation, TurthYDeviation, TruthZDeviation;
  double TruthXFirst, TurthYFirst, TruthZFirst;
  double TruthXLast, TruthYLast, TruthZLast;
  unsigned char TruthEdge;
  unsigned char TruthFacet;
  unsigned char TruthScanDirection;
  unsigned char TruthIntensity;
  


  std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";

  /*
   * This section reads the wave and GPS file
   */
  GPSInformation gpsInfo;   // Create a GPS object

  PULSEreadOpener pOpener;  // Create a pulse read opener object
  PULSEreader *pReader;     // Create a pulse reader object
  pOpener.set_file_name(fileNameCorrect.c_str());
  pReader = pOpener.open();
  long long pulseIndex = 0; // Keep track of the index

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
    // TruthXDeviation = 0; 
    // TurthYDeviation = 0; 
    // TruthZDeviation = 0;
    TruthXFirst = 516211.940623; 
    TurthYFirst = 4767921.375796; 
    TruthZFirst = 2093.580011;
    TruthXLast = 516210.627993; 
    TruthYLast = 4767922.675509; 
    TruthZLast = 2084.933649;
    TruthEdge = 0;
    TruthScanDirection = 0;
    TruthFacet = 1;
    TruthIntensity = 0;

    ASSERT_EQ (TruthGpsIndex, gpsInfo.gpsIndex);
    ASSERT_DOUBLE_EQ (TruthGpsTime, gpsInfo.gpsTime);
    ASSERT_DOUBLE_EQ (TruthXAnchor, gpsInfo.xAnchor);
    ASSERT_DOUBLE_EQ (TruthYAnchor, gpsInfo.yAnchor);
    ASSERT_DOUBLE_EQ (TruthZAnchor, gpsInfo.zAnchor);
    ASSERT_DOUBLE_EQ (TruthXTarget, gpsInfo.xTarget);
    ASSERT_DOUBLE_EQ (TruthYTarget, gpsInfo.yTarget);
    ASSERT_DOUBLE_EQ (TruthZTarget, gpsInfo.zTarget);
    ASSERT_NEAR (TruthXFirst, gpsInfo.xFirst, 0.0000002);
    ASSERT_NEAR (TurthYFirst, gpsInfo.yFirst, 0.0000005);
    ASSERT_NEAR (TruthZFirst, gpsInfo.zFirst, 0.0000003);
    ASSERT_NEAR (TruthXLast, gpsInfo.xLast, 0.000002);
    ASSERT_NEAR (TruthYLast, gpsInfo.yLast, 0.000002);
    ASSERT_NEAR (TruthZLast, gpsInfo.zLast, 0.000002);
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
    TruthXTarget = 516302.248; 
    TruthYTarget = 4767831.952; 
    TruthZTarget = 2688.876;
    // TruthXDeviation = 0; 
    // TurthYDeviation = 0; 
    // TruthZDeviation = 0;
    TruthXFirst = 516211.549716; 
    TurthYFirst = 4767921.735659; 
    TruthZFirst = 2093.231562;
    TruthXLast = 516210.233309; 
    TruthYLast = 4767923.03879; 
    TruthZLast = 2084.586298;
    TruthEdge = 0;
    TruthScanDirection = 0;
    TruthFacet = 1;
    TruthIntensity = 0;

    ASSERT_EQ (TruthGpsIndex, gpsInfo.gpsIndex);
    ASSERT_NEAR (TruthGpsTime, gpsInfo.gpsTime, 0.000005);
    ASSERT_DOUBLE_EQ (TruthXAnchor, gpsInfo.xAnchor);
    ASSERT_DOUBLE_EQ (TruthYAnchor, gpsInfo.yAnchor);
    ASSERT_DOUBLE_EQ (TruthZAnchor, gpsInfo.zAnchor);
    ASSERT_DOUBLE_EQ (TruthXTarget, gpsInfo.xTarget);
    ASSERT_DOUBLE_EQ (TruthYTarget, gpsInfo.yTarget);
    ASSERT_DOUBLE_EQ (TruthZTarget, gpsInfo.zTarget);
    ASSERT_NEAR (TruthXFirst, gpsInfo.xFirst, 0.0000005);
    ASSERT_NEAR (TurthYFirst, gpsInfo.yFirst, 0.000001  );
    ASSERT_NEAR (TruthZFirst, gpsInfo.zFirst, 0.0000005);
    ASSERT_NEAR (TruthXLast, gpsInfo.xLast, 0.000002);
    ASSERT_NEAR (TruthYLast, gpsInfo.yLast, 0.000002);
    ASSERT_NEAR (TruthZLast, gpsInfo.zLast, 0.000002);
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
    TruthGpsTime = 66689.303207;
    TruthXAnchor = 516324.56; 
    TruthYAnchor = 4767809.865;
    TruthZAnchor = 2835.406;
    TruthXTarget = 516302.187; 
    TruthYTarget = 4767832.007; 
    TruthZTarget = 2688.894;
    // TruthXDeviation = 0; 
    // TurthYDeviation = 0; 
    // TruthZDeviation = 0;
    TruthXFirst = 516211.240756; 
    TurthYFirst = 4767922.014231; 
    TruthZFirst = 2093.322687;
    TruthXLast = 516209.920748; 
    TruthYLast = 4767923.320612; 
    TruthZLast = 2084.678522;
    TruthEdge = 0;
    TruthScanDirection = 0;
    TruthFacet = 1;
    TruthIntensity = 0;

    ASSERT_EQ (TruthGpsIndex, gpsInfo.gpsIndex);
    ASSERT_DOUBLE_EQ (TruthGpsTime, gpsInfo.gpsTime);
    ASSERT_DOUBLE_EQ (TruthXAnchor, gpsInfo.xAnchor);
    ASSERT_DOUBLE_EQ (TruthYAnchor, gpsInfo.yAnchor);
    ASSERT_DOUBLE_EQ (TruthZAnchor, gpsInfo.zAnchor);
    ASSERT_DOUBLE_EQ (TruthXTarget, gpsInfo.xTarget);
    ASSERT_DOUBLE_EQ (TruthYTarget, gpsInfo.yTarget);
    ASSERT_DOUBLE_EQ (TruthZTarget, gpsInfo.zTarget);
    ASSERT_NEAR (TruthXFirst, gpsInfo.xFirst, 0.000006);
    ASSERT_NEAR (TurthYFirst, gpsInfo.yFirst, 0.000002);
    ASSERT_NEAR (TruthZFirst, gpsInfo.zFirst, 0.000002);
    ASSERT_NEAR (TruthXLast, gpsInfo.xLast, 0.000002);
    ASSERT_NEAR (TruthYLast, gpsInfo.yLast, 0.0000002);
    ASSERT_NEAR (TruthZLast, gpsInfo.zLast, 0.000002);
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
    TruthGpsTime = 66689.30321;
    TruthXAnchor = 516324.561; 
    TruthYAnchor = 4767809.865;
    TruthZAnchor = 2835.406;
    TruthXTarget = 516302.127; 
    TruthYTarget = 4767832.061; 
    TruthZTarget = 2688.912;
    // TruthXDeviation = 0; 
    // TurthYDeviation = 0; 
    // TruthZDeviation = 0;
    TruthXFirst = 516210.910358; 
    TurthYFirst = 4767922.309939; 
    TruthZFirst = 2093.267389;
    TruthXLast = 516209.586749; 
    TurthYLast = 4767923.619501; 
    TruthZLast = 2084.624262;
    TruthEdge = 0;
    TruthScanDirection = 0;
    TruthFacet = 1;
    TruthIntensity = 0;

    ASSERT_EQ (TruthGpsIndex, gpsInfo.gpsIndex);
    ASSERT_DOUBLE_EQ (TruthGpsTime, gpsInfo.gpsTime);
    ASSERT_DOUBLE_EQ (TruthXAnchor, gpsInfo.xAnchor);
    ASSERT_DOUBLE_EQ (TruthYAnchor, gpsInfo.yAnchor);
    ASSERT_DOUBLE_EQ (TruthZAnchor, gpsInfo.zAnchor);
    ASSERT_DOUBLE_EQ (TruthXTarget, gpsInfo.xTarget);
    ASSERT_DOUBLE_EQ (TruthYTarget, gpsInfo.yTarget);
    ASSERT_DOUBLE_EQ (TruthZTarget, gpsInfo.zTarget);
    ASSERT_NEAR (TruthXFirst, gpsInfo.xFirst, 0.000002);
    ASSERT_NEAR (TurthYFirst, gpsInfo.yFirst, 0.0000004);
    ASSERT_NEAR (TruthZFirst, gpsInfo.zFirst, 0.0000002);
    ASSERT_NEAR (TruthXLast, gpsInfo.xLast, 0.0000003);
    ASSERT_NEAR (TurthYLast, gpsInfo.yLast, 0.0000004);
    ASSERT_NEAR (TruthZLast, gpsInfo.zLast, 0.0000003);
    ASSERT_EQ (TruthEdge, gpsInfo.edge);
    ASSERT_EQ (TruthFacet, gpsInfo.facet);
    ASSERT_EQ (TruthScanDirection, gpsInfo.scanDirection);
    ASSERT_EQ (TruthIntensity, gpsInfo.intensity);
  }
}
