// File name: WaveGPSInformation_unittests.cpp
// Created on: 17-June-2017
// Author: ravi

// This test requres the sample 140823_183115_1_clipped_test.pls file
// and its corresponding wvs file to be in the etc directory to run

#include "FullWaveformIngestion.hpp"
#include "WaveGPSInformation.hpp"
#include "pulsereader.hpp"
#include "pulsewriter.hpp"
#include "gtest/gtest.h"


class WaveGPSInformationTest: public testing::Test{
  protected:

  // function to set up space used by all tests
  virtual void SetUp(){
  //populate with setup declarations that are
  //currently inside 'checkValues' test
  }

};


/****************************************************************************
* 
* Given the known values of the GPS file we read the pulse file and compare
* the scanned values to the known values
* 
****************************************************************************/
TEST_F(WaveGPSInformationTest, checkValues){

  // Known GPS information
  double known_gps_time;
  double known_x_anchor, known_y_anchor, known_z_anchor;
  double known_x_target, known_y_target, known_z_target;
  double known_x_first, known_y_first, known_z_first;
  double known_xLast, known_YLast, known_ZLast;
  unsigned char known_edge;
  unsigned char known_facet;
  unsigned char known_scan_direction;
  unsigned char known_intensity;



  std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";

  /*
   * This section reads the wave and GPS file
   */
  WaveGPSInformation gpsInfo;   // Create a GPS object

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

    known_gps_time = 66689.303202;
    known_x_anchor = 516324.56;
    known_y_anchor = 4767809.865;
    known_z_anchor = 2835.406;
    known_x_target = 516302.312;
    known_y_target = 4767831.894;
    known_z_target = 2688.858;
    known_x_first = 516211.940623;
    known_y_first = 4767921.375796;
    known_z_first = 2093.580011;
    known_xLast = 516210.627993;
    known_YLast = 4767922.675509;
    known_ZLast = 2084.933649;
    known_edge = 0;
    known_scan_direction = 0;
    known_facet = 1;
    known_intensity = 0;

    EXPECT_DOUBLE_EQ (known_gps_time, gpsInfo.gpsTime);
    EXPECT_DOUBLE_EQ (known_x_anchor, gpsInfo.xAnchor);
    EXPECT_DOUBLE_EQ (known_y_anchor, gpsInfo.yAnchor);
    EXPECT_DOUBLE_EQ (known_z_anchor, gpsInfo.zAnchor);
    EXPECT_DOUBLE_EQ (known_x_target, gpsInfo.xTarget);
    EXPECT_DOUBLE_EQ (known_y_target, gpsInfo.yTarget);
    EXPECT_DOUBLE_EQ (known_z_target, gpsInfo.zTarget);
    EXPECT_NEAR (known_x_first, gpsInfo.xFirst, 0.0000002);
    EXPECT_NEAR (known_y_first, gpsInfo.yFirst, 0.0000005);
    EXPECT_NEAR (known_z_first, gpsInfo.zFirst, 0.0000003);
    EXPECT_NEAR (known_xLast, gpsInfo.xLast, 0.000002);
    EXPECT_NEAR (known_YLast, gpsInfo.yLast, 0.000002);
    EXPECT_NEAR (known_ZLast, gpsInfo.zLast, 0.000002);
    EXPECT_EQ (known_edge, gpsInfo.edge);
    EXPECT_EQ (known_facet, gpsInfo.facet);
    EXPECT_EQ (known_scan_direction, gpsInfo.scanDirection);
    EXPECT_EQ (known_intensity, gpsInfo.intensity);
  }
  /****************************************************************************
  *
  * Begin index = 1 tests
  *
  ****************************************************************************/
  if(pReader->read_pulse()) {
    gpsInfo.populateGPS(pReader);
    pulseIndex++;

    known_gps_time = 66689.303202;
    known_x_anchor = 516324.56;
    known_y_anchor = 4767809.865;
    known_z_anchor = 2835.406;
    known_x_target = 516302.248;
    known_y_target = 4767831.952;
    known_z_target = 2688.876;
    known_x_first = 516211.549716;
    known_y_first = 4767921.735659;
    known_z_first = 2093.231562;
    known_xLast = 516210.233309;
    known_YLast = 4767923.03879;
    known_ZLast = 2084.586298;
    known_edge = 0;
    known_scan_direction = 0;
    known_facet = 1;
    known_intensity = 0;

    EXPECT_NEAR (known_gps_time, gpsInfo.gpsTime, 0.000005);
    EXPECT_DOUBLE_EQ (known_x_anchor, gpsInfo.xAnchor);
    EXPECT_DOUBLE_EQ (known_y_anchor, gpsInfo.yAnchor);
    EXPECT_DOUBLE_EQ (known_z_anchor, gpsInfo.zAnchor);
    EXPECT_DOUBLE_EQ (known_x_target, gpsInfo.xTarget);
    EXPECT_DOUBLE_EQ (known_y_target, gpsInfo.yTarget);
    EXPECT_DOUBLE_EQ (known_z_target, gpsInfo.zTarget);
    EXPECT_NEAR (known_x_first, gpsInfo.xFirst, 0.0000005);
    EXPECT_NEAR (known_y_first, gpsInfo.yFirst, 0.000001  );
    EXPECT_NEAR (known_z_first, gpsInfo.zFirst, 0.0000005);
    EXPECT_NEAR (known_xLast, gpsInfo.xLast, 0.000002);
    EXPECT_NEAR (known_YLast, gpsInfo.yLast, 0.000002);
    EXPECT_NEAR (known_ZLast, gpsInfo.zLast, 0.000002);
    EXPECT_EQ (known_edge, gpsInfo.edge);
    EXPECT_EQ (known_facet, gpsInfo.facet);
    EXPECT_EQ (known_scan_direction, gpsInfo.scanDirection);
    EXPECT_EQ (known_intensity, gpsInfo.intensity);
}
  /****************************************************************************
  *
  * Begin index = 2 tests
  *
  ****************************************************************************/
  if(pReader->read_pulse()) {
    gpsInfo.populateGPS(pReader);
    pulseIndex++;

    known_gps_time = 66689.303207;
    known_x_anchor = 516324.56;
    known_y_anchor = 4767809.865;
    known_z_anchor = 2835.406;
    known_x_target = 516302.187;
    known_y_target = 4767832.007;
    known_z_target = 2688.894;
    known_x_first = 516211.240756;
    known_y_first = 4767922.014231;
    known_z_first = 2093.322687;
    known_xLast = 516209.920748;
    known_YLast = 4767923.320612;
    known_ZLast = 2084.678522;
    known_edge = 0;
    known_scan_direction = 0;
    known_facet = 1;
    known_intensity = 0;

    EXPECT_DOUBLE_EQ (known_gps_time, gpsInfo.gpsTime);
    EXPECT_DOUBLE_EQ (known_x_anchor, gpsInfo.xAnchor);
    EXPECT_DOUBLE_EQ (known_y_anchor, gpsInfo.yAnchor);
    EXPECT_DOUBLE_EQ (known_z_anchor, gpsInfo.zAnchor);
    EXPECT_DOUBLE_EQ (known_x_target, gpsInfo.xTarget);
    EXPECT_DOUBLE_EQ (known_y_target, gpsInfo.yTarget);
    EXPECT_DOUBLE_EQ (known_z_target, gpsInfo.zTarget);
    EXPECT_NEAR (known_x_first, gpsInfo.xFirst, 0.000006);
    EXPECT_NEAR (known_y_first, gpsInfo.yFirst, 0.000002);
    EXPECT_NEAR (known_z_first, gpsInfo.zFirst, 0.000002);
    EXPECT_NEAR (known_xLast, gpsInfo.xLast, 0.000002);
    EXPECT_NEAR (known_YLast, gpsInfo.yLast, 0.0000002);
    EXPECT_NEAR (known_ZLast, gpsInfo.zLast, 0.000002);
    EXPECT_EQ (known_edge, gpsInfo.edge);
    EXPECT_EQ (known_facet, gpsInfo.facet);
    EXPECT_EQ (known_scan_direction, gpsInfo.scanDirection);
    EXPECT_EQ (known_intensity, gpsInfo.intensity);
  }
  /****************************************************************************
  *
  * Begin index = 3 tests
  *
  ****************************************************************************/
  if(pReader->read_pulse()) {
    gpsInfo.populateGPS(pReader);
    pulseIndex++;

    known_gps_time = 66689.30321;
    known_x_anchor = 516324.561;
    known_y_anchor = 4767809.865;
    known_z_anchor = 2835.406;
    known_x_target = 516302.127;
    known_y_target = 4767832.061;
    known_z_target = 2688.912;
    known_x_first = 516210.910358;
    known_y_first = 4767922.309939;
    known_z_first = 2093.267389;
    known_xLast = 516209.586749;
    known_YLast = 4767923.619501;
    known_ZLast = 2084.624262;
    known_edge = 0;
    known_scan_direction = 0;
    known_facet = 1;
    known_intensity = 0;

    EXPECT_DOUBLE_EQ (known_gps_time, gpsInfo.gpsTime);
    EXPECT_DOUBLE_EQ (known_x_anchor, gpsInfo.xAnchor);
    EXPECT_DOUBLE_EQ (known_y_anchor, gpsInfo.yAnchor);
    EXPECT_DOUBLE_EQ (known_z_anchor, gpsInfo.zAnchor);
    EXPECT_DOUBLE_EQ (known_x_target, gpsInfo.xTarget);
    EXPECT_DOUBLE_EQ (known_y_target, gpsInfo.yTarget);
    EXPECT_DOUBLE_EQ (known_z_target, gpsInfo.zTarget);
    EXPECT_NEAR (known_x_first, gpsInfo.xFirst, 0.000002);
    EXPECT_NEAR (known_y_first, gpsInfo.yFirst, 0.0000004);
    EXPECT_NEAR (known_z_first, gpsInfo.zFirst, 0.0000002);
    EXPECT_NEAR (known_xLast, gpsInfo.xLast, 0.0000003);
    EXPECT_NEAR (known_YLast, gpsInfo.yLast, 0.0000004);
    EXPECT_NEAR (known_ZLast, gpsInfo.zLast, 0.0000003);
    EXPECT_EQ (known_edge, gpsInfo.edge);
    EXPECT_EQ (known_facet, gpsInfo.facet);
    EXPECT_EQ (known_scan_direction, gpsInfo.scanDirection);
    EXPECT_EQ (known_intensity, gpsInfo.intensity);
  }
}

