/*
 * File name: LidarVolume_unittests.cpp
 * Created on: 08-November-2017
 * Author: ravi
 */

#include <math.h>

#include "gtest/gtest.h"
#include "LidarVolume.hpp"
#include "pulsereader.hpp"
#include "pulsewriter.hpp"

class LidarVolumeTest: public testing::Test{

  protected:
  
  LidarVolume lidarVolume;

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

/*
TEST_F(LidarVolumeTest, CheckBoundingBox1){
    //This test will compare the calculated bounding box values
    //for x, y, and z with the known values

    std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";

    
    GPSInformation gpsInfo;   // Create a GPS object

    PULSEreadOpener pOpener;  // Create a pulse read opener object
    PULSEreader *pReader;     // Create a pulse reader object
    pOpener.set_file_name(fileNameCorrect.c_str());
    pReader = pOpener.open();

    int known_x_min= (int) (floor(516210) - 10);
    int known_y_min= (int) (floor(4767920) - 10);
    int known_z_min= (int) (floor(2084.59) - 10);
    int known_x_max= (int) (ceil(0) + 10);
    int known_y_max= (int) (ceil(0) + 10);
    int known_z_max= (int) (ceil(0) + 10);
   
    int pulse_x_min= (int) (floor(pReader->header.min_x) - 10);
    int pulse_y_min= (int) (floor(pReader->header.min_y) - 10);
    int pulse_z_min= (int) (floor(pReader->header.min_z) - 10);
    int pulse_x_max= (int) (floor(pReader->header.max_x) + 10);
    int pulse_y_max= (int) (floor(pReader->header.max_y) + 10);
    int pulse_z_max= (int) (floor(pReader->header.max_z) + 10);

    int known_x_extent= known_x_max - known_x_min;
    int known_y_extent= known_y_max - known_y_min;
    int known_z_extent= known_z_max - known_z_min;

    int pulse_x_extent= pulse_x_max - pulse_x_min;
    int pulse_y_extent= pulse_y_max - pulse_y_min;
    int pulse_z_extent= pulse_z_max - pulse_z_min;
    
    ASSERT_EQ(known_x_min,pulse_x_min);
    ASSERT_EQ(known_y_min,pulse_y_min);
    ASSERT_EQ(known_z_min,pulse_z_min);
    ASSERT_EQ(known_x_max,pulse_x_max);
    ASSERT_EQ(known_y_max,pulse_x_max);
    ASSERT_EQ(known_x_max,pulse_x_max);
    
    ASSERT_EQ(known_x_extent,pulse_x_extent);
    ASSERT_EQ(known_x_extent,pulse_x_extent);
    ASSERT_EQ(known_x_extent,pulse_x_extent);
    

    //pulseheader.hpp is insided fullalpha/deps/wavething/inc
 
} */

TEST_F(LidarVolumeTest, CheckBoundingBox2Nayani){
    //This test will compare the calculated bounding box values
    //for x, y, and z with the known values for nayani's pulse wave

    std::string fileNameCorrect =  "140823_152426_2.pls"; 

    PULSEreadOpener pOpener;  // Create a pulse read opener object
    PULSEreader *pReader;     // Create a pulse reader object
    try {
      pOpener.set_file_name(fileNameCorrect.c_str());
      pReader = pOpener.open();
    }
    catch (...){
      std::cout << "the file does not exist" << std::endl;
    }
    
    lidarVolume.calculateBoundingBox(pReader);
    
    

    int known_x_min= (int) (floor(510706.487) - 10);
    int known_y_min= (int) (floor(4792924.648) - 10);
    int known_z_min= (int) (floor(1604.828) - 10);
    int known_x_max= (int) (ceil(512412.097) + 10);
    int known_y_max= (int) (ceil(4794125.777) + 10);
    int known_z_max= (int) (ceil(1927.448) + 10);
   

    int known_x_extent= known_x_max - known_x_min;
    int known_y_extent= known_y_max - known_y_min;
    int known_z_extent= known_z_max - known_z_min;

    
    ASSERT_EQ(known_x_min,lidarVolume.pulse_bb_i_min);
    ASSERT_EQ(known_y_min,lidarVolume.pulse_bb_j_min);
    ASSERT_EQ(known_z_min,lidarVolume.pulse_bb_k_min);
    ASSERT_EQ(known_x_max,lidarVolume.pulse_bb_i_max);
    ASSERT_EQ(known_y_max,lidarVolume.pulse_bb_j_max);
    ASSERT_EQ(known_z_max,lidarVolume.pulse_bb_k_max);
    
    ASSERT_EQ(known_x_extent,lidarVolume.pulse_x_extent);
    ASSERT_EQ(known_y_extent,lidarVolume.pulse_y_extent);
    ASSERT_EQ(known_z_extent,lidarVolume.pulse_z_extent);
    

    //pulseheader.hpp is insided fullalpha/deps/wavething/inc
 
}
