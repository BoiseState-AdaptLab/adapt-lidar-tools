 // File name: LidarVolume_unittests.cpp
 // Created on: 08-November-2017
 // Author: ravi 

#include <math.h>

#include "gtest/gtest.h"
#include "LidarVolume.hpp"
#include "FlightLineData.hpp"
#include "Peak.hpp"

class LidarVolumeTest: public testing::Test{

  protected:
  
  // function to set up space used by all tests
  virtual void SetUp(){
  }

};


TEST_F(LidarVolumeTest, CheckBoundingBoxClippedTest){

    //This test will compare the calculated bounding box values
    //for x, y, and z with the known values in the clipped test .pls file

    FlightLineData fld;
    std::string file_name =  "etc/140823_183115_1_clipped_test.pls";
    
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
    
    peak myPeak;
    myPeak.time = 12.23;
    myPeak.amp = 345.445;
    myPeak.width = 23.453;
    
    
    // lidarVolume.allocateMemory();
    // lidarVolume.insertPeak(&myPeak);
    // lidarVolume.deallocateMemory();
}

//this test will add peaks to lidar volume by adding struct pointers to an array
TEST_F(LidarVolumeTest, allocateTwoPeaksMemory){
    LidarVolume lidarVolume;
    
    struct peak{
      double time;
      double amp;
      double width;
    };
    
    peak *myFirstPeak = new peak;
    peak *mySecondPeak = new peak;
    
    myFirstPeak->time = 12.23;
    myFirstPeak->amp = 345.445;
    myFirstPeak->width = 23.453;
    
    mySecondPeak->time = 13.23;
    mySecondPeak->amp = 350.445;
    mySecondPeak->width = 33.453;
    
    
    // lidarVolume.allocateMemory();
    // lidarVolume.insertPeak(myFirstPeak);
    // lidarVolume.deallocateMemory();
}

//this test will add peaks to lidar volume by adding struct pointers to an array
TEST_F(LidarVolumeTest, allocateThreePeaksMemory){
    LidarVolume lidarVolume;
    
    struct peak{
      double time;
      double amp;
      double width;
    };
    
    peak *myFirstPeak = new peak;
    peak *mySecondPeak = new peak;
    peak *myThirdPeak = new peak;
    
    myFirstPeak->time = 12.23;
    myFirstPeak->amp = 345.445;
    myFirstPeak->width = 23.453;
    
    mySecondPeak->time = 13.23;
    mySecondPeak->amp = 350.445;
    mySecondPeak->width = 33.453;
    
    myThirdPeak->time = 14.23;
    myThirdPeak->amp = 450.445;
    myThirdPeak->width = 53.453;
    
    
    // lidarVolume.allocateMemory();
    // lidarVolume.insertPeak(myFirstPeak);
    // lidarVolume.deallocateMemory();
}

