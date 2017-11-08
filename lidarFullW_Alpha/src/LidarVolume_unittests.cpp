/*
 * File name: LidarVolume_unittests.cpp
 * Created on: 08-November-2017
 * Author: ravi
 */

#include "gtest/gtest.h"
#include "LidarVolume.hpp"
#include "pulsereader.hpp"
#include "pulsewriter.hpp"

class LidarVolumeTest: public testing::Test{

  protected:

  // function to set up space used by all tests
  virtual void SetUp(){
    
  }

};

/*
* Test x,y & z mins and maxes, i, j, k mins and maxes and x,y and z extents
*/ 
TEST_F(LidarVolumeTest, allBoundingBoxTests){
  LidarVolume ld;

  //Replace the filename with the pls file you are using for the unit test
  std::string fileName =  "etc/140823_183115_1_clipped_test.pls";
  
  PULSEreadOpener pOpener;  // Create a pulse read opener object
  PULSEreader *pReader;     // Create a pulse reader object
  pOpener.set_file_name(fileName.c_str());
  pReader = pOpener.open();

  //TODO:


}