TEST_F(WaveGPSInformationTest, checkXYZ_offset){
    //This test will compare the known values of x,y, and z
    //offset with the values that are retrieved from the pulse
    //data.
    std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";

    WaveGPSInformation gpsInfo;
    PULSEreadOpener pOpener;  // Create a pulse read opener object
    PULSEreader *pReader;     // Create a pulse reader object
    pOpener.set_file_name(fileNameCorrect.c_str());
    pReader = pOpener.open();

    gpsInfo.populateGPS(pReader);

    double known_x_offset=.1;
    double known_Y_offset=.1;
    double known_Z_offset=.1;

    EXPECT_DOUBLE_EQ(known_x_offset, gpsInfo.xOffset);
    EXPECT_DOUBLE_EQ(known_Y_offset, gpsInfo.yOffset);
    EXPECT_DOUBLE_EQ(known_Z_offset, gpsInfo.zOffset);
 }

TEST_F(WaveGPSInformationTest, checkXYZ_scale){
    //This test will compare the known values of x, y, and z
    //scale with the values that are retrieved from the pulse
    //data.

    std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";

    WaveGPSInformation gpsInfo;
    PULSEreadOpener pOpener;  // Create a pulse read opener object
    PULSEreader *pReader;     // Create a pulse reader object
    pOpener.set_file_name(fileNameCorrect.c_str());
    pReader = pOpener.open();

    gpsInfo.populateGPS(pReader);

    double known_x_scale=.1;
    double known_Y_scale=.1;
    double known_Z_scale=.1;

    EXPECT_DOUBLE_EQ(known_x_scale, gpsInfo.xScale);
    EXPECT_DOUBLE_EQ(known_Y_scale, gpsInfo.yScale);
    EXPECT_DOUBLE_EQ(known_Z_scale, gpsInfo.zScale);

 }

