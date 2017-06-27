/*
 * File name: Impulse_unittests.cpp
 * Created on: 27-June-2017
 * Author: ravi
 *
 * This test requres the sample 140823_183115_1_clipped_test.pls file 
 * and its corresponding wvs file to be in the etc directory to run
 *
 */

#include "gtest/gtest.h"
#include "GPSInformation.hpp"


class ImpulseTest: public testing::Test{
  protected:

  Impulse aPulse;

  // function to set up space used by all tests
  virtual void SetUp(){
  }

};

/*
* Given the true values of the actual anchor, target and deviation,
* we calculate the actual anchor, actual target and actual deviation
* using FullWaveformIngestion functions
*/
TEST_F(ImpulseTest, checkValues){
  
}
