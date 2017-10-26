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


  // function to set up space used by all tests
  virtual void SetUp(){
	//populate with setup declarations that are currently inside 'checkValues' test
  }

};

/*
* Given the true values of the GPS file we read the pulse file and compare 
* the scanned values to the true values
*/
TEST_F(GPSInformationTest, checkValues){

  // Known GPS information
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

  /****************************************************************************
  * 
  * Begin index = 0 tests
  * 
  ****************************************************************************/
  if(pReader->read_pulse()) {
    gpsInfo.populateGPS(pReader);
    pulseIndex++;
 
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
    gpsInfo.populateGPS(pReader);
    pulseIndex++;
 
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
    gpsInfo.populateGPS(pReader);
    pulseIndex++;
 
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
    gpsInfo.populateGPS(pReader);
    pulseIndex++;
 
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
    TruthYLast = 4767923.619501; 
    TruthZLast = 2084.624262;
    TruthEdge = 0;
    TruthScanDirection = 0;
    TruthFacet = 1;
    TruthIntensity = 0;

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
    ASSERT_NEAR (TruthYLast, gpsInfo.yLast, 0.0000004);
    ASSERT_NEAR (TruthZLast, gpsInfo.zLast, 0.0000003);
    ASSERT_EQ (TruthEdge, gpsInfo.edge);
    ASSERT_EQ (TruthFacet, gpsInfo.facet);
    ASSERT_EQ (TruthScanDirection, gpsInfo.scanDirection);
    ASSERT_EQ (TruthIntensity, gpsInfo.intensity);
  }

TEST_F(GPSInformationTest, checkXYZ_offset){
    //This test will compare the known values of x,y, and z 
    //offset with the values that are retrieved from the pulse
    //data.
    
    var known_X_offset=1;
    var known_Y_offset=1;
    var known_Z_offset=1;
    var pulse_x_offset=0;
    var pulse_Y_offset=0;
    var pulse_Z_offset=0;

    ASSERT_EQ(known_X_offset, pulse_X_offset);
    ASSERT_EQ(known_Y_offset, pulse_Y_offset);
    ASSERT_EQ(known_Z_offset, pulse_Z_offset);
 }

TEST_F(GPSInformationTest, checkXYZ_scale){
    //This test will compare the known values of x, y, and z
    //scale with the values that are retrieved from the pulse
    //data.
    
    var known_X_scale=.1;
    var known_Y_scale=.1;
    var known_Z_scale=.1;
    var pulse_x_scale=0.0;
    var pulse_Y_scale=0.0;
    var pulse_Z_scale=0.0;

    ASSERT_DOUBLE_EQ(known_X_scale, pulse_X_scale);
    ASSERT_DOUBLE_EQ(known_Y_scale, pulse_Y_scale);
    ASSERT_DOUBLE_EQ(known_Z_scale, pulse_Z_scale);

 }

TEST_F(GPSInformationTest, checkXYZ_realTargetValues){
    //This test will compare the real world target values
    //of x, y, and z that are calculated by the GPSInformation
    //class to the real world target values that are calculated
    //by hand.

    var known_X_target=.1;
    var known_Y_target=.1;
    var known_Z_target=.1;
    var pulse_x_target=0.0;
    var pulse_Y_target=0.0;
    var pulse_Z_target=0.0;

    ASSERT_DOUBLE_EQ(known_X_target, pulse_X_target);
    ASSERT_DOUBLE_EQ(known_Y_target, pulse_Y_target);
    ASSERT_DOUBLE_EQ(known_Z_target, pulse_Z_target);

 }

TEST_F(GPSInformationTest, checkXYZ_realAnchorValues){
    //This test will compare the real world anchor values of
    //x, y, and z that are calculated by the GPSInformation
    //class to the real world anchor values that are calculated 
    //by hand.

    var known_X_anchor=.1;
    var known_Y_anchor=.1;
    var known_Z_anchor=.1;
    var pulse_x_anchor=0.0;
    var pulse_Y_anchor=0.0;
    var pulse_Z_anchor=0.0;

    ASSERT_DOUBLE_EQ(known_X_anchor, pulse_X_anchor);
    ASSERT_DOUBLE_EQ(known_Y_anchor, pulse_Y_anchor);
    ASSERT_DOUBLE_EQ(known_Z_anchor, pulse_Z_anchor);
    
 }

TEST_F(GPSInformationTest, checkXYZ_realFirstValues){
    //This test will compare the real world first values of
    //x, y, and z that are calcuated by the GPSInformation
    //class to the real world first values that are calculated
    //by hand.

    var known_X_first=.1;
    var known_Y_first=.1;
    var known_Z_first=.1;
    var pulse_x_first=0.0;
    var pulse_Y_first=0.0;
    var pulse_Z_first=0.0;

    ASSERT_DOUBLE_EQ(known_X_first, pulse_X_first);
    ASSERT_DOUBLE_EQ(known_Y_first, pulse_Y_first);
    ASSERT_DOUBLE_EQ(known_Z_first, pulse_Z_first);

 }

TEST_F(GPSInformationTest, checkXYZ_realLastValues){
    //This test will compare the real world last values of
    //x, y, and z that are calculated by the GPSInformation 
    //class to the real world last values that are calculated 
    //by hand.

    var known_X_last=.1;
    var known_Y_last=.1;
    var known_Z_last=.1;
    var pulse_x_last=0.0;
    var pulse_Y_last=0.0;
    var pulse_Z_last=0.0;

    ASSERT_DOUBLE_EQ(known_X_last, pulse_X_last);
    ASSERT_DOUBLE_EQ(known_Y_last, pulse_Y_last);
    ASSERT_DOUBLE_EQ(known_Z_last, pulse_Z_last);

 }

TEST_F(GPSInformationTest, checkXYZ_realMinimumValues){
    //This test will compare the real world minimum values of
    //x, y, and z that are calculated by the GPSInformation
    //class to the real world minimum values that are calculated
    //by hand.

    var known_X_minimum=.1;
    var known_Y_minimum=.1;
    var known_Z_minimum=.1;
    var pulse_x_minimum=0.0;
    var pulse_Y_minimum=0.0;
    var pulse_Z_minimum=0.0;

    ASSERT_DOUBLE_EQ(known_X_minimum, pulse_X_minimum);
    ASSERT_DOUBLE_EQ(known_Y_minimum, pulse_Y_minimum);
    ASSERT_DOUBLE_EQ(known_Z_minimum, pulse_Z_minimum);

 }

TEST_F(GPSInformationTest, checkXYZ_realMaximumValues){
    //This test will compare the real world maximum values of
    //x, y, and z that are calculated by the GPSInformation
    //class to the real world maximum values that are calculated
    //by hand.

    var known_X_maximum=.1;
    var known_Y_maximum=.1;
    var known_Z_maximum=.1;
    var pulse_x_maximum=0.0;
    var pulse_Y_maximum=0.0;
    var pulse_Z_maximum=0.0;

    ASSERT_DOUBLE_EQ(known_X_maximum, pulse_X_maximum);
    ASSERT_DOUBLE_EQ(known_Y_maximum, pulse_Y_maximum);
    ASSERT_DOUBLE_EQ(known_Z_maximum, pulse_Z_maximum);

 }
}
