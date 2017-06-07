/*
 * File name: testDriver.cpp
 * Created on: 02-June-2017
 * Author: ravi
 */

#include "gtest/gtest.h"
#include "FullWaveformIngestion.hpp"


class FullWaveFormTest : public testing::Test {
  protected:

  FullWaveformIngestion ingester;

  // function to set up space used by all tests
  virtual void SetUp(){
  }

};

  /*
  * Given the true values of the actual anchor, target and deviation,
  * we calculate the actual anchor, actual target and actual deviation
  * using FullWaveformIngestion functions
  */
TEST_F(FullWaveFormTest, computeRange) {


/****************************************************************************
* 
* Begin X tests
* 
****************************************************************************/

  // Data from a wave file
  double anchorX = 24220.;
  double targetX = 1825.;
  double scaleFactorX = 0.001;
  double offsetX = 475048.;

  // Pre-calculate values to test with
  double truthActualAnchorX = 475072.220;
  double truthActualTargetX = 475049.83;
  double truthDeviationX = -0.022395; 

  /*
  * Calculate the actual X anchor and test if it is equal 
  * to the pre-calculated value
  */
  double actualAnchorX = ingester.calculateActualCoordinate(
                         anchorX, scaleFactorX, offsetX);
  ASSERT_DOUBLE_EQ(truthActualAnchorX, actualAnchorX);

  /*
  * Calculate the actual X target and test if it is equal 
  * to the pre-calculated value
  */
  double actualTargetX = ingester.calculateActualCoordinate(
                         targetX, scaleFactorX, offsetX);
  ASSERT_NEAR(truthActualTargetX, actualTargetX,0.06);

  /*
  * Calculate the actual X deviation and test if it 
  * is equal to the pre-calculated value
  */
  double deviationX = ingester.calculateDeviation(
                        actualAnchorX,actualTargetX);
  ASSERT_NEAR(truthDeviationX, deviationX,0.0005);


/****************************************************************************
* 
* Begin Y tests
* 
****************************************************************************/

  // Data from a wave file
  double anchorY = 3178859.;
  double targetY = 3171794.;
  double scaleFactorY = 0.001;
  double offsetY = 4422943.;

  // Pre-calculate values to test with
  double truthActualAnchorY = 4426121.9;
  double truthActualTargetY = 4426114.8;
  double truthDeviationY = -0.007606;
  
  /*
  * Calculate the actual Y anchor and test if it is equal 
  * to the pre-calculated value
  */
  double actualAnchorY = ingester.calculateActualCoordinate(
                         anchorY, scaleFactorY, offsetY);
  ASSERT_NEAR(truthActualAnchorY, actualAnchorY, 0.1);
  
  /*
  * Calculate the actual Y target and test if it is equal 
  * to the pre-calculated value
  */
  double actualTargetY = ingester.calculateActualCoordinate(
                         targetY, scaleFactorY, offsetY);
  ASSERT_NEAR(truthActualTargetY, actualTargetY,0.01);
  
  /*
  * Calculate the actual Y deviation and test if it is equal 
  * to the pre-calculated value
  */
  double deviationY = ingester.calculateDeviation(
                        actualAnchorY,actualTargetY);
  ASSERT_NEAR(truthDeviationY, deviationY, 0.001);

/****************************************************************************
* 
* Begin Z tests
* 
****************************************************************************/    

  // Data from a wave file
  double anchorZ = -6008.;
  double targetZ = -154011.;
  double scaleFactorZ = 0.001;
  double offsetZ = 3042;

  // Pre-calculate values to test with
  double truthActualAnchorZ = 3035.992;
  double truthActualTargetZ = 2887.9890;
  double truthDeviationZ = -0.148003;

  /*
  * Calculate the actual z anchor and test if it is equal 
  * to the pre-calculated value
  */
  double actualAnchorZ = ingester.calculateActualCoordinate(
                         anchorZ, scaleFactorZ, offsetZ);
  ASSERT_DOUBLE_EQ(truthActualAnchorZ, actualAnchorZ);

  /*
  * Calculate the actual Z target and test if it is equal 
  * to the pre-calculated value
  */
  double actualTargetZ = ingester.calculateActualCoordinate(
                         targetZ, scaleFactorZ, offsetZ);
  ASSERT_DOUBLE_EQ(truthActualTargetZ, actualTargetZ);
  
  /*
  * Calculate the actual Z deviation and test if it is equal 
  * to the pre-calculated value
  */
  double deviationZ = ingester.calculateDeviation(
                        actualAnchorZ,actualTargetZ);
  ASSERT_NEAR(truthDeviationZ, deviationZ, 0.0001);


/****************************************************************************
* 
* Begin Return Location tests (Calculate the location of a point)
* formula --> returnLocation = actualAnchor + time*deviation
*
****************************************************************************/ 

  // The time in this case is called the first returning sample from
  // the binary input file
  double time = 6054.;

  // Pre-calculate values to test with
  double truthReturnLocationX = 474936.64;
  double truthReturnLocationY = 4426079.1;
  double truthReturnLocationZ = 2139.9818;

  /*
  * Calculate the X return location and test if it is equal 
  * to the pre-calculated value
  */
  double returnLocationX = ingester.calculateReturnLocation(
                             actualAnchorX, deviationX, time);
  ASSERT_NEAR(truthReturnLocationX,returnLocationX, 0.001);

  /*
  * Calculate the Y return location and test if it is equal 
  * to the pre-calculated value
  */
  double returnLocationY = ingester.calculateReturnLocation(
                             actualAnchorY, deviationY, time);
  ASSERT_NEAR(truthReturnLocationY,returnLocationY, 0.1);

  /*
  * Calculate the Z return location and test if it is equal 
  * to the pre-calculated value
  */
  double returnLocationZ = ingester.calculateReturnLocation(
                             actualAnchorZ, deviationZ, time);
  ASSERT_NEAR(truthReturnLocationZ,returnLocationZ, 0.0001);

}
