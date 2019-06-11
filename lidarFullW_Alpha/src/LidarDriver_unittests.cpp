//
// Created by arezaii on 3/22/19.
//

#include "LidarDriver.hpp"
#include "gtest/gtest.h"
#include "Peak.hpp"
#include <math.h>

/* All peak input data to be tested against is defined in the following
 * macros */

#define INPUT_TYPICAL \
    "2 2 1 1 0 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 139 " \
    "120 99 79 63 50 46 43 43 50 60 70 65 50 33 34 31 24 17 11 8 7 6 5 6 5 4 " \
    "4 5 5 6 5 5 2 1 1 1"

#define TYPICAL_FIRST_PEAK 19 //Index of first peak in vector
#define TYPICAL_LAST_PEAK 34 //Index of last peak in vector

#define TYPICAL_FIRST_MAX 200
#define TYPICAL_FIRST_MIN 200
#define TYPICAL_FIRST_MEAN 200
#define TYPICAL_FIRST_STDEV 0
#define TYPICAL_FIRST_SKEW 0
#define TYPICAL_FIRST_KURT 0

#define TYPICAL_LAST_MAX 70
#define TYPICAL_LAST_MIN 70
#define TYPICAL_LAST_MEAN 70
#define TYPICAL_LAST_STDEV 0
#define TYPICAL_LAST_SKEW 0
#define TYPICAL_LAST_KURT 0

#define TYPICAL_MAX 200
#define TYPICAL_MIN 0
#define TYPICAL_MEAN 45.8833
#define TYPICAL_STDEV 60.08025
#define TYPICAL_SKEW 1.3757
#define TYPICAL_KURT 3.745619643

#define INPUT_RISING \
    "1 1 2 3 3 4 5 6 6 7 8 9 9 10 20 20 30 40 50 50 53 58 60 62 64 68 70 80 " \
    "90 100 120 145 156 178 194 200 213 219 219 220 220 230 230 259 459 500 " \
    "560 600 612 630 640"

//No first or last peak for this

#define RISING_MAX 640
#define RISING_MIN 1
#define RISING_MEAN 152.8039
#define RISING_STDEV 186.0385
#define RISING_SKEW 1.4518
#define RISING_KURT 4.127892193

#define INPUT_FALLING \
    "800 800 700 700 600 600 590 590 580 580 580 540 420 420 420 419 419 419 " \
    "419 412 394 320 219 218 217 217 216 216 202 199 189 187 179 178 175 174 " \
    "140 120 80 50 40 30 20 10 5 2 2 2 2 2 1 1 1 0"

//No first or last peak for this

#define FALLING_MAX 800
#define FALLING_MIN 0
#define FALLING_MEAN 278.0741
#define FALLING_STDEV 238.2384
#define FALLING_SKEW 0.5008
#define FALLING_KURT 2.103732506

#define INPUT_CONST "70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 " \
    "70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 " \
    "70 70 70 70 70 70 70 70 70"

//No first or last peak for this

#define CONST_MAX 70
#define CONST_MIN 70
#define CONST_MEAN 70
#define CONST_STDEV 0
#define CONST_SKEW 0
#define CONST_KURT 0

/* Adjust these to tweak the acceptable error between expected & actual vals */
#define MAX_ERR 0.5
#define MEAN_ERR 0.5
#define STDEV_ERR 0.05
#define SKEW_ERR 0.05
#define KURT_ERR 0.05

class LidarDriverTest: public testing::Test
{
    public:
        std::vector<PulseData*> pulses;
        PulseData* pd;

    protected:
        //Peaks
        std::vector<Peak*> typicalPeaks;
        std::vector<Peak*> risingPeaks;
        std::vector<Peak*> fallingPeaks;
        std::vector<Peak*> constantPeaks;

        //Args
        char** commonArgSpace;
        int numberOfArgs;
        int maxLengthOfArg;

        //Some drivers to test with
        LidarDriver driver1;
        LidarDriver driver2;
        LidarDriver driver3;

        //Variables
        std::vector<char> vars = {'z', 'a', 'w', 'b'};

        /**
         * Creates a vector of Peak pointers for testing purposes. Artificially
         * @param input  List of numbers to turn into peaks
         * @returns vector of peak pointers.
         */
        std::vector<Peak*> makeTestPeaks(char *input)
        {
            std::vector<Peak*> peaks;
            char* ptr = strtok (input, " ");
            int i = 0;

            while (ptr != NULL) {
                int z0 = atoi(ptr);
                Peak* p = new Peak;

                //Sets peak up to be testable for all three of these variables
                p->z_activation = z0;
                p->amp = z0;
                p->fwhm = z0;
                p->backscatter_coefficient = z0;
                p->position_in_wave = 3;
                p->is_final_peak = false;
                peaks.push_back(p);
                i++;
                ptr = strtok (NULL," ");
            }
 
            return peaks;
        }

        /**
         * Frees vector of Peak pointers created by makeTestPeaks
         * @param peaks Peaks to remove
         */
        void freeTestPeaks(std::vector<Peak*> peaks)
        {
            std::vector<Peak*>::iterator i;

            for (i = peaks.begin(); i != peaks.end(); ++i) {
                delete *i;
            }
        }

        /**
         * Inhereted from GTest parent class
         */
        void SetUp()
        {
            char *typicalInput = (char*) malloc(sizeof(char) * 1028);
            char *risingInput = (char*) malloc(sizeof(char) * 1028);
            char *fallingInput = (char*) malloc(sizeof(char) * 1028);
            char *constantInput = (char*) malloc(sizeof(char) * 1028);

            strncpy(typicalInput, INPUT_TYPICAL, 1028);
            typicalInput[1028] = '\0';
            strncpy(risingInput, INPUT_RISING, 1028);
            risingInput[1028] = '\0';
            strncpy(fallingInput, INPUT_FALLING, 1028);
            fallingInput[1028] = '\0';
            strncpy(constantInput, INPUT_CONST, 1028);
            constantInput[1028] = '\0';

            typicalPeaks = makeTestPeaks(typicalInput);
            free(typicalInput);
            risingPeaks = makeTestPeaks(risingInput);
            free(risingInput);
            fallingPeaks = makeTestPeaks(fallingInput);
            free(fallingInput);
            constantPeaks = makeTestPeaks(constantInput);
            free(constantInput);

            // Adding first and last peak data to the typical peaks-- The rest
            // don't have any.
            typicalPeaks.at(TYPICAL_FIRST_PEAK)->position_in_wave = 1;
            typicalPeaks.at(TYPICAL_LAST_PEAK)->is_final_peak = true;
        }

        /**
         * Inhereted from Gtest parent class
         */
        void TearDown()
        {
            freeTestPeaks(typicalPeaks);
            freeTestPeaks(risingPeaks);
            freeTestPeaks(fallingPeaks);
            freeTestPeaks(constantPeaks);
        }
};

/*
 * For max, min, mean, std-dev, skewness and kurtosis, there will be a tests
 * for first, last and all peaks. Each of these tests will test with all the
 * inputs, and with all the peak variables (altitude, amplitude and width).
 */


/******************************************************************************
 *
 * Max
 *
 ******************************************************************************/
TEST_F(LidarDriverTest, get_max_first_test)
{
    for(auto it = vars.begin(); it != vars.end(); ++it){
        double val = driver1.get_extreme(&typicalPeaks,true,0,*it);
        EXPECT_NEAR(TYPICAL_FIRST_MAX,val,MAX_ERR);

        val = driver1.get_extreme(&risingPeaks,true,0,*it);
        EXPECT_NEAR(NO_DATA,val,MAX_ERR);

        val = driver1.get_extreme(&fallingPeaks,true,0,*it);
        EXPECT_NEAR(NO_DATA,val,MAX_ERR);

        val = driver1.get_extreme(&constantPeaks,true,0,*it);
        EXPECT_NEAR(NO_DATA,val,MAX_ERR);
    }
}

TEST_F(LidarDriverTest, get_max_last_test)
{
    for(auto it = vars.begin(); it != vars.end(); ++it){
        double val = driver1.get_extreme(&typicalPeaks,true,1,*it);
        EXPECT_NEAR(TYPICAL_LAST_MAX,val,MAX_ERR);

        val = driver1.get_extreme(&risingPeaks,true,1,*it);
        EXPECT_NEAR(NO_DATA,val,MAX_ERR);

        val = driver1.get_extreme(&fallingPeaks,true,1,*it);
        EXPECT_NEAR(NO_DATA,val,MAX_ERR);

        val = driver1.get_extreme(&constantPeaks,true,1,*it);
        EXPECT_NEAR(NO_DATA,val,MAX_ERR);
    }
}

TEST_F(LidarDriverTest, get_max_all_test)
{
    for(auto it = vars.begin(); it != vars.end(); ++it){
        double val = driver1.get_extreme(&typicalPeaks,true,2,*it);
        EXPECT_NEAR(TYPICAL_MAX,val,MAX_ERR);

        val = driver1.get_extreme(&risingPeaks,true,2,*it);
        EXPECT_NEAR(RISING_MAX,val,MAX_ERR);

        val = driver1.get_extreme(&fallingPeaks,true,2,*it);
        EXPECT_NEAR(FALLING_MAX,val,MAX_ERR);

        val = driver1.get_extreme(&constantPeaks,true,2,*it);
        EXPECT_NEAR(CONST_MAX,val,MAX_ERR);
    }
}


/******************************************************************************
 *
 * Min
 *
 ******************************************************************************/
TEST_F(LidarDriverTest, get_min_first_test)
{
    for(auto it = vars.begin(); it != vars.end(); ++it){
        double val = driver1.get_extreme(&typicalPeaks,false,0,*it);
        EXPECT_NEAR(TYPICAL_FIRST_MIN,val,MAX_ERR);

        val = driver1.get_extreme(&risingPeaks,false,0,*it);
        EXPECT_NEAR(NO_DATA,val,MAX_ERR);

        val = driver1.get_extreme(&fallingPeaks,false,0,*it);
        EXPECT_NEAR(NO_DATA,val,MAX_ERR);

        val = driver1.get_extreme(&constantPeaks,false,0,*it);
        EXPECT_NEAR(NO_DATA,val,MAX_ERR);
    }
}

TEST_F(LidarDriverTest, get_min_last_test)
{
    for(auto it = vars.begin(); it != vars.end(); ++it){
        double val = driver1.get_extreme(&typicalPeaks,false,1,*it);
        EXPECT_NEAR(TYPICAL_LAST_MIN,val,MAX_ERR);

        val = driver1.get_extreme(&risingPeaks,false,1,*it);
        EXPECT_NEAR(NO_DATA,val,MAX_ERR);

        val = driver1.get_extreme(&fallingPeaks,false,1,*it);
        EXPECT_NEAR(NO_DATA,val,MAX_ERR);

        val = driver1.get_extreme(&constantPeaks,false,1,*it);
        EXPECT_NEAR(NO_DATA,val,MAX_ERR);
    }
}

TEST_F(LidarDriverTest, get_min_all_test)
{
    for(auto it = vars.begin(); it != vars.end(); ++it){
        double val = driver1.get_extreme(&typicalPeaks,false,2,*it);
        EXPECT_NEAR(TYPICAL_MIN,val,MAX_ERR);

        val = driver1.get_extreme(&risingPeaks,false,2,*it);
        EXPECT_NEAR(RISING_MIN,val,MAX_ERR);

        val = driver1.get_extreme(&fallingPeaks,false,2,*it);
        EXPECT_NEAR(FALLING_MIN,val,MAX_ERR);

        val = driver1.get_extreme(&constantPeaks,false,2,*it);
        EXPECT_NEAR(CONST_MIN,val,MAX_ERR);
    }
}


/******************************************************************************
 *
 * Mean
 *
 ******************************************************************************/
TEST_F(LidarDriverTest, get_mean_first_test)
{
    for(auto it = vars.begin(); it != vars.end(); ++it){
        double val = driver1.get_mean(&typicalPeaks,0,*it);
        EXPECT_NEAR(TYPICAL_FIRST_MEAN,val,MEAN_ERR);

        val = driver1.get_mean(&risingPeaks,0,*it);
        EXPECT_NEAR(NO_DATA,val,MEAN_ERR);

        val = driver1.get_mean(&fallingPeaks,0,*it);
        EXPECT_NEAR(NO_DATA,val,MEAN_ERR);

        val = driver1.get_mean(&constantPeaks,0,*it);
        EXPECT_NEAR(NO_DATA,val,MEAN_ERR);
    }
}

TEST_F(LidarDriverTest, get_mean_last_test)
{
    for(auto it = vars.begin(); it != vars.end(); ++it){
        double val = driver1.get_mean(&typicalPeaks,1,*it);
        EXPECT_NEAR(TYPICAL_LAST_MEAN,val,MEAN_ERR);

        val = driver1.get_mean(&risingPeaks,1,*it);
        EXPECT_NEAR(NO_DATA,val,MEAN_ERR);

        val = driver1.get_mean(&fallingPeaks,1,*it);
        EXPECT_NEAR(NO_DATA,val,MEAN_ERR);

        val = driver1.get_mean(&constantPeaks,1,*it);
        EXPECT_NEAR(NO_DATA,val,MEAN_ERR);
    }
}

TEST_F(LidarDriverTest, get_mean_all_test)
{
    for(auto it = vars.begin(); it != vars.end(); ++it){
        double val = driver1.get_mean(&typicalPeaks,2,*it);
        EXPECT_NEAR(TYPICAL_MEAN,val,MEAN_ERR);

        val = driver1.get_mean(&risingPeaks,2,*it);
        EXPECT_NEAR(RISING_MEAN,val,MEAN_ERR);

        val = driver1.get_mean(&fallingPeaks,2,*it);
        EXPECT_NEAR(FALLING_MEAN,val,MEAN_ERR);

        val = driver1.get_mean(&constantPeaks,2,*it);
        EXPECT_NEAR(CONST_MEAN,val,MEAN_ERR);
    }
}

/******************************************************************************
 *
 * Standard Deviation
 *
 ******************************************************************************/
TEST_F(LidarDriverTest, get_deviation_first_test)
{
    for(auto it = vars.begin(); it != vars.end(); ++it){
        double mean = driver1.get_mean(&typicalPeaks,0,*it);
        double val = driver1.get_deviation(&typicalPeaks,mean,0,*it);
        EXPECT_NEAR(TYPICAL_FIRST_STDEV,val,STDEV_ERR);

        mean = driver1.get_mean(&risingPeaks,0,*it);
        val = driver1.get_deviation(&risingPeaks,mean,0,*it);
        EXPECT_NEAR(NO_DATA,val,STDEV_ERR);

        mean = driver1.get_mean(&fallingPeaks,0,*it);
        val = driver1.get_deviation(&fallingPeaks,mean,0,*it);
        EXPECT_NEAR(NO_DATA,val,STDEV_ERR);

        mean = driver1.get_mean(&constantPeaks,0,*it);
        val = driver1.get_deviation(&constantPeaks,mean,0,*it);
        EXPECT_NEAR(NO_DATA,val,STDEV_ERR);
    }
}

TEST_F(LidarDriverTest, get_deviation_last_test)
{
    for(auto it = vars.begin(); it != vars.end(); ++it){
        double mean = driver1.get_mean(&typicalPeaks,1,*it);
        double val = driver1.get_deviation(&typicalPeaks,mean,1,*it);
        EXPECT_NEAR(TYPICAL_LAST_STDEV,val,STDEV_ERR);

        mean = driver1.get_mean(&risingPeaks,1,*it);
        val = driver1.get_deviation(&risingPeaks,mean,1,*it);
        EXPECT_NEAR(NO_DATA,val,STDEV_ERR);

        mean = driver1.get_mean(&fallingPeaks,1,*it);
        val = driver1.get_deviation(&fallingPeaks,mean,1,*it);
        EXPECT_NEAR(NO_DATA,val,STDEV_ERR);

        mean = driver1.get_mean(&constantPeaks,1,*it);
        val = driver1.get_deviation(&constantPeaks,mean,1,*it);
        EXPECT_NEAR(NO_DATA,val,STDEV_ERR);
    }
}

TEST_F(LidarDriverTest, get_deviation_all_test)
{
    for(auto it = vars.begin(); it != vars.end(); ++it){
        double mean = driver1.get_mean(&typicalPeaks,2,*it);
        double val = driver1.get_deviation(&typicalPeaks,mean,2,*it);
        EXPECT_NEAR(TYPICAL_STDEV,val,STDEV_ERR);

        mean = driver1.get_mean(&risingPeaks,2,*it);
        val = driver1.get_deviation(&risingPeaks,mean,2,*it);
        EXPECT_NEAR(RISING_STDEV,val,STDEV_ERR);

        mean = driver1.get_mean(&fallingPeaks,2,*it);
        val = driver1.get_deviation(&fallingPeaks,mean,2,*it);
        EXPECT_NEAR(FALLING_STDEV,val,STDEV_ERR);

        mean = driver1.get_mean(&constantPeaks,2,*it);
        val = driver1.get_deviation(&constantPeaks,mean,2,*it);
        EXPECT_NEAR(CONST_STDEV,val,STDEV_ERR);
    }
}

/******************************************************************************
 *
 * Skewness
 *
 ******************************************************************************/
TEST_F(LidarDriverTest, get_skewness_first_test)
{
   for(auto it = vars.begin(); it != vars.end(); ++it){
        double mean = driver1.get_mean(&typicalPeaks,0,*it);
        double stdev = driver1.get_deviation(&typicalPeaks,mean,0,*it);
        double val = driver1.get_skewtosis(&typicalPeaks,mean,stdev,0,*it,3);
        EXPECT_NEAR(TYPICAL_FIRST_SKEW,val,SKEW_ERR);

        mean = driver1.get_mean(&risingPeaks,0,*it);
        stdev = driver1.get_deviation(&risingPeaks,mean,0,*it);
        val = driver1.get_skewtosis(&risingPeaks,mean,stdev,0,*it,3);
        EXPECT_NEAR(NO_DATA,val,SKEW_ERR);

        mean = driver1.get_mean(&fallingPeaks,0,*it);
        stdev = driver1.get_deviation(&fallingPeaks,mean,0,*it);
        val = driver1.get_skewtosis(&fallingPeaks,mean,stdev,0,*it,3);
        EXPECT_NEAR(NO_DATA,val,SKEW_ERR);

        mean = driver1.get_mean(&constantPeaks,0,*it);
        stdev = driver1.get_deviation(&constantPeaks,mean,0,*it);
        val = driver1.get_skewtosis(&constantPeaks,mean,stdev,0,*it,3);
        EXPECT_NEAR(NO_DATA,val,SKEW_ERR);
    }
}

TEST_F(LidarDriverTest, get_skewness_last_test)
{
    for(auto it = vars.begin(); it != vars.end(); ++it){
        double mean = driver1.get_mean(&typicalPeaks,1,*it);
        double stdev = driver1.get_deviation(&typicalPeaks,mean,1,*it);
        double val = driver1.get_skewtosis(&typicalPeaks,mean,stdev,1,*it,3);
        EXPECT_NEAR(TYPICAL_LAST_SKEW,val,SKEW_ERR);

        mean = driver1.get_mean(&risingPeaks,1,*it);
        stdev = driver1.get_deviation(&risingPeaks,mean,1,*it);
        val = driver1.get_skewtosis(&risingPeaks,mean,stdev,1,*it,3);
        EXPECT_NEAR(NO_DATA,val,SKEW_ERR);

        mean = driver1.get_mean(&fallingPeaks,1,*it);
        stdev = driver1.get_deviation(&fallingPeaks,mean,1,*it);
        val = driver1.get_skewtosis(&fallingPeaks,mean,stdev,1,*it,3);
        EXPECT_NEAR(NO_DATA,val,SKEW_ERR);

        mean = driver1.get_mean(&constantPeaks,1,*it);
        stdev = driver1.get_deviation(&constantPeaks,mean,1,*it);
        val = driver1.get_skewtosis(&constantPeaks,mean,stdev,1,*it,3);
        EXPECT_NEAR(NO_DATA,val,SKEW_ERR);
    }
}

TEST_F(LidarDriverTest, get_skewness_all_test)
{
    for(auto it = vars.begin(); it != vars.end(); ++it){
        double mean = driver1.get_mean(&typicalPeaks,2,*it);
        double stdev = driver1.get_deviation(&typicalPeaks,mean,2,*it);
        double val = driver1.get_skewtosis(&typicalPeaks,mean,stdev,2,*it,3);
        EXPECT_NEAR(TYPICAL_SKEW,val,SKEW_ERR);

        mean = driver1.get_mean(&risingPeaks,2,*it);
        stdev = driver1.get_deviation(&risingPeaks,mean,2,*it);
        val = driver1.get_skewtosis(&risingPeaks,mean,stdev,2,*it,3);
        EXPECT_NEAR(RISING_SKEW,val,SKEW_ERR);

        mean = driver1.get_mean(&fallingPeaks,2,*it);
        stdev = driver1.get_deviation(&fallingPeaks,mean,2,*it);
        val = driver1.get_skewtosis(&fallingPeaks,mean,stdev,2,*it,3);
        EXPECT_NEAR(FALLING_SKEW,val,SKEW_ERR);

        mean = driver1.get_mean(&constantPeaks,2,*it);
        stdev = driver1.get_deviation(&constantPeaks,mean,2,*it);
        val = driver1.get_skewtosis(&constantPeaks,mean,stdev,2,*it,3);
        EXPECT_NEAR(CONST_SKEW,val,SKEW_ERR);
    }
}

/******************************************************************************
 *
 * Kurtosis
 *
 ******************************************************************************/
TEST_F(LidarDriverTest, get_kurtosis_first_test)
{
   for(auto it = vars.begin(); it != vars.end(); ++it){
        double mean = driver1.get_mean(&typicalPeaks,0,*it);
        double stdev = driver1.get_deviation(&typicalPeaks,mean,0,*it);
        double val = driver1.get_skewtosis(&typicalPeaks,mean,stdev,0,*it,4);
        EXPECT_NEAR(TYPICAL_FIRST_KURT,val,KURT_ERR);

        mean = driver1.get_mean(&risingPeaks,0,*it);
        stdev = driver1.get_deviation(&risingPeaks,mean,0,*it);
        val = driver1.get_skewtosis(&risingPeaks,mean,stdev,0,*it,4);
        EXPECT_NEAR(NO_DATA,val,KURT_ERR);

        mean = driver1.get_mean(&fallingPeaks,0,*it);
        stdev = driver1.get_deviation(&fallingPeaks,mean,0,*it);
        val = driver1.get_skewtosis(&fallingPeaks,mean,stdev,0,*it,4);
        EXPECT_NEAR(NO_DATA,val,KURT_ERR);

        mean = driver1.get_mean(&constantPeaks,0,*it);
        stdev = driver1.get_deviation(&constantPeaks,mean,0,*it);
        val = driver1.get_skewtosis(&constantPeaks,mean,stdev,0,*it,4);
        EXPECT_NEAR(NO_DATA,val,KURT_ERR);
    }
}

TEST_F(LidarDriverTest, get_kurtosis_last_test)
{
    for(auto it = vars.begin(); it != vars.end(); ++it){
        double mean = driver1.get_mean(&typicalPeaks,1,*it);
        double stdev = driver1.get_deviation(&typicalPeaks,mean,1,*it);
        double val = driver1.get_skewtosis(&typicalPeaks,mean,stdev,1,*it,4);
        EXPECT_NEAR(TYPICAL_LAST_KURT,val,KURT_ERR);

        mean = driver1.get_mean(&risingPeaks,1,*it);
        stdev = driver1.get_deviation(&risingPeaks,mean,1,*it);
        val = driver1.get_skewtosis(&risingPeaks,mean,stdev,1,*it,4);
        EXPECT_NEAR(NO_DATA,val,KURT_ERR);

        mean = driver1.get_mean(&fallingPeaks,1,*it);
        stdev = driver1.get_deviation(&fallingPeaks,mean,1,*it);
        val = driver1.get_skewtosis(&fallingPeaks,mean,stdev,1,*it,4);
        EXPECT_NEAR(NO_DATA,val,KURT_ERR);
        
        mean = driver1.get_mean(&constantPeaks,1,*it);
        stdev = driver1.get_deviation(&constantPeaks,mean,1,*it);
        val = driver1.get_skewtosis(&constantPeaks,mean,stdev,1,*it,4);
        EXPECT_NEAR(NO_DATA,val,KURT_ERR);
    }
}

TEST_F(LidarDriverTest, get_kurtosis_all_test)
{
    for(auto it = vars.begin(); it != vars.end(); ++it){
        double mean = driver1.get_mean(&typicalPeaks,2,*it);
        double stdev = driver1.get_deviation(&typicalPeaks,mean,2,*it);
        double val = driver1.get_skewtosis(&typicalPeaks,mean,stdev,2,*it,4);
        EXPECT_NEAR(TYPICAL_KURT,val,KURT_ERR);

        mean = driver1.get_mean(&risingPeaks,2,*it);
        stdev = driver1.get_deviation(&risingPeaks,mean,2,*it);
        val = driver1.get_skewtosis(&risingPeaks,mean,stdev,2,*it,4);
        EXPECT_NEAR(RISING_KURT,val,KURT_ERR);

        mean = driver1.get_mean(&fallingPeaks,2,*it);
        stdev = driver1.get_deviation(&fallingPeaks,mean,2,*it);
        val = driver1.get_skewtosis(&fallingPeaks,mean,stdev,2,*it,4);
        EXPECT_NEAR(FALLING_KURT,val,KURT_ERR);

        mean = driver1.get_mean(&constantPeaks,2,*it);
        stdev = driver1.get_deviation(&constantPeaks,mean,2,*it);
        val = driver1.get_skewtosis(&constantPeaks,mean,stdev,2,*it,4);
        EXPECT_NEAR(CONST_KURT,val,KURT_ERR);
    }
}

/******************************************************************************
 *
 * Test 4 *lot of redundancies with FlightLineData_unittests in this test
 *
 ******************************************************************************/

