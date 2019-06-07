// File name: LidarVolume_unittests.cpp
// Created on: 08-November-2017
// Author: ravi 

#include "gtest/gtest.h"
#include "LidarVolume.hpp"
#include "FlightLineData.hpp"
#include <math.h>
#include <vector>

class LidarVolumeTest: public testing::Test{
    protected:

        // function to set up space used by all tests
        virtual void SetUp(){
        }

};

/****************************************************************************
 * 
 * Test bounding box i, j, k mins and maxes and the extents
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


    int known_x_idx_min = 0;
    int known_y_idx_min = 0;
    int known_z_idx_min = 0;
    int known_x_idx_max= (int) (ceil(516211.942 + 10)) -
        (floor(516209.586 - 10));
    int known_y_idx_max= (int) (ceil(4767923.621 + 10)) -
        (floor(4767921.375 - 10));
    int known_z_idx_max= (int) (ceil(2093.581 + 10)) - (floor(2084.585 - 10));

    EXPECT_EQ(known_x_idx_min, lv.bb_x_idx_min);
    EXPECT_EQ(known_y_idx_min, lv.bb_y_idx_min);
    EXPECT_EQ(known_z_idx_min, lv.bb_z_idx_min);
    EXPECT_EQ(known_x_idx_max, lv.bb_x_idx_max);
    EXPECT_EQ(known_y_idx_max, lv.bb_y_idx_max);
    EXPECT_EQ(known_z_idx_max, lv.bb_z_idx_max);

    int known_x_idx_extent= known_x_idx_max - known_x_idx_min + 1;
    int known_y_idx_extent= known_y_idx_max - known_y_idx_min + 1;

    EXPECT_EQ(known_x_idx_extent, lv.x_idx_extent);
    EXPECT_EQ(known_y_idx_extent, lv.y_idx_extent);

}

TEST_F(LidarVolumeTest, testPosition){
    std::string file_name =  "etc/140823_152425_1.pls";
    FlightLineData fld;
    EXPECT_NO_THROW (fld.setFlightLineData(file_name));

    LidarVolume lv;
    lv.setBoundingBox(fld.bb_x_min,fld.bb_x_max,
            fld.bb_y_min,fld.bb_y_max,
            fld.bb_z_min,fld.bb_z_max);

    int known_x_idx_extent = 1771;
    int known_y_idx_extent = 2621;

    EXPECT_EQ(known_x_idx_extent, lv.x_idx_extent);
    EXPECT_EQ(known_y_idx_extent, lv.y_idx_extent);

    //Testing postion at y = 1770, x = 1771
    int known_position_1 = 1771 + (1770 * known_y_idx_extent);
    EXPECT_EQ(known_position_1, lv.position(1770,1771));

    //Testing postion at y = 1770, x = 1772
    int known_position_2 = 1772 + (1770 * known_y_idx_extent);
    EXPECT_EQ(known_position_2, lv.position(1770,1772));

}


/******************************************************************************
 *
 * Test adding a peak outside the bounding area
 *
 ******************************************************************************/
TEST_F(LidarVolumeTest, add_invalid_peak_to_volume_test){

    std::vector<Peak> peaks;
    LidarVolume lidarVolume;
    FlightLineData fld;
    std::string filename;
    filename = "etc/140823_183115_1_clipped_test.pls";

    EXPECT_NO_THROW(fld.setFlightLineData(filename));


    double known_amp = 142;
    double known_loc = 19;
    double known_fwhm = 5;
    double known_fwhm_t_pos = 21;
    double known_fwhm_t_neg = 16;
    int known_pos = 0;
    int known_triggering_amp = 13;
    int known_triggering_loc = 14;
    double known_x_act = 253036;
    double known_y_act = 4330482;
    double known_z_act = 4159;

    Peak p;
    p.amp= known_amp;
    p.location = known_loc;
    p.fwhm = known_fwhm;
    p.fwhm_t_positive = known_fwhm_t_pos;
    p.fwhm_t_negative = known_fwhm_t_neg;
    p.position_in_wave = known_pos;
    p.triggering_amp = known_triggering_amp;
    p.triggering_location = known_triggering_loc;
    p.x_activation = known_x_act;
    p.y_activation = known_y_act;
    p.z_activation = known_z_act;

    peaks.push_back(p);
    lidarVolume.setBoundingBox(fld.bb_x_min,fld.bb_x_max,
            fld.bb_y_min,fld.bb_y_max,
            fld.bb_z_min,fld.bb_z_max);

    EXPECT_NO_THROW(lidarVolume.insert_peak(&peaks.at(0)));

}
