/*
 * File Name: Peak_unittests.cpp
 * Created On: 11 June 2019
 * Author: Aaron Orenstein
*/

#include "Peak.hpp"
#include "gtest/gtest.h"

class PeakTest : public testing::Test{
    protected:
        
        virtual void SetUp(){
            //Set activation point coordinates
            peak->x_activation = 3;
            peak->y_activation = 4;
            peak->z_activation = 12;
            //Set amplitude and fwhm
            peak->amp = 10;
            peak->fwhm = 5;
        }

        virtual void TearDown(){
            delete(peak);
        }

        Peak* peak = new Peak();
};

//Tests backscatter coefficient calculation
TEST_F(PeakTest, backscatterTest){
    EXPECT_NO_THROW(peak->calcBackscatter(10, 5, 1e-1, 0, 0, 0));
    EXPECT_NEAR(119.9607135,peak->backscatter_coefficient,.1);
}

//Tests infinite backscatter coefficient
TEST_F(PeakTest, backscatterDivZeroTest){
    EXPECT_NO_THROW(peak->calcBackscatter(0, 5, 1e-1, 0, 0, 0));
    EXPECT_EQ(INFINITY,peak->backscatter_coefficient);
}
