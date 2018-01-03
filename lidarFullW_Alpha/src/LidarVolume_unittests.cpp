/*
 * File name: LidarVolume_unittests.cpp
 * Created on: 08-November-2017
 * Author: ravi
 * Modified by: Nicholas Chapa
 */

#include <math.h>

#include "gtest/gtest.h"
#include "LidarVolume.hpp"
#include "Peak.hpp"

class LidarVolumeTest: public testing::Test{

  protected:
  
  // function to set up space used by all tests
  virtual void SetUp(){
  }

};


TEST_F(LidarVolumeTest, CheckBoundingBoxClippedTest){

    LidarVolume lidarVolume;
    // values that will be read from the FlightLineData
    double known_bb_x_min= 516209.586;
    double known_bb_y_min= 4767921.375;
    double known_bb_z_min= 2084.585;
    double known_bb_x_max= 516211.942;
    double known_bb_y_max= 4767923.621;
    double known_bb_z_max= 2093.581;

    lidarVolume.setBoundingBox(known_bb_x_min, known_bb_x_max,
                               known_bb_y_min, known_bb_y_max,
                               known_bb_z_min, known_bb_z_max);

    int known_i_min= (int) (floor(516209.586) - 10);
    int known_j_min= (int) (floor(4767921.375) - 10);
    int known_k_min= (int) (floor(2084.585) - 10);
    int known_i_max= (int) (ceil(516211.942) + 10);
    int known_j_max= (int) (ceil(4767923.621) + 10);
    int known_k_max= (int) (ceil(2093.581) + 10);

    int known_i_extent= known_i_max - known_i_min;
    int known_j_extent= known_j_max - known_j_min;
    int known_k_extent= known_k_max - known_k_min;
    
    ASSERT_EQ(known_i_min,lidarVolume.bb_i_min);
    ASSERT_EQ(known_j_min,lidarVolume.bb_j_min);
    ASSERT_EQ(known_k_min,lidarVolume.bb_k_min);
    ASSERT_EQ(known_i_max,lidarVolume.bb_i_max);
    ASSERT_EQ(known_j_max,lidarVolume.bb_j_max);
    ASSERT_EQ(known_k_max,lidarVolume.bb_k_max);
    
    ASSERT_EQ(known_i_extent,lidarVolume.i_extent);
    ASSERT_EQ(known_j_extent,lidarVolume.j_extent);
    ASSERT_EQ(known_k_extent,lidarVolume.k_extent);
 
}

TEST_F(LidarVolumeTest, CheckBoundingBox2Nayani){
    
    LidarVolume lidarVolume;

    // values that will be read from the FlightLineData
    double known_bb_x_min=510706.487;
    double known_bb_y_min=4792924.648;
    double known_bb_z_min=1604.828;
    double known_bb_x_max=512412.097;
    double known_bb_y_max=4794125.777;
    double known_bb_z_max=1927.448;

    lidarVolume.setBoundingBox(known_bb_x_min, known_bb_x_max,
                               known_bb_y_min, known_bb_y_max,
                               known_bb_z_min, known_bb_z_max);


    int known_i_min= (int) (floor(510706.487) - 10);
    int known_j_min= (int) (floor(4792924.648) - 10);
    int known_k_min= (int) (floor(1604.828) - 10);
    int known_i_max= (int) (ceil(512412.097) + 10);
    int known_j_max= (int) (ceil(4794125.777) + 10);
    int known_k_max= (int) (ceil(1927.448) + 10);
   

    int known_i_extent= known_i_max - known_i_min;
    int known_j_extent= known_j_max - known_j_min;
    int known_k_extent= known_k_max - known_k_min;

    
    ASSERT_EQ(known_i_min,lidarVolume.bb_i_min);
    ASSERT_EQ(known_j_min,lidarVolume.bb_j_min);
    ASSERT_EQ(known_k_min,lidarVolume.bb_k_min);
    ASSERT_EQ(known_i_max,lidarVolume.bb_i_max);
    ASSERT_EQ(known_j_max,lidarVolume.bb_j_max);
    ASSERT_EQ(known_k_max,lidarVolume.bb_k_max);
    
    ASSERT_EQ(known_i_extent,lidarVolume.i_extent);
    ASSERT_EQ(known_j_extent,lidarVolume.j_extent);
    ASSERT_EQ(known_k_extent,lidarVolume.k_extent);
    
}

//this test will add peaks to lidar volume by adding struct pointers to an array
TEST_F(LidarVolumeTest, allocatePeakMemory){
    LidarVolume lidarVolume;
    
    struct peak{
      double time;
      double amp;
      double width;
    };
    
    peak *myPeak = new peak;
    myPeak->time = 12.23;
    myPeak->amp = 345.445;
    myPeak->width = 23.453;
    
    
    //lidarVolume.allocateMemory();
    //lidarVolume.insertPeak(&myPeak);
    //lidarVolume.deallocateMemory();
}