TEST_F(LidarDriverTest, setup_flight_data_test)
{
    // Known FlightLineData
    int known_scanner_id = 1;
    double known_wave_length = 1064;
    double known_outgoing_pulse_width = 4.822761;
    int known_scan_pattern = 2;
    int known_numer_of_mirror_facets = 4;
    double known_scan_frequency = 146.666672;
    double known_scan_angle_min = 0.00001;
    double known_scan_angle_max = 359.999969;
    double known_pulse_frequency = 400;
    double known_beam_diameter_at_exit_aperture = 10;
    double known_beam_divergence = 0.5;
    double known_minimal_range = 660.48053;
    double known_maximal_range = 1860.605225;

    double known_bb_x_min = 516209.586;
    double known_bb_y_min = 4767921.375;
    double known_bb_z_min = 2084.585;
    double known_bb_x_max = 516211.942;
    double known_bb_y_max = 4767923.621;
    double known_bb_z_max = 2093.581;

    FlightLineData fld;
    std::string filename;
    filename = "etc/140823_183115_1_clipped_test.pls";

    EXPECT_NO_THROW(driver1.setup_flight_data(fld,filename));

    EXPECT_EQ(fld.hasNextPulse(),true);

    EXPECT_EQ (known_scanner_id, fld.scanner_id);
    EXPECT_DOUBLE_EQ (known_wave_length, fld.wave_length);
    EXPECT_NEAR (known_outgoing_pulse_width, fld.outgoing_pulse_width,
            0.0000005);
    EXPECT_EQ (known_scan_pattern, fld.scan_pattern);
    EXPECT_EQ (known_numer_of_mirror_facets, fld.number_of_mirror_facets);
    EXPECT_NEAR (known_scan_frequency, fld.scan_frequency, 0.000001);
    EXPECT_NEAR (known_scan_angle_min, fld.scan_angle_min, 0.0000001);
    EXPECT_NEAR (known_scan_angle_max, fld.scan_angle_max, 0.0000005);
    EXPECT_DOUBLE_EQ (known_pulse_frequency, fld.pulse_frequency);
    EXPECT_DOUBLE_EQ (known_beam_diameter_at_exit_aperture,
            fld.beam_diameter_at_exit_aperture);
    EXPECT_NEAR (known_beam_divergence, fld.beam_divergence, 0.01);
    EXPECT_NEAR (known_minimal_range, fld.minimal_range, 0.00001);
    EXPECT_NEAR (known_maximal_range, fld.maximal_range, 0.000001);

    EXPECT_DOUBLE_EQ (known_bb_x_min, fld.bb_x_min);
    EXPECT_DOUBLE_EQ (known_bb_y_min, fld.bb_y_min);
    EXPECT_DOUBLE_EQ (known_bb_z_min, fld.bb_z_min);
    EXPECT_DOUBLE_EQ (known_bb_x_max, fld.bb_x_max);
    EXPECT_DOUBLE_EQ (known_bb_y_max, fld.bb_y_max);
    EXPECT_DOUBLE_EQ (known_bb_z_max, fld.bb_z_max);
}


/******************************************************************************
 *
 * Test 5
 *
 ******************************************************************************/
TEST_F(LidarDriverTest, add_peaks_to_volume_test)
{
    std::vector<Peak*> peaks;
    LidarVolume lidarVolume;
    FlightLineData fld;
    std::string filename;
    filename = "etc/140823_183115_1_clipped_test.pls";

    EXPECT_NO_THROW(driver1.setup_flight_data(fld,filename));


    double known_amp = 142;
    double known_loc = 19;
    double known_fwhm = 5;
    double known_fwhm_t_pos = 21;
    double known_fwhm_t_neg = 16;
    int known_pos = 0;
    int known_triggering_amp = 13;
    int known_triggering_loc = 14;
    double known_x_act = 516211;
    double known_y_act = 4767923;
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

    peaks.push_back(&p);
    EXPECT_NO_THROW(driver1.setup_lidar_volume(fld,lidarVolume));
    EXPECT_NO_THROW(driver1.add_peaks_to_volume(lidarVolume,peaks,1));

    EXPECT_EQ(peaks.at(0)->amp, lidarVolume.volume[5]->at(0)->amp);
    EXPECT_EQ(peaks.at(0)->location, lidarVolume.volume[5]->at(0)->location);
    EXPECT_EQ(peaks.at(0)->fwhm, lidarVolume.volume[5]->at(0)->fwhm);
    EXPECT_EQ(peaks.at(0)->fwhm_t_positive, lidarVolume.volume[5]->
            at(0)->fwhm_t_positive);
    EXPECT_EQ(peaks.at(0)->fwhm_t_negative, lidarVolume.volume[5]->
            at(0)->fwhm_t_negative);
    EXPECT_EQ(peaks.at(0)->position_in_wave, lidarVolume.volume[5]->
            at(0)->position_in_wave);
    EXPECT_EQ(peaks.at(0)->triggering_amp, lidarVolume.volume[5]->
            at(0)->triggering_amp);
    EXPECT_EQ(peaks.at(0)->triggering_location, lidarVolume.volume[5]->
            at(0)->triggering_location);
    EXPECT_EQ(peaks.at(0)->x_activation, lidarVolume.volume[5]->
            at(0)->x_activation);
    EXPECT_EQ(peaks.at(0)->y_activation, lidarVolume.volume[5]->
            at(0)->y_activation);
    EXPECT_EQ(peaks.at(0)->z_activation, lidarVolume.volume[5]->
            at(0)->z_activation);
}