TEST_F(WaveGPSInformationTest, checkXYZ_realTargetValues){
    //This test will compare the real world target values
    //of x, y, and z that are calculated by the GPSInformation
    //class to the real world target values that are calculated
    //by hand.

    std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";

    WaveGPSInformation gpsInfo;
    PULSEreadOpener pOpener;  // Create a pulse read opener object
    PULSEreader *pReader;     // Create a pulse reader object
    pOpener.set_file_name(fileNameCorrect.c_str());
    pReader = pOpener.open();

    gpsInfo.populateGPS(pReader);

    double known_x_target=.1;
    double known_Y_target=.1;
    double known_Z_target=.1;

    EXPECT_DOUBLE_EQ(known_x_target, gpsInfo.xRealTarget);
    EXPECT_DOUBLE_EQ(known_Y_target, gpsInfo.yRealTarget);
    EXPECT_DOUBLE_EQ(known_Z_target, gpsInfo.zRealTarget);

 }

TEST_F(WaveGPSInformationTest, checkXYZ_realAnchorValues){
    //This test will compare the real world anchor values of
    //x, y, and z that are calculated by the GPSInformation
    //class to the real world anchor values that are calculated
    //by hand.

    std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";

    WaveGPSInformation gpsInfo;
    PULSEreadOpener pOpener;  // Create a pulse read opener object
    PULSEreader *pReader;     // Create a pulse reader object
    pOpener.set_file_name(fileNameCorrect.c_str());
    pReader = pOpener.open();

    gpsInfo.populateGPS(pReader);

    double known_x_anchor=.1;
    double known_Y_anchor=.1;
    double known_Z_anchor=.1;

    EXPECT_DOUBLE_EQ(known_x_anchor, gpsInfo.xRealAnchor);
    EXPECT_DOUBLE_EQ(known_Y_anchor, gpsInfo.yRealAnchor);
    EXPECT_DOUBLE_EQ(known_Z_anchor, gpsInfo.zRealAnchor);

 }

