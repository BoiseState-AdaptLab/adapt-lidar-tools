/*
 * File name: LidarVolume_unittests.cpp
 * Created on: 08-November-2017
 * Author: ravi
 * Modified by: Nicholas Chapa
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


TEST_F(LidarVolumeTest, CheckBoundingBoxClippedTest){
    //This test will compare the calculated bounding box values
    //for x, y, and z with the known values in the clipped test .pls file

    std::string fileNameCorrect =  "etc/140823_183115_1_clipped_test.pls";

    
    PULSEreadOpener pOpener;  // Create a pulse read opener object
    PULSEreader *pReader;     // Create a pulse reader object
    pOpener.set_file_name(fileNameCorrect.c_str());
    pReader = pOpener.open();
    
    lidarVolume.calculateBoundingBox(pReader);

    int known_x_min= (int) (floor(516209.586) - 10);
    int known_y_min= (int) (floor(4767921.375) - 10);
    int known_z_min= (int) (floor(2084.585) - 10);
    int known_x_max= (int) (ceil(516211.942) + 10);
    int known_y_max= (int) (ceil(4767923.621) + 10);
    int known_z_max= (int) (ceil(2093.581) + 10);

    int known_x_extent= known_x_max - known_x_min;
    int known_y_extent= known_y_max - known_y_min;
    int known_z_extent= known_z_max - known_z_min;
    
    ASSERT_EQ(known_x_min,lidarVolume.pulse_bb_i_min);
    ASSERT_EQ(known_y_min,lidarVolume.pulse_bb_j_min);
    ASSERT_EQ(known_z_min,lidarVolume.pulse_bb_k_min);
    ASSERT_EQ(known_x_max,lidarVolume.pulse_bb_i_max);
    ASSERT_EQ(known_y_max,lidarVolume.pulse_bb_j_max);
    ASSERT_EQ(known_x_max,lidarVolume.pulse_bb_k_max);
    
    ASSERT_EQ(known_x_extent,lidarVolume.pulse_x_extent);
    ASSERT_EQ(known_y_extent,lidarVolume.pulse_y_extent);
    ASSERT_EQ(known_z_extent,lidarVolume.pulse_z_extent);
    

    //pulseheader.hpp is insided fullalpha/deps/wavething/inc
 
}

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
