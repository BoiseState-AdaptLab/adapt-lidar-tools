// File name: LidarVolume_unittests.cpp
// Created on: 08-November-2017
// Author: ravi 

#include "gtest/gtest.h"
#include "LidarVolume.hpp"
#include "FlightLineData.hpp"
#include <math.h>

class LidarVolumeTest: public testing::Test{
  protected:
  
  // function to set up space used by all tests
  virtual void SetUp(){
  }

};

/****************************************************************************
* 
* Calling the setFlightLineData() function test
* 
****************************************************************************/
TEST_F(LidarVolumeTest, testBoundingBox){

    //This test will compare the calculated bounding box values
    //for x, y, and z with the known values in the clipped test .pls file

    std::string file_name =  "etc/140823_183115_1_clipped_test.pls";
    FlightLineData fld;

    EXPECT_NO_THROW (fld.setFlightLineData(file_name)); 

    LidarVolume lv;
    lv.setBoundingBox(fld.bb_x_min,fld.bb_x_max,
                      fld.bb_y_min,fld.bb_y_max,
                      fld.bb_z_min,fld.bb_z_max);
    

    int known_i_min= (int) (floor(516209.586) - 10);
    int known_j_min= (int) (floor(4767921.375) - 10);
    int known_k_min= (int) (floor(2084.585) - 10);
    int known_i_max= (int) (ceil(516211.942) + 10);
    int known_j_max= (int) (ceil(4767923.621) + 10);
    int known_k_max= (int) (ceil(2093.581) + 10);
  
    ASSERT_EQ(known_i_min,lv.bb_i_min);
    ASSERT_EQ(known_j_min,lv.bb_j_min);
    ASSERT_EQ(known_k_min,lv.bb_k_min);
    ASSERT_EQ(known_i_max,lv.bb_i_max);
    ASSERT_EQ(known_j_max,lv.bb_j_max);
    ASSERT_EQ(known_k_max,lv.bb_k_max);
    
    int known_i_extent= known_i_max - known_i_min;
    int known_j_extent= known_j_max - known_j_min;
    int known_k_extent= known_k_max - known_k_min;

    ASSERT_EQ(known_i_extent,lv.i_extent);
    ASSERT_EQ(known_j_extent,lv.j_extent);
    ASSERT_EQ(known_k_extent,lv.k_extent);
    
}


/****************************************************************************
* 
* Calling the setFlightLineData() function test
* 
****************************************************************************/
TEST_F(LidarVolumeTest, allocatePeakMemory){
    LidarVolume lidarVolume;
    
    struct peak{
      double time;
      double amp;
      double width;
    };
    
    peak myPeak;
    myPeak.time = 12.23;
    myPeak.amp = 345.445;
    myPeak.width = 23.453;
    
    
    // lidarVolume.allocateMemory();
    // lidarVolume.insertPeak(&myPeak);
    // lidarVolume.deallocateMemory();
}