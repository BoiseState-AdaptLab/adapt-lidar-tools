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
  double known_x_last, known_Y_last, known_Z_last;
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
    known_x_last = 516210.627993;
    known_Y_last = 4767922.675509;
    known_Z_last = 2084.933649;
    known_edge = 0;
    known_scan_direction = 0;
    known_facet = 1;
    known_intensity = 0;

    EXPECT_DOUBLE_EQ (known_gps_time, gpsInfo.gpsTime);
    EXPECT_DOUBLE_EQ (known_x_anchor, gpsInfo.x_anchor);
    EXPECT_DOUBLE_EQ (known_y_anchor, gpsInfo.y_anchor);
    EXPECT_DOUBLE_EQ (known_z_anchor, gpsInfo.z_anchor);
    EXPECT_DOUBLE_EQ (known_x_target, gpsInfo.x_target);
    EXPECT_DOUBLE_EQ (known_y_target, gpsInfo.y_target);
    EXPECT_DOUBLE_EQ (known_z_target, gpsInfo.z_target);
    EXPECT_NEAR (known_x_first, gpsInfo.x_first, 0.0000002);
    EXPECT_NEAR (known_y_first, gpsInfo.y_first, 0.0000005);
    EXPECT_NEAR (known_z_first, gpsInfo.z_first, 0.0000003);
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
    known_x_last = 516210.233309;
    known_Y_last = 4767923.03879;
    known_Z_last = 2084.586298;
    known_edge = 0;
    known_scan_direction = 0;
    known_facet = 1;
    known_intensity = 0;

    EXPECT_NEAR (known_gps_time, gpsInfo.gpsTime, 0.000005);
    EXPECT_DOUBLE_EQ (known_x_anchor, gpsInfo.x_anchor);
    EXPECT_DOUBLE_EQ (known_y_anchor, gpsInfo.y_anchor);
    EXPECT_DOUBLE_EQ (known_z_anchor, gpsInfo.z_anchor);
    EXPECT_DOUBLE_EQ (known_x_target, gpsInfo.x_target);
    EXPECT_DOUBLE_EQ (known_y_target, gpsInfo.y_target);
    EXPECT_DOUBLE_EQ (known_z_target, gpsInfo.z_target);
    EXPECT_NEAR (known_x_first, gpsInfo.x_first, 0.0000005);
    EXPECT_NEAR (known_y_first, gpsInfo.y_first, 0.000001  );
    EXPECT_NEAR (known_z_first, gpsInfo.z_first, 0.0000005);
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
    known_x_last = 516209.920748;
    known_Y_last = 4767923.320612;
    known_Z_last = 2084.678522;
    known_edge = 0;
    known_scan_direction = 0;
    known_facet = 1;
    known_intensity = 0;

    EXPECT_DOUBLE_EQ (known_gps_time, gpsInfo.gpsTime);
    EXPECT_DOUBLE_EQ (known_x_anchor, gpsInfo.x_anchor);
    EXPECT_DOUBLE_EQ (known_y_anchor, gpsInfo.y_anchor);
    EXPECT_DOUBLE_EQ (known_z_anchor, gpsInfo.z_anchor);
    EXPECT_DOUBLE_EQ (known_x_target, gpsInfo.x_target);
    EXPECT_DOUBLE_EQ (known_y_target, gpsInfo.y_target);
    EXPECT_DOUBLE_EQ (known_z_target, gpsInfo.z_target);
    EXPECT_NEAR (known_x_first, gpsInfo.x_first, 0.000006);
    EXPECT_NEAR (known_y_first, gpsInfo.y_first, 0.000002);
    EXPECT_NEAR (known_z_first, gpsInfo.z_first, 0.000002);
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
    known_x_last = 516209.586749;
    known_Y_last = 4767923.619501;
    known_Z_last = 2084.624262;
    known_edge = 0;
    known_scan_direction = 0;
    known_facet = 1;
    known_intensity = 0;

    EXPECT_DOUBLE_EQ (known_gps_time, gpsInfo.gpsTime);
    EXPECT_DOUBLE_EQ (known_x_anchor, gpsInfo.x_anchor);
    EXPECT_DOUBLE_EQ (known_y_anchor, gpsInfo.y_anchor);
    EXPECT_DOUBLE_EQ (known_z_anchor, gpsInfo.z_anchor);
    EXPECT_DOUBLE_EQ (known_x_target, gpsInfo.x_target);
    EXPECT_DOUBLE_EQ (known_y_target, gpsInfo.y_target);
    EXPECT_DOUBLE_EQ (known_z_target, gpsInfo.z_target);
    EXPECT_NEAR (known_x_first, gpsInfo.x_first, 0.000002);
    EXPECT_NEAR (known_y_first, gpsInfo.y_first, 0.0000004);
    EXPECT_NEAR (known_z_first, gpsInfo.z_first, 0.0000002);
    EXPECT_EQ (known_edge, gpsInfo.edge);
    EXPECT_EQ (known_facet, gpsInfo.facet);
    EXPECT_EQ (known_scan_direction, gpsInfo.scanDirection);
    EXPECT_EQ (known_intensity, gpsInfo.intensity);
  }
}