TEST_F(WaveGPSInformationTest, checkXYZ_realFirstValues){
    //This test will compare the real world first values of
    //x, y, and z that are calcuated by the GPSInformation
    //class to the real world first values that are calculated
    //by hand.

    std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";

    WaveGPSInformation gpsInfo;
    PULSEreadOpener pOpener;  // Create a pulse read opener object
    PULSEreader *pReader;     // Create a pulse reader object
    pOpener.set_file_name(fileNameCorrect.c_str());
    pReader = pOpener.open();

    gpsInfo.populateGPS(pReader);

    double known_x_first=.1;
    double known_Y_first=.1;
    double known_Z_first=.1;

    EXPECT_DOUBLE_EQ(known_x_first, gpsInfo.xRealFirst);
    EXPECT_DOUBLE_EQ(known_Y_first, gpsInfo.yRealFirst);
    EXPECT_DOUBLE_EQ(known_Z_first, gpsInfo.zRealFirst);

 }

TEST_F(WaveGPSInformationTest, checkXYZ_realLastValues){
    //This test will compare the real world last values of
    //x, y, and z that are calculated by the GPSInformation
    //class to the real world last values that are calculated
    //by hand.

    std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";

    WaveGPSInformation gpsInfo;
    PULSEreadOpener pOpener;  // Create a pulse read opener object
    PULSEreader *pReader;     // Create a pulse reader object
    pOpener.set_file_name(fileNameCorrect.c_str());
    pReader = pOpener.open();

    gpsInfo.populateGPS(pReader);

    double known_x_last=.1;
    double known_Y_last=.1;
    double known_Z_last=.1;

    EXPECT_DOUBLE_EQ(known_x_last, gpsInfo.xRealLast);
    EXPECT_DOUBLE_EQ(known_Y_last, gpsInfo.yRealLast);
    EXPECT_DOUBLE_EQ(known_Z_last, gpsInfo.zRealLast);

 }

TEST_F(WaveGPSInformationTest, checkXYZ_realMinimumValues){
    //This test will compare the real world minimum values of
    //x, y, and z that are calculated by the GPSInformation
    //class to the real world minimum values that are calculated
    //by hand.

    std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";

    WaveGPSInformation gpsInfo;
    PULSEreadOpener pOpener;  // Create a pulse read opener object
    PULSEreader *pReader;     // Create a pulse reader object
    pOpener.set_file_name(fileNameCorrect.c_str());
    pReader = pOpener.open();

    gpsInfo.populateGPS(pReader);

    double known_x_minimum=.1;
    double known_Y_minimum=.1;
    double known_Z_minimum=.1;

    EXPECT_DOUBLE_EQ(known_x_minimum, gpsInfo.xRealMin);
    EXPECT_DOUBLE_EQ(known_Y_minimum, gpsInfo.yRealMin);
    EXPECT_DOUBLE_EQ(known_Z_minimum, gpsInfo.zRealMin);

 }

TEST_F(WaveGPSInformationTest, checkXYZ_realMaximumValues){
    //This test will compare the real world maximum values of
    //x, y, and z that are calculated by the GPSInformation
    //class to the real world maximum values that are calculated
    //by hand.

    std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";

    WaveGPSInformation gpsInfo;
    PULSEreadOpener pOpener;  // Create a pulse read opener object
    PULSEreader *pReader;     // Create a pulse reader object
    pOpener.set_file_name(fileNameCorrect.c_str());
    pReader = pOpener.open();

    gpsInfo.populateGPS(pReader);

    double known_x_maximum=.1;
    double known_Y_maximum=.1;
    double known_Z_maximum=.1;

    EXPECT_DOUBLE_EQ(known_x_maximum, gpsInfo.xRealMax);
    EXPECT_DOUBLE_EQ(known_Y_maximum, gpsInfo.yRealMax);
    EXPECT_DOUBLE_EQ(known_Z_maximum, gpsInfo.zRealMax);

 }

