/*
 * File name: LidarVolume_unittests.cpp
 * Created on: 08-November-2017
 * Author: ravi
 */

#include "gtest/gtest.h"
#include "LidarVolume.hpp"
#include "pulsereader.hpp"
#include "pulsewriter.hpp"
#include <iostream>


class LidarVolumeTest: public testing::Test{
  public: 
    LidarVolume ld;

  protected:


  // function to set up space used by all tests
  virtual void SetUp(){
    std::string fileName =  "etc/140823_183115_1_clipped_test.pls";
    PULSEreadOpener pOpener;  // Create a pulse read opener object
    PULSEreader *pReader;     // Create a pulse reader object
    pOpener.set_file_name(fileName.c_str());
    pReader = pOpener.open();

    if(pReader->read_waves){
      CalculateBoundingBox(pReader);
    }
  }

};

/*
* Given the true values of the the min and max fields describing the 
* boundary box(x, y & z), compare it with values that are read in. 
*/ 
TEST_F(LidarVolumeTest, xyzMinsAndMaxes){
  //TODO:
}

/*
* Given the true values of the the min and max fields describing the 
* boundary box(i, j & k), compare it with values that are read in. 
*/
TEST_F(LidarVolumeTest, ijkMinsAndMaxes){
  //TODO:
}

/*
* Given the true values of the the min and max fields describing the 
* boundary box, compare it with values that are read in. 
*/
TEST_F(LidarVolumeTest, xyzExtents){
  //TODO:
}

