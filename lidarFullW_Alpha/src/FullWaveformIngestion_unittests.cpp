/*
 * File name: testDriver.cpp
 * Created on: 24-May-2017
 * Author: ravi
 */

#include "gtest/gtest.h"
#include "FullWaveformIngestion.hpp"
#include <getopt.h>


class FullWaveFormTest : public testing::Test {
  protected:

  FullWaveformIngestion ingester;

  // function to set up space used by all tests
  virtual void SetUp(){
  }

};

// Tests valid short command line options
TEST_F(FullWaveFormTest, computeRange) {

  double anchorX = 24220.;
  double targetX = 1825.;
  double scaleFactorX = 0.001;
  double offsetX = 475048.;
  double truthActualAnchorX = 475072.220;
  double truthActualTargetX = 475049.83;
  double truthDeviationX = -0.022395;
  double actualAnchorX = ingester.calculateActualCoordinate(
                         anchorX, scaleFactorX, offsetX);
  ASSERT_DOUBLE_EQ(truthActualAnchorX, actualAnchorX);
  double actualTargetX = ingester.calculateActualCoordinate(
                         targetX, scaleFactorX, offsetX);
  ASSERT_DOUBLE_EQ(truthActualTargetX, actualTargetX);
  double deviationX = ingester.calculateDeviation(
                        actualAnchorX,actualTargetX);
  ASSERT_DOUBLE_EQ(truthDeviationX, deviationX);

  double anchorY = 3178859.;
  double targetY = 3171794.;
  double scaleFactorY = 0.001;
  double offsetY = 4422943.;
  double truthActualAnchorY = 4426121.9;
  double truthActualTargetY = 4426114.8;
  double truthDeviationY = -0.007606;
  double actualAnchorY = ingester.calculateActualCoordinate(
                         anchorY, scaleFactorY, offsetY);
  ASSERT_DOUBLE_EQ(truthActualAnchorY, actualAnchorY);
  double actualTargetY = ingester.calculateActualCoordinate(
                         targetY, scaleFactorY, offsetY);
  ASSERT_DOUBLE_EQ(truthActualTargetY, actualTargetY);
  double deviationY = ingester.calculateDeviation(
                        actualAnchorY,actualTargetY);
  ASSERT_DOUBLE_EQ(truthDeviationY, deviationY);
    

  double anchorZ = -6008.;
  double targetZ = -154011.;
  double scaleFactorZ = 0.001;
  double offsetZ = 3042;
  double truthActualAnchorZ = 3035.992;
  double truthActualTargetZ = 2887.9890;
  double truthDeviationZ = -0.148003;
  double actualAnchorZ = ingester.calculateActualCoordinate(
                         anchorZ, scaleFactorZ, offsetZ);
  ASSERT_DOUBLE_EQ(truthActualAnchorZ, actualAnchorZ);
  double actualTargetZ = ingester.calculateActualCoordinate(
                         targetZ, scaleFactorZ, offsetZ);
  ASSERT_DOUBLE_EQ(truthActualTargetZ, actualTargetZ);
  double deviationZ = ingester.calculateDeviation(
                        actualAnchorZ,actualTargetZ);
  ASSERT_DOUBLE_EQ(truthDeviationZ, deviationZ);

  // calculate the location of a point
  // The time in this case is called the first returning sample from
  // the binary input file
  double time = 6054.;
  double truthReturnLocationX = 474936.64;
  double truthReturnLocationY = 4426079.1;
  double truthReturnLocationZ = 2139.9818;

  // actualAnchor + time*deviation
  double returnLocationX = ingester.calculateReturnLocation(
                             actualAnchorX, deviationX, time);
  ASSERT_DOUBLE_EQ(truthReturnLocationX,returnLocationX);
  double returnLocationY = ingester.calculateReturnLocation(
                             actualAnchorY, deviationY, time);
  ASSERT_DOUBLE_EQ(truthReturnLocationY,returnLocationY);
  double returnLocationZ = ingester.calculateReturnLocation(
                             actualAnchorZ, deviationZ, time);
  ASSERT_DOUBLE_EQ(truthReturnLocationZ,returnLocationZ);

}
