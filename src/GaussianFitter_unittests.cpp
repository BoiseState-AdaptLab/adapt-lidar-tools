//File name: GaussianFitter_unittests.cpp
//Created on: 13-October-2017
//Author: ravi

#include "GaussianFitter.hpp"
#include "PulseData.hpp"
#include "pulsereader.hpp"
#include "pulsewriter.hpp"
#include "gtest/gtest.h"
#include "Peak.hpp"
#include <fstream>

#define MAX_ITER 200

class GaussianFitterTest: public testing::Test{
    public:
        std::vector<PulseData*> pulses;
        PulseData* pd;
    GaussianFitter fitter;
    //    GaussianFitter fitter;
    //    fitter.noise_level = 9;

    protected:

        //Function to set up space used by all tests
        virtual void SetUp(){
            fitter.noise_level = 9;
        }
        
        int parseWave(char *input, 
                      std::vector<int> &idxData, std::vector<int> &ampData){
        
        char* ptr;
        ptr = strtok (input," ");
        int i=0;
        while (ptr != NULL){
            int y0 = atoi(ptr);
            ampData.push_back(y0);
            idxData.push_back(i);
            i++;
            ptr = strtok (NULL," ");
         }
         return 0;
       }
};

/****************************************************************************
 *
 * Test gaussianFitter() method on Nayani_clipped_test_1.pls
 *
 ****************************************************************************/

        ///////////////////////////
        // TESTING guess_peaks() //
        ///////////////////////////

TEST_F(GaussianFitterTest, NayaniClipped1_guess){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 2 1 1 0 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 "
        "179 160 139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11 "
        "8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

    parseWave(input, idxData, ampData);
    
    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    ASSERT_EQ(3,peaks.size());
    EXPECT_EQ(200,peaks.at(0)->amp);
    EXPECT_NEAR(18.5, peaks.at(0)->location, .25);
    EXPECT_NEAR(10.5, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(43 ,peaks.at(1)->amp);
    EXPECT_NEAR(30.5, peaks.at(1)->location,.25);
    EXPECT_NEAR(4, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(34 ,peaks.at(2)->amp);
    EXPECT_EQ(38, peaks.at(2)->location);
    EXPECT_NEAR(6, peaks.at(2)->fwhm, 1);

    EXPECT_EQ(3, count);

}

TEST_F(GaussianFitterTest, NayaniClipped2_guess){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 3 2 2 3 2 2 1 1 1 2 7 22 58 114 174 216 235 235 221 195 155 110 67 39 24 18 16 15 15 15 14 11 10 9 8 7 6 5 5 4 3 3 4 5 4 4 3 3 1 2 1 2 3 3 4 4 5 4 2";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    ASSERT_EQ(2,peaks.size());
    EXPECT_EQ(235,peaks.at(0)->amp);
    EXPECT_NEAR(17.5, peaks.at(0)->location,1);
    EXPECT_NEAR(7.8, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(29, peaks.at(1)->location);
    EXPECT_EQ(15,peaks.at(1)->amp);
    EXPECT_NEAR(14, peaks.at(1)->fwhm, 1);
    
    EXPECT_EQ(2, count);
}

TEST_F(GaussianFitterTest, gaussianFitter_guess){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;
    char input[] = "2 2 2 1 1 1 1 1 1 0 0 1 9 35 88 155 212 240 237 200 145 87 42 18 12 13 14 15 15 14 13 10 8 8 8 8 7 6 6 4 4 4 3 4 5 6 4 4 3 2 2 1 1 0 1 2 3 4 4 2";

    parseWave(input, idxData, ampData);

    //the noise level for this waveform is 21.6
    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    ASSERT_EQ(2,peaks.size());
    EXPECT_EQ(240,peaks.at(0)->amp);
    EXPECT_EQ(17, peaks.at(0)->location);
    EXPECT_NEAR(6, peaks.at(0)->fwhm,1);

    EXPECT_EQ(15,peaks.at(1)->amp);
    EXPECT_NEAR(27.5, peaks.at(1)->location,0.25);
    EXPECT_NEAR(16, peaks.at(1)->fwhm,1);
    EXPECT_EQ(2, count);

}

TEST_F(GaussianFitterTest, NayaniClipped3_guess){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 3 4 3 3 2 1 1 0 1 1 1 1 1 6 29 79 147 207 238 235 198 141 86 43 20 11 11 13 15 15 15 13 11 8 5 5 4 6 6 7 7 5 4 4 3 4 4 5 4 3 3 2 1 1 3 3 3 2 3";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(238,peaks.at(0)->amp);
    EXPECT_EQ(19, peaks.at(0)->location);
    EXPECT_NEAR(5.9, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(15,peaks.at(1)->amp);
    EXPECT_EQ(30, peaks.at(1)->location);
    EXPECT_NEAR(9, peaks.at(1)->fwhm, 1.5);
    EXPECT_EQ(2, count);
}


TEST_F(GaussianFitterTest, NayaniClipped4_guess){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 2 3 2 2 2 2 1 0 1 1 2 2 4 16 49 105 170 217 239 240 223 "
        "195 155 110 68 40 25 20 16 16 15 12 10 7 7 7 7 6 5 4 4 3 2 2 3 5 4 4 "
        "3 2 3 4 4 3 3 2 2 2 2";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    ASSERT_EQ(2,peaks.size());
    EXPECT_EQ(240,peaks.at(0)->amp);
    EXPECT_EQ(20, peaks.at(0)->location);
    EXPECT_NEAR(7.4, peaks.at(0)->fwhm, 1);
   
    EXPECT_EQ(16,peaks.at(1)->amp);
    EXPECT_NEAR(29.5, peaks.at(1)->location,.25);
    EXPECT_NEAR(8, peaks.at(1)->fwhm, 1);
    EXPECT_EQ(2, count);

}

TEST_F(GaussianFitterTest, NayaniClipped5_guess){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 1 1 1 0 1 3 2 3 4 5 7 16 38 72 109 136 150 150 140 118 "
        "87 56 32 23 30 53 82 111 133 145 146 135 121 106 96 87 71 52 35 23 19 "
        "16 14 10 8 8 7 6 6 4 4 4 5 5 6 5 4 3 1";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(150,peaks.at(0)->amp);
    EXPECT_NEAR(17.5, peaks.at(0)->location,.25);
    EXPECT_NEAR(7.3, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(146,peaks.at(1)->amp);
    EXPECT_EQ(31, peaks.at(1)->location);
    EXPECT_NEAR(10.2, peaks.at(1)->fwhm, 1);
 
    EXPECT_EQ(2, count);   
}

TEST_F(GaussianFitterTest, NayaniClipped6_guess){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 1 1 1 2 3 2 2 2 3 5 8 10 11 10 8 7 9 14 28 51 88 131 171 193 193 173 140 117 117 135 151 148 122 88 58 37 23 16 12 11 12 12 12 10 10 10 10 10 8 6 5 5 4 4 3 3 4 3 2";

    parseWave(input, idxData, ampData);
    
    //the noise level for this waveform is 17.4
    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    ASSERT_EQ(4,peaks.size());
    EXPECT_EQ(11,peaks.at(0)->amp);
    EXPECT_EQ(13, peaks.at(0)->location);
    EXPECT_NEAR(6, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(193,peaks.at(1)->amp);
    EXPECT_NEAR(24.5, peaks.at(1)->location,.25);
    EXPECT_NEAR(7.6, peaks.at(1)->fwhm, 1.25);

    EXPECT_EQ(151,peaks.at(2)->amp);
    EXPECT_EQ(31, peaks.at(2)->location);
    EXPECT_NEAR(6.6, peaks.at(2)->fwhm, 1);

    EXPECT_EQ(12, peaks.at(3)->amp);
    EXPECT_EQ(42, peaks.at(3)->location);
    EXPECT_NEAR(15, peaks.at(3)->fwhm, 1.5);

    EXPECT_EQ(4, count);
}

TEST_F(GaussianFitterTest, NayaniClipped7_guess){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 0 1 0 0 1 2 1 2 1 1 1 2 8 19 39 65 87 98 97 89 82 76 79 93 114 130 137 141 152 165 168 153 119 77 46 26 19 15 13 13 13 13 11 10 8 6 6 5 4 4 3 2 2 2 2 3 4 4 4";

    parseWave(input, idxData, ampData);


    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    fitter.noise_level = 9;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    ASSERT_EQ(3,peaks.size());
    EXPECT_EQ(98,peaks.at(0)->amp);
    EXPECT_EQ(18, peaks.at(0)->location);
    EXPECT_NEAR(5.2, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(168,peaks.at(1)->amp);
    EXPECT_EQ(31, peaks.at(1)->location);
    EXPECT_NEAR(10, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(13,peaks.at(2)->amp);
    EXPECT_NEAR(40.5, peaks.at(2)->location,.25);
    EXPECT_NEAR(11, peaks.at(2)->fwhm, 1);
  
    EXPECT_EQ(3, count);

}


TEST_F(GaussianFitterTest, NayaniClipped8_guess){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "4 3 2 2 1 1 1 1 1 3 3 6 11 17 23 26 26 25 27 42 76 124 170 "
        "200 211 213 215 219 221 209 178 133 90 60 42 30 23 20 21 20 19 16 13 "
        "11 9 6 5 4 3 3 3 3 4 6 6 6 4 3 1 2";

    parseWave(input, idxData, ampData);


    //the noise level for this waveform is 19.9
    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    ASSERT_EQ(3,peaks.size());
    EXPECT_EQ(26,peaks.at(0)->amp);
    EXPECT_NEAR(15.5,peaks.at(0)->location,0.25);
    EXPECT_NEAR(9,peaks.at(0)->fwhm, 1);

    EXPECT_EQ(221,peaks.at(1)->amp);
    EXPECT_EQ(28,peaks.at(1)->location);
    EXPECT_NEAR(10.8,peaks.at(1)->fwhm, 1);

    EXPECT_EQ(21,peaks.at(2)->amp);
    EXPECT_EQ(38,peaks.at(2)->location);
    EXPECT_NEAR(10.2,peaks.at(2)->fwhm, 1);

    EXPECT_EQ(3, count);
}

/****************************************************************************
 *
 * Test gaussianFitter() method on 140823_152425_1.pls
 *
 ****************************************************************************/

//Exceeding max no of iterations
TEST_F(GaussianFitterTest, max_iter_1_guess){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 2 2 2 2 3 2 3 3 2 2 3 4 5 15 39 80 124 150 147 119 80 46 "
        "23 10 6 4 5 7 9 8 6 5 4 2 2 2 2 3 4 4 4 3 2 1 3 4 4 3 3 2 2 3 3 5 10 "
        "18 23 25 23";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    ASSERT_EQ(2,peaks.size());
    EXPECT_EQ(150,peaks.at(0)->amp);
    EXPECT_EQ(18, peaks.at(0)->location);
    EXPECT_NEAR(5.3, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(25,peaks.at(1)->amp);
    EXPECT_EQ(58, peaks.at(1)->location);
    EXPECT_NEAR(5.4, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2, count);

}

//Exceeding max no of iterations
TEST_F(GaussianFitterTest, max_iter_2_guess){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "3 3 2 2 2 2 3 2 4 4 4 6 18 47 89 125 139 124 89 53 25 9 3 "
        "2 3 6 8 10 9 7 5 4 3 3 1 1 2 3 3 4 4 3 2 2 2 2 2 3 2 2 3 2 2 3 4 8 16 "
        "23 26 25";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(3,peaks.size());
    EXPECT_EQ(139,peaks.at(0)->amp);
    EXPECT_EQ(16, peaks.at(0)->location);
    EXPECT_NEAR(5, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(10,peaks.at(1)->amp);
    EXPECT_EQ(27, peaks.at(1)->location);
    EXPECT_NEAR(5.2, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(26,peaks.at(2)->amp);
    EXPECT_EQ(58, peaks.at(2)->location);
    EXPECT_NEAR(4.6, peaks.at(2)->fwhm, 1);
    
    EXPECT_EQ(3, count);
}
//Exceeding max no of iterations
TEST_F(GaussianFitterTest, max_iter_3_guess){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "3 2 2 1 1 2 3 4 4 4 4 3 5 12 35 77 127 161 164 136 93 53 "
        "25 11 6 4 6 8 10 11 10 8 7 5 4 3 2 2 3 3 3 5 5 4 4 2 2 2 1 2 2 3 4 8 "
        "15 24 32 33 27 19";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);
    fitter.guess_peaks(&peaks, ampData, idxData);
    EXPECT_EQ(3,peaks.size());
    EXPECT_EQ(164,peaks.at(0)->amp);
    EXPECT_EQ(11,peaks.at(1)->amp);
    EXPECT_EQ(33,peaks.at(2)->amp);
    EXPECT_EQ(18,peaks.at(0)->location);
    EXPECT_EQ(29,peaks.at(1)->location);
    EXPECT_EQ(57,peaks.at(2)->location);
    EXPECT_NEAR(5.1, peaks.at(0)->fwhm, 1); 
    EXPECT_NEAR(8, peaks.at(1)->fwhm, 1); 
    EXPECT_NEAR(5.8, peaks.at(2)->fwhm, 1); 

    EXPECT_EQ(3,count);
}

//Exceeding max no of iterations
TEST_F(GaussianFitterTest, max_iter_4_guess){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 3 2 2 1 1 1 1 1 2 2 6 8 17 29 48 68 81 88 87 86 86 84 73 "
        "57 40 30 28 29 31 32 34 33 32 28 25 22 19 16 13 12 10 8 8 8 8 9 9 11 "
        "15 19 20 18 14 9 6 6 4 4 5 3 4 4 3 4 5 4 4 5 4 3 2 2 1 1 3 3 3 3 3 4 "
        "5 5 5 3 3 2 2 1 1 1 2 2 1 1 1 3 2 3 2 2 2 2 3 2 2 2 1 2 4 4 4 4 2 1 1 "
        "2 3 4 4";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(4,peaks.size());
    EXPECT_EQ(88,peaks.at(0)->amp);
    EXPECT_EQ(18,peaks.at(0)->location);
    EXPECT_NEAR(9.9, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(86,peaks.at(1)->amp);
    EXPECT_EQ(20.5,peaks.at(1)->location);
    EXPECT_NEAR(7.2, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(34,peaks.at(2)->amp);
    EXPECT_EQ(31,peaks.at(2)->location);
    EXPECT_NEAR(13.4, peaks.at(2)->fwhm, 1);

    EXPECT_EQ(20,peaks.at(3)->amp);
    EXPECT_EQ(51,peaks.at(3)->location);
    EXPECT_NEAR(6.3, peaks.at(3)->fwhm, 1);
 
    EXPECT_EQ(4,count);
}

//Exceeding max no of iterations
TEST_F(GaussianFitterTest, max_iter_5_guess){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 3 2 2 1 1 1 1 1 2 2 6 8 17 29 48 68 81 88 87 86 86 84 73 "
        "57 40 30 28 29 31 32 34 33 32 28 25 22 19 16 13 12 10 8 9 8 8 9 9 11 "
        "15 19 20 18 14 9 6 6 4 4 5 3 4 4 3 4 5 4 4 5 4 3 2 2 1 1 3 3 3 3 3 4 "
        "5 5 5 3 3 2 2 1 1 1 2 2 1 1 1 3 2 3 2 2 2 2 3 2 2 2 1 2 4 4 4 4 2 1 1 "
        "2 3 4 4 2 1 2 2 3 4 4 4 3 2 3 5 7 13 20 24 25 21 14 9 8 10 12 13 12 8 "
        "5 4 3 3 3 4 8 21 46 81 114 132 130 110 82 55 34 21 13 10 9 11 11 11 "
        "12 14 17 21 22 21 15 10 6 3 2 3 3 4 3 4 4 4 4 3 2 1 1 2 2 3 4 3 3 3 2 "
        "2 1 2 2 3 3 4 4 4 3 2 1 2 2 4 4 2 1 2 2 1 1 1 2 2 2 2 1 2 3 4 4 5 5 4 "
        "4 2 2 2";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(8,peaks.size());
    EXPECT_EQ(88,peaks.at(0)->amp);
    EXPECT_EQ(18,peaks.at(0)->location);
    EXPECT_NEAR(10.1, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(86,peaks.at(1)->amp);
    EXPECT_EQ(20.5,peaks.at(1)->location);
    EXPECT_NEAR(7.2, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(34,peaks.at(2)->amp);
    EXPECT_EQ(31,peaks.at(2)->location);
    EXPECT_NEAR(11.8, peaks.at(2)->fwhm, 1);

    EXPECT_EQ(20,peaks.at(3)->amp);
    EXPECT_EQ(51,peaks.at(3)->location);
    EXPECT_NEAR(6.3, peaks.at(3)->fwhm, 1);

    EXPECT_EQ(25,peaks.at(4)->amp);
    EXPECT_EQ(136,peaks.at(4)->location);
    EXPECT_NEAR(5.4, peaks.at(4)->fwhm, 1);

    EXPECT_EQ(13,peaks.at(5)->amp);
    EXPECT_EQ(143,peaks.at(5)->location);
    EXPECT_NEAR(5.2, peaks.at(5)->fwhm, 1);

    EXPECT_EQ(132,peaks.at(6)->amp);
    EXPECT_EQ(157,peaks.at(6)->location);
    EXPECT_NEAR(6, peaks.at(6)->fwhm, 1);

    EXPECT_EQ(22,peaks.at(7)->amp);
    EXPECT_EQ(174,peaks.at(7)->location);
    EXPECT_NEAR(7.9, peaks.at(7)->fwhm, 1);

    EXPECT_EQ(8,count);
}

//Triggering location: -2147483648 not in range: 60
//Using file 140823_152425_1.pls
TEST_F(GaussianFitterTest, trig_loc_1_guess){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 1 0 2 2 3 2 2 1 2 2 1 2 6 22 55 101 148 175 183 176 171 "
        "172 167 146 112 73 45 30 21 17 14 14 13 12 10 7 6 6 6 5 4 4 5 4 4 3 3 "
        "2 3 2 3 2 1 1 2 2 3 4 2";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(3,peaks.size());
    EXPECT_EQ(183,peaks.at(0)->amp);
    EXPECT_EQ(19,peaks.at(0)->location);
    EXPECT_NEAR(9.8, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(172,peaks.at(1)->amp);
    EXPECT_EQ(22,peaks.at(1)->location);
    EXPECT_NEAR(10, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(14,peaks.at(2)->amp);
    EXPECT_EQ(31.5,peaks.at(2)->location);
    EXPECT_NEAR(8, peaks.at(2)->fwhm, 1);


    EXPECT_EQ(3, count);
}

//Triggering location: -2147483648 not in range: 60
//Using file 140823_152425_1.pls
TEST_F(GaussianFitterTest, trig_loc_2_guess){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 0 0 1 1 1 1 2 2 2 3 4 5 13 28 56 91 124 143 141 125 112 "
        "114 127 135 127 102 74 49 31 19 14 10 10 10 10 8 6 5 4 4 4 4 4 4 4 3 "
        "3 3 4 3 3 2 2 1 0 0 1 2 1";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(3,peaks.size());
    EXPECT_EQ(143, peaks.at(0)->amp);
    EXPECT_EQ(18, peaks.at(0)->location);
    EXPECT_NEAR(5.2, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(135, peaks.at(1)->amp);
    EXPECT_EQ(24, peaks.at(1)->location);
    EXPECT_NEAR(6.4, peaks.at(1)->fwhm, 1);
   
    EXPECT_EQ(10, peaks.at(2)->amp);
    EXPECT_EQ(35, peaks.at(2)->location);
    EXPECT_NEAR(9, peaks.at(2)->fwhm, 1);

    EXPECT_EQ(3,count);
}

//Test gaussian fitting iterations
//Using file 140823_152425_1.pls
TEST_F(GaussianFitterTest, num_iterations_10_guess){
    
    std::vector<int> idxData;
    std::vector<int> ampData;
    
    char input[] = "26 36 37 30 21 22 47 96 153 190 186 147 94 49 21 7 3 4 4 3 3 2 1 0 0 0 0 0";

    parseWave(input, idxData, ampData);

    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(2, peaks.size());
    EXPECT_EQ(37, peaks.at(0)->amp);
    EXPECT_EQ(2, peaks.at(0)->location);
    EXPECT_NEAR(5.5, peaks.at(0)->fwhm,1);

    EXPECT_EQ(190, peaks.at(1)->amp);
    EXPECT_EQ(9, peaks.at(1)->location);
    EXPECT_NEAR(5, peaks.at(1)->fwhm,1);

    EXPECT_EQ(2, count);
}

//Collect a list of up to 10 problematic waveforms (first peak is < 10% of second peak). #259
//1
TEST_F(GaussianFitterTest, problem_waveform_1_guess){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 1 1 1 1 1 3 2 2 1 1 1 2 5 11 26 57 102 148 181 189 "
        "173 138 96 59 32 17 11 10 10 11 11 12 9 7 5 4 3 "
        "3 3 2 2 2 2 2 3 3 2 2 2 2 2 2 4 3 3 2 2 2 2";

    parseWave(input, idxData, ampData);
    
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(189, peaks.at(0)->amp);
    EXPECT_EQ(20, peaks.at(0)->location);
    EXPECT_NEAR(6.2, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(12, peaks.at(1)->amp);
    EXPECT_EQ(32, peaks.at(1)->location);
    EXPECT_NEAR(5, peaks.at(1)->fwhm, 1);
  
    EXPECT_EQ(2, count);
}

//2
TEST_F(GaussianFitterTest, problem_waveform_2_guess){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "3 3 2 2 2 2 2 2 2 2 1 1 1 2 6 16 42 83 132 176 198 197 "
        "170 124 77 41 20 12 10 10 11 12 13 12 9 6 6 6 5 "
        "5 4 3 2 2 2 2 3 3 3 3 2 2 2 2 2 2 3 5 4 4";

    parseWave(input, idxData, ampData);
    
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(198, peaks.at(0)->amp);
    EXPECT_EQ(20, peaks.at(0)->location);
    EXPECT_NEAR(6.2, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(13, peaks.at(1)->amp);
    EXPECT_EQ(32, peaks.at(1)->location);
    EXPECT_NEAR(5.4, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2, count);
     
}             
//3
TEST_F(GaussianFitterTest, problem_waveform_3_guess){
    
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 2 2 2 2 2 2 2 1 1 1 1 1 2 6 16 38 74 120 161 182 174 "
        "138 94 54 28 14 10 7 9 10 11 11 12 9 7 6 6 6 5 "
        "5 4 4 3 2 2 2 1 1 1 1 1 1 3 2 2 1 1 2 4";

    parseWave(input, idxData, ampData);
    
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(182, peaks.at(0)->amp);
    EXPECT_EQ(20, peaks.at(0)->location);
    EXPECT_NEAR(5.6, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(12, peaks.at(1)->amp);
    EXPECT_EQ(33, peaks.at(1)->location);
    EXPECT_NEAR(6, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2, count);
    
}
             
//4
TEST_F(GaussianFitterTest, problem_waveform_4_guess){
    
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 1 1 1 2 2 2 1 1 0 1 1 1 1 3 10 33 77 134 183 205 "
        "194 159 110 66 34 18 12 12 12 11 13 12 10 7 7 6 5 5 4 "
        "4 3 3 3 3 3 3 3 3 2 2 2 2 2 2 2 2 2 2 3";

    parseWave(input, idxData, ampData);
    
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(3,peaks.size());
    EXPECT_EQ(205, peaks.at(0)->amp);
    EXPECT_EQ(20, peaks.at(0)->location);
    EXPECT_NEAR(5.8, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(12, peaks.at(1)->amp);
    EXPECT_EQ(28, peaks.at(1)->location);
    EXPECT_NEAR(14, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(13, peaks.at(2)->amp);
    EXPECT_EQ(31, peaks.at(2)->location);
    EXPECT_NEAR(9, peaks.at(2)->fwhm, 1);
    EXPECT_EQ(3, count);
}
             
//5
TEST_F(GaussianFitterTest, problem_waveform_5_guess){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "4 3 3 2 2 2 2 1 1 1 1 2 6 9 16 32 65 111 158 186 186 "
        "158 115 71 39 20 11 10 11 13 14 12 11 8 7 6 5 5 "
        "4 3 3 3 3 3 2 2 2 2 2 2 2 2 2 2 2 1 1 1 1 3";

    parseWave(input, idxData, ampData);
    
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(186, peaks.at(0)->amp);
    EXPECT_NEAR(19.5, peaks.at(0)->location,.25);
    EXPECT_NEAR(5.7, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(14, peaks.at(1)->amp);
    EXPECT_EQ(30, peaks.at(1)->location);
    EXPECT_NEAR(8, peaks.at(1)->fwhm, 1.25);

    EXPECT_EQ(2, count);
}             
//6
TEST_F(GaussianFitterTest, problem_waveform_6_guess){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = " 3 2 1 1 1 1 1 1 1 1 2 2 4 10 26 61 110 157 181 "
        "172 136 90 49 23 9 5 7 9 11 12 12 10 8 6 5 4 3 3 3 3 3 "
        "3 3 2 2 2 2 2 2 2 2 1 1 2 2 3 5 10 19";

    parseWave(input, idxData, ampData);
    
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(181, peaks.at(0)->amp);
    EXPECT_EQ(18, peaks.at(0)->location);
    EXPECT_NEAR(5.5, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(12, peaks.at(1)->amp);
    EXPECT_NEAR(29.5, peaks.at(1)->location,.25);
    EXPECT_NEAR(7, peaks.at(1)->fwhm, 1.2);

    EXPECT_EQ(2, count);
}
             
//7
TEST_F(GaussianFitterTest, problem_waveform_7_guess){
    
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 0 0 0 2 0 1 1 1 1 1 1 1 2 4 10 25 56 98 141 174 "
        "189 186 164 126 83 47 25 16 13 11 11 11 10 10 8 7 6 6 "
        "6 5 5 4 4 3 3 2 2 3 3 3 5 3 3 2 2 2 2 2 2";
   
    parseWave(input, idxData, ampData);
    
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(189, peaks.at(0)->amp);
    EXPECT_EQ(21, peaks.at(0)->location);
    EXPECT_NEAR(7.9, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(11, peaks.at(1)->amp);
    EXPECT_EQ(31, peaks.at(1)->location);
    EXPECT_NEAR(17, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2, count);
}
   
//8
TEST_F(GaussianFitterTest, problem_waveform_8_guess){
    
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 0 0 1 3 2 2 2 2 2 1 1 2 5 11 30 67 117 163 191 "
        "95 174 137 92 53 27 16 12 11 11 11 11 11 10 6 6 4 3 2 2 2 "
        "2 2 3 3 2 2 2 2 3 3 3 3 3 2 2 2 2 2 3";

    parseWave(input, idxData, ampData);
    
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(3, peaks.size());
    EXPECT_NEAR(191, peaks.at(0)->amp, 1);
    EXPECT_EQ(19, peaks.at(0)->location);
    EXPECT_NEAR(3.5, peaks.at(0)->fwhm, 1);

    EXPECT_NEAR(174, peaks.at(1)->amp, 1);
    EXPECT_EQ(21, peaks.at(1)->location);
    EXPECT_NEAR(4.4, peaks.at(1)->fwhm, 1);

    EXPECT_NEAR(11, peaks.at(2)->amp, 1);
    EXPECT_EQ(30, peaks.at(2)->location);
    EXPECT_NEAR(10.4, peaks.at(2)->fwhm, 1);

    EXPECT_EQ(3, count);

}
             
//9
TEST_F(GaussianFitterTest, problem_waveform_9_guess){
    
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 1 1 1 1 3 1 1 1 1 2 2 4 8 22 53 97 141 172 183 172 147 113 77 46 26 15 12 13 13 12 11 9 8 7 6 5 5 4 4 3 3 2 2 1 1 0 0 0 0 1 3 2 2 2 2 2 2 2 3";

    parseWave(input, idxData, ampData); 
   
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(2,peaks.size());
    EXPECT_NEAR(183, peaks.at(0)->amp, 1);
    EXPECT_EQ(19, peaks.at(0)->location);
    EXPECT_NEAR(6.8, peaks.at(0)->fwhm, 1);

    EXPECT_NEAR(13, peaks.at(1)->amp, 1);
    EXPECT_NEAR(28.5, peaks.at(1)->location,.25);
    EXPECT_NEAR(12, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2, count);
}
             
//10
TEST_F(GaussianFitterTest, problem_waveform_10_guess){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "20 13 8 5 5 4 3 5 9 26 62 112 157 184 180 149 106 64 33 16 8 8 9 11 12 12 9 8 6 5 5 4 4 3 3 3 3 3 3 3 3 3 2 2 4 2 2 2 4 3 3 2 1 1 1 1 3 2 2 2";

    parseWave(input, idxData, ampData); 
    
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(2,peaks.size());
    EXPECT_NEAR(184, peaks.at(0)->amp, 1);
    EXPECT_EQ(13, peaks.at(0)->location);
    EXPECT_NEAR(5.6, peaks.at(0)->fwhm, 1);

    EXPECT_NEAR(12, peaks.at(1)->amp, 1);
    EXPECT_NEAR(24.5, peaks.at(1)->location,.25);
    EXPECT_NEAR(6, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2, count);

}

//testing guess.peaks() without flat areas
//convex, one peak waveform
TEST_F(GaussianFitterTest, FlatFreeTest1_guess){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "9 11 12 13 15 22 34 68 54 42 27 21 17 15 13 12";

    parseWave(input, idxData, ampData);

    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(1,peaks.size());
    EXPECT_NEAR(68, peaks.at(0)->amp, 1);
    EXPECT_EQ(7, peaks.at(0)->location);
    EXPECT_NEAR(3.3, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(1, count);
}

//concave, no peaks waveform
TEST_F(GaussianFitterTest, FlatFreeTest2_guess){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "87 73 56 41 18 15 26 41 57 57 57 78 81 89 97";

    parseWave(input, idxData, ampData);

    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(0,peaks.size());

    EXPECT_NEAR(57, peaks.at(0)->amp, 1);
    EXPECT_EQ(9, peaks.at(0)->location);
    EXPECT_NEAR(7, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(0, count);
    //EXPECT_NEAR(68, peaks.at(0)->amp, 1);
   // EXPECT_EQ(8, peaks.at(0)->location);
}

//slope up, no peaks waveform
TEST_F(GaussianFitterTest, FlatFreeTest3_guess){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "14 18 21 26 35 44 52 64 73 82 86 91 103";

    parseWave(input, idxData, ampData);

    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(0,peaks.size());
    EXPECT_EQ(0, count);
    //EXPECT_NEAR(68, peaks.at(0)->amp, 1);
    //EXPECT_EQ(8, peaks.at(0)->location);
}

//slope down, no peaks waveform
TEST_F(GaussianFitterTest, FlatFreeTest4_guess){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "123	109 93 82 71 64 51 42 33 20 12";

    parseWave(input, idxData, ampData);

    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);
    EXPECT_EQ(0,peaks.size());
   // EXPECT_NEAR(68, peaks.at(0)->amp, 1);
   // EXPECT_EQ(8, peaks.at(0)->location);
    EXPECT_EQ(0, count);
}

//two peaks waveform
TEST_F(GaussianFitterTest, FlatFreeTest5_guess){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "24 37 52 66 65 52 41 32 22 17 28 56 78 62 45 45 45 27";

    parseWave(input, idxData, ampData);

    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(3,peaks.size());
    EXPECT_NEAR(66, peaks.at(0)->amp, 1);
    EXPECT_EQ(3, peaks.at(0)->location);
    EXPECT_NEAR(5.2, peaks.at(0)->fwhm, 1);

    EXPECT_NEAR(78, peaks.at(1)->amp, 1);
    EXPECT_EQ(12, peaks.at(1)->location);
    EXPECT_NEAR(3.2, peaks.at(1)->fwhm, 1);

    EXPECT_NEAR(45, peaks.at(2)->amp, 1);
    EXPECT_EQ(15, peaks.at(2)->location);
    EXPECT_NEAR(7, peaks.at(2)->fwhm, 1);

    EXPECT_EQ(3, count);
}

//flat areas waveform
TEST_F(GaussianFitterTest, FlatFreeTest6_guess){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "13 17 17 17 23 32 32 32 32 67 67 67 73 73 89";

    parseWave(input, idxData, ampData);

    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(3,peaks.size());
    EXPECT_NEAR(17, peaks.at(0)->amp, 1);
    EXPECT_EQ(2, peaks.at(0)->location);
    EXPECT_NEAR(5.2, peaks.at(0)->fwhm, 1);

    EXPECT_NEAR(32, peaks.at(1)->amp, 1);
    EXPECT_EQ(6.5, peaks.at(1)->location);
    EXPECT_NEAR(3.2, peaks.at(1)->fwhm, 1);

    EXPECT_NEAR(45, peaks.at(2)->amp, 1);
    EXPECT_EQ(10, peaks.at(2)->location);
    EXPECT_NEAR(7, peaks.at(2)->fwhm, 1);

    EXPECT_EQ(3, count);
}

        //////////////////////////
        // TESTING find_peaks() //
        //////////////////////////
/*
TEST_F(GaussianFitterTest, NayaniClipped1_find){
    
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 2 1 1 0 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 "
        "179 160 139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11 "
        "8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    ASSERT_EQ(3,peaks.size());
    EXPECT_EQ(200,peaks.at(0)->amp);
    EXPECT_NEAR(18.5, peaks.at(0)->location, .25);
    EXPECT_NEAR(10.5, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(43 ,peaks.at(1)->amp);
    EXPECT_NEAR(30.5, peaks.at(1)->location,.25);
    EXPECT_NEAR(4, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(34 ,peaks.at(2)->amp);
    EXPECT_EQ(38, peaks.at(2)->location);
    EXPECT_NEAR(6, peaks.at(2)->fwhm, 1);

    EXPECT_EQ(3, count);
}

TEST_F(GaussianFitterTest, NayaniClipped2_find){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 3 2 2 3 2 2 1 1 1 2 7 22 58 114 174 216 235 235 221 195 "
        "155 110 67 39 24 18 16 15 15 15 14 11 10 9 8 7 6 5 5 4 3 3 4 5 4 4 3 "
        "3 1 2 1 2 3 3 4 4 5 4 2";

    parseWave(input, idxData, ampData);

    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    ASSERT_EQ(2,peaks.size());
    EXPECT_EQ(235,peaks.at(0)->amp);
    EXPECT_NEAR(17.5, peaks.at(0)->location,1);
    EXPECT_NEAR(7.8, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(29, peaks.at(1)->location);
    EXPECT_EQ(15,peaks.at(1)->amp);
    EXPECT_NEAR(14, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2, count);
}

TEST_F(GaussianFitterTest, gaussianFitter_find){

    std::vector<int> idxData;
    std::vector<int> ampData;
    char input[] = "2 2 2 1 1 1 1 1 1 0 0 1 9 35 88 155 212 240 237 200 145 87 42 18 12 13 14 15 15 14 13 10 8 8 8 8 7 6 6 4 4 4 3 4 5 6 4 4 3 2 2 1 1 0 1 2 3 4 4 2";

    parseWave(input, idxData, ampData);

    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(240,peaks.at(0)->amp);
    EXPECT_EQ(15, peaks.at(1)->amp);
    EXPECT_EQ(17, peaks.at(0)->location);
    EXPECT_NEAR(27.5, peaks.at(1)->location,.25);
    EXPECT_NEAR(6, peaks.at(0)->fwhm,1);
    EXPECT_NEAR(17 , peaks.at(1)->fwhm,2);    
  
    EXPECT_EQ(2, count);
}


TEST_F(GaussianFitterTest, NayaniClipped3_find){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 3 4 3 3 2 1 1 0 1 1 1 1 1 6 29 79 147 207 238 235 198 141 86 43 20 11 11 13 15 15 15 13 11 8 5 5 4 6 6 7 7 5 4 4 3 4 4 5 4 3 3 2 1 1 3 3 3 2 3";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(238,peaks.at(0)->amp);
    EXPECT_EQ(19, peaks.at(0)->location);
    EXPECT_NEAR(5.9, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(15, peaks.at(1)->amp);
    EXPECT_NEAR(30, peaks.at(1)->location,1);
    EXPECT_NEAR(9, peaks.at(1)->fwhm, 1.2);

    EXPECT_EQ(2, count);
}

TEST_F(GaussianFitterTest, NayaniClipped4_find){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 2 3 2 2 2 2 1 0 1 1 2 2 4 16 49 105 170 217 239 240 223 "
        "195 155 110 68 40 25 20 16 16 15 12 10 7 7 7 7 6 5 4 4 3 2 2 3 5 4 4 "
        "3 2 3 4 4 3 3 2 2 2 2";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    ASSERT_EQ(2,peaks.size());
    EXPECT_EQ(240,peaks.at(0)->amp);
    EXPECT_EQ(20, peaks.at(0)->location);
    EXPECT_NEAR(7.4, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(16,peaks.at(1)->amp);
    EXPECT_NEAR(29.5, peaks.at(1)->location,.25);
    EXPECT_NEAR(8, peaks.at(1)->fwhm, 1);
    EXPECT_EQ(2, count);
}

TEST_F(GaussianFitterTest, NayaniClipped5_find){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 1 1 1 0 1 3 2 3 4 5 7 16 38 72 109 136 150 150 140 118 "
        "87 56 32 23 30 53 82 111 133 145 146 135 121 106 96 87 71 52 35 23 19 "
        "16 14 10 8 8 7 6 6 4 4 4 5 5 6 5 4 3 1";

    parseWave(input, idxData, ampData);


    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    ASSERT_EQ(2,peaks.size());
    EXPECT_EQ(150,peaks.at(0)->amp);
    EXPECT_EQ(146,peaks.at(1)->amp);
    EXPECT_NEAR(17.5, peaks.at(0)->location,.25);
    EXPECT_EQ(31, peaks.at(1)->location);
    EXPECT_NEAR(7.3, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(10.2, peaks.at(1)->fwhm, 1);
    EXPECT_EQ(2,count);
}

TEST_F(GaussianFitterTest, NayaniClipped6_find){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 1 1 1 2 3 2 2 2 3 5 8 10 11 10 8 7 9 14 28 51 88 131 171 193 193 173 140 117 117 135 151 148 122 88 58 37 23 16 12 11 12 12 12 10 10 10 10 10 8 6 5 5 4 4 3 3 4 3 2";

    parseWave(input, idxData, ampData);
    
    //the noise level for this waveform is 17.4
    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    ASSERT_EQ(4,peaks.size());
    EXPECT_EQ(11,peaks.at(0)->amp);
    EXPECT_EQ(193,peaks.at(1)->amp);
    EXPECT_EQ(151,peaks.at(2)->amp);
    EXPECT_EQ(12, peaks.at(3)->amp);
    EXPECT_EQ(13, peaks.at(0)->location);
    EXPECT_NEAR(24.5, peaks.at(1)->location,.25);
    EXPECT_EQ(31, peaks.at(2)->location);
    EXPECT_EQ(42, peaks.at(3)->location);
    EXPECT_NEAR(5, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(5.6, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(7.6, peaks.at(1)->fwhm, 1.25);
    EXPECT_NEAR(6.6, peaks.at(2)->fwhm, 1);
    EXPECT_NEAR(16, peaks.at(3)->fwhm, 2);

    EXPECT_EQ(4, count);
}

TEST_F(GaussianFitterTest, NayaniClipped7_find){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 0 1 0 0 1 2 1 2 1 1 1 2 8 19 39 65 87 98 97 89 82 76 79 "
        "93 114 130 137 141 152 165 168 153 119 77 46 26 19 15 13 13 13 13 11 "
        "10 8 6 6 5 4 4 3 2 2 2 2 3 4 4 4";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    fitter.noise_level = 9;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    ASSERT_EQ(3,peaks.size());
    EXPECT_EQ(98,peaks.at(0)->amp);
    EXPECT_EQ(168,peaks.at(1)->amp);
    EXPECT_EQ(18, peaks.at(0)->location);
    EXPECT_EQ(31, peaks.at(1)->location);
    EXPECT_NEAR(5.2, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(10, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(13,peaks.at(2)->amp);
    EXPECT_NEAR(40.5, peaks.at(2)->location,.25);
    EXPECT_NEAR(11, peaks.at(2)->fwhm, 2);

    EXPECT_EQ(3, count);
} 
 
TEST_F(GaussianFitterTest, NayaniClipped8_find){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "4 3 2 2 1 1 1 1 1 3 3 6 11 17 23 26 26 25 27 42 76 124 170 "
        "200 211 213 215 219 221 209 178 133 90 60 42 30 23 20 21 20 19 16 13 "
        "11 9 6 5 4 3 3 3 3 4 6 6 6 4 3 1 2";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    ASSERT_EQ(3,peaks.size());
    EXPECT_EQ(26,peaks.at(0)->amp);
    EXPECT_EQ(221,peaks.at(1)->amp);
    EXPECT_EQ(21,peaks.at(2)->amp);
    EXPECT_NEAR(15.5,peaks.at(0)->location,0.25);
    EXPECT_EQ(28,peaks.at(1)->location);
    EXPECT_EQ(38,peaks.at(2)->location);
    EXPECT_NEAR(7,peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(10.8,peaks.at(1)->fwhm, 1);
    EXPECT_NEAR(10.2,peaks.at(2)->fwhm, 1);

    EXPECT_EQ(3, count);
}

TEST_F(GaussianFitterTest, max_iter_1_find){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 2 2 2 2 3 2 3 3 2 2 3 4 5 15 39 80 124 150 147 119 80 46 "
        "23 10 6 4 5 7 9 8 6 5 4 2 2 2 2 3 4 4 4 3 2 1 3 4 4 3 3 2 2 3 3 5 10 "
        "18 23 25 23";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    ASSERT_EQ(2,peaks.size());
    EXPECT_EQ(150,peaks.at(0)->amp);
    EXPECT_EQ(25,peaks.at(1)->amp);
    EXPECT_EQ(18, peaks.at(0)->location);
    EXPECT_EQ(58, peaks.at(1)->location);
    EXPECT_NEAR(5.3, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(5.4, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2, count);

}


TEST_F(GaussianFitterTest, max_iter_2_find){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "3 3 2 2 2 2 3 2 4 4 4 6 18 47 89 125 139 124 89 53 25 9 3 "
        "2 3 6 8 10 9 7 5 4 3 3 1 1 2 3 3 4 4 3 2 2 2 2 2 3 2 2 3 2 2 3 4 8 16 "
        "23 26 25";

    parseWave(input, idxData, ampData);


    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    ASSERT_EQ(2,peaks.size());
    EXPECT_EQ(139,peaks.at(0)->amp);
    EXPECT_EQ(26,peaks.at(1)->amp);
    EXPECT_EQ(16, peaks.at(0)->location);
    EXPECT_EQ(58, peaks.at(1)->location);
    EXPECT_NEAR(5, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(4.6, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2, count);
}


TEST_F(GaussianFitterTest, max_iter_3_find){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "3 2 2 1 1 2 3 4 4 4 4 3 5 12 35 77 127 161 164 136 93 53 "
        "25 11 6 4 6 8 10 11 10 8 7 5 4 3 2 2 3 3 3 5 5 4 4 2 2 2 1 2 2 3 4 8 "
        "15 24 32 33 27 19";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(3,peaks.size());
    EXPECT_EQ(164,peaks.at(0)->amp);
    EXPECT_EQ(11,peaks.at(1)->amp);
    EXPECT_EQ(33,peaks.at(2)->amp);
    EXPECT_EQ(18,peaks.at(0)->location);
    EXPECT_EQ(29,peaks.at(1)->location);
    EXPECT_EQ(57,peaks.at(2)->location);
    EXPECT_NEAR(5.1, peaks.at(0)->fwhm, 1); 
    EXPECT_NEAR(8, peaks.at(1)->fwhm, 1); 
    EXPECT_NEAR(5.8, peaks.at(2)->fwhm, 1); 

    EXPECT_EQ(3,count);
}

TEST_F(GaussianFitterTest, max_iter_4_find){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 3 2 2 1 1 1 1 1 2 2 6 8 17 29 48 68 81 88 87 86 86 84 73 "
        "57 40 30 28 29 31 32 34 33 32 28 25 22 19 16 13 12 10 8 8 8 8 9 9 11 "
        "15 19 20 18 14 9 6 6 4 4 5 3 4 4 3 4 5 4 4 5 4 3 2 2 1 1 3 3 3 3 3 4 "
        "5 5 5 3 3 2 2 1 1 1 2 2 1 1 1 3 2 3 2 2 2 2 3 2 2 2 1 2 4 4 4 4 2 1 1 "
        "2 3 4 4";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(3,peaks.size());
    EXPECT_EQ(88,peaks.at(0)->amp);
    EXPECT_EQ(34,peaks.at(1)->amp);
    EXPECT_EQ(20,peaks.at(2)->amp);
    EXPECT_EQ(18,peaks.at(0)->location);
    EXPECT_EQ(31,peaks.at(1)->location);
    EXPECT_EQ(51,peaks.at(2)->location);
    EXPECT_NEAR(9.9, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(13.4, peaks.at(1)->fwhm, 1);
    EXPECT_NEAR(6.3, peaks.at(2)->fwhm, 1);

    EXPECT_EQ(3,count);
}

TEST_F(GaussianFitterTest, max_iter_5_find){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 3 2 2 1 1 1 1 1 2 2 6 8 17 29 48 68 81 88 87 86 86 84 73 "
        "57 40 30 28 29 31 32 34 33 32 28 25 22 19 16 13 12 10 8 9 8 8 9 9 11 "
        "15 19 20 18 14 9 6 6 4 4 5 3 4 4 3 4 5 4 4 5 4 3 2 2 1 1 3 3 3 3 3 4 "
        "5 5 5 3 3 2 2 1 1 1 2 2 1 1 1 3 2 3 2 2 2 2 3 2 2 2 1 2 4 4 4 4 2 1 1 "
        "2 3 4 4 2 1 2 2 3 4 4 4 3 2 3 5 7 13 20 24 25 21 14 9 8 10 12 13 12 8 "
        "5 4 3 3 3 4 8 21 46 81 114 132 130 110 82 55 34 21 13 10 9 11 11 11 "
        "12 14 17 21 22 21 15 10 6 3 2 3 3 4 3 4 4 4 4 3 2 1 1 2 2 3 4 3 3 3 2 "
        "2 1 2 2 3 3 4 4 4 3 2 1 2 2 4 4 2 1 2 2 1 1 1 2 2 2 2 1 2 3 4 4 5 5 4 "
        "4 2 2 2";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    ASSERT_EQ(7,peaks.size());
    EXPECT_EQ(88,peaks.at(0)->amp);
    EXPECT_EQ(34,peaks.at(1)->amp);
    EXPECT_EQ(20,peaks.at(2)->amp);
    EXPECT_EQ(25,peaks.at(3)->amp);
    EXPECT_EQ(13,peaks.at(4)->amp);
    EXPECT_EQ(132,peaks.at(5)->amp);
    EXPECT_EQ(22,peaks.at(6)->amp);
    EXPECT_EQ(18,peaks.at(0)->location);
    EXPECT_EQ(31,peaks.at(1)->location);
    EXPECT_EQ(51,peaks.at(2)->location);
    EXPECT_EQ(136,peaks.at(3)->location);
    EXPECT_EQ(143,peaks.at(4)->location);
    EXPECT_EQ(157,peaks.at(5)->location);
    EXPECT_EQ(174,peaks.at(6)->location);
    EXPECT_NEAR(10.1, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(11.8, peaks.at(1)->fwhm, 1);
    EXPECT_NEAR(6.3, peaks.at(2)->fwhm, 1);
    EXPECT_NEAR(5.4, peaks.at(3)->fwhm, 1);
    EXPECT_NEAR(5.2, peaks.at(4)->fwhm, 1);
    EXPECT_NEAR(6, peaks.at(5)->fwhm, 1);
    EXPECT_NEAR(7.9, peaks.at(6)->fwhm, 1);

    EXPECT_EQ(7,count);
}

TEST_F(GaussianFitterTest, trig_loc_1_find){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 1 0 2 2 3 2 2 1 2 2 1 2 6 22 55 101 148 175 183 176 171 "
        "172 167 146 112 73 45 30 21 17 14 14 13 12 10 7 6 6 6 5 4 4 5 4 4 3 3 "
        "2 3 2 3 2 1 1 2 2 3 4 2";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(183,peaks.at(0)->amp);
    EXPECT_EQ(172,peaks.at(1)->amp);
    EXPECT_EQ(19,peaks.at(0)->location);
    EXPECT_EQ(22,peaks.at(1)->location);
    EXPECT_NEAR(9.8, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(10, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2,count);
}

TEST_F(GaussianFitterTest, trig_loc_2_find){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 0 0 1 1 1 1 2 2 2 3 4 5 13 28 56 91 124 143 141 125 112 "
        "114 127 135 127 102 74 49 31 19 14 10 10 10 10 8 6 5 4 4 4 4 4 4 4 3 "
        "3 3 4 3 3 2 2 1 0 0 1 2 1";

    parseWave(input, idxData, ampData);

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(143, peaks.at(0)->amp);
    EXPECT_EQ(135, peaks.at(1)->amp);
    EXPECT_EQ(18, peaks.at(0)->location);
    EXPECT_EQ(24, peaks.at(1)->location);
    EXPECT_NEAR(5.2, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(6.4, peaks.at(1)->fwhm, 1);
   
    EXPECT_EQ(2,count);
}

TEST_F(GaussianFitterTest, num_iterations_10_find){
    
    std::vector<int> idxData;
    std::vector<int> ampData;
    
    char input[] = "26 36 37 30 21 22 47 96 153 190 186 147 94 49 21 7 3 4 4 3 3 2 1 0 0 0 0 0";

    parseWave(input, idxData, ampData);

    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(2, peaks.size());
    EXPECT_EQ(37, peaks.at(0)->amp);
    EXPECT_EQ(190, peaks.at(1)->amp);
    EXPECT_EQ(2, peaks.at(0)->location);
    EXPECT_EQ(9, peaks.at(1)->location);
    EXPECT_NEAR(5.5, peaks.at(0)->fwhm,1);
    EXPECT_NEAR(5, peaks.at(1)->fwhm,1);

    EXPECT_EQ(2, count);
}

TEST_F(GaussianFitterTest, problem_waveform_1_find){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 1 1 1 1 1 3 2 2 1 1 1 2 5 11 26 57 102 148 181 189 "
        "173 138 96 59 32 17 11 10 10 11 11 12 9 7 5 4 3 "
        "3 3 2 2 2 2 2 3 3 2 2 2 2 2 2 4 3 3 2 2 2 2";

    parseWave(input, idxData, ampData);
    
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(189, peaks.at(0)->amp);
    EXPECT_EQ(12, peaks.at(1)->amp);
    EXPECT_EQ(20, peaks.at(0)->location);
    EXPECT_EQ(32, peaks.at(1)->location);
    EXPECT_NEAR(6.2, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(5, peaks.at(1)->fwhm, 1);
  
    EXPECT_EQ(2, count);
}

//2
TEST_F(GaussianFitterTest, problem_waveform_2_find){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "3 3 2 2 2 2 2 2 2 2 1 1 1 2 6 16 42 83 132 176 198 197 "
        "170 124 77 41 20 12 10 10 11 12 13 12 9 6 6 6 5 "
        "5 4 3 2 2 2 2 3 3 3 3 2 2 2 2 2 2 3 5 4 4";

    parseWave(input, idxData, ampData);
    
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(198, peaks.at(0)->amp);
    EXPECT_EQ(13, peaks.at(1)->amp);
    EXPECT_EQ(20, peaks.at(0)->location);
    EXPECT_EQ(32, peaks.at(1)->location);
    EXPECT_NEAR(6.2, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(5.4, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2, count);
     
}
             
TEST_F(GaussianFitterTest, problem_waveform_3_find){
    
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 2 2 2 2 2 2 2 1 1 1 1 1 2 6 16 38 74 120 161 182 174 "
        "138 94 54 28 14 10 7 9 10 11 11 12 9 7 6 6 6 5 "
        "5 4 4 3 2 2 2 1 1 1 1 1 1 3 2 2 1 1 2 4";

    parseWave(input, idxData, ampData);
    
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(182, peaks.at(0)->amp);
    EXPECT_EQ(12, peaks.at(1)->amp);
    EXPECT_EQ(20, peaks.at(0)->location);
    EXPECT_EQ(33, peaks.at(1)->location);
    EXPECT_NEAR(5.6, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(6, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2, count);
    
}
             
//4
TEST_F(GaussianFitterTest, problem_waveform_4_find){
    
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 1 1 1 2 2 2 1 1 0 1 1 1 1 3 10 33 77 134 183 205 "
        "194 159 110 66 34 18 12 12 12 11 13 12 10 7 7 6 5 5 4 "
        "4 3 3 3 3 3 3 3 3 2 2 2 2 2 2 2 2 2 2 3";

    parseWave(input, idxData, ampData);
    
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(205, peaks.at(0)->amp);
    EXPECT_EQ(13, peaks.at(1)->amp);
    EXPECT_EQ(20, peaks.at(0)->location);
    EXPECT_EQ(31, peaks.at(1)->location);
    EXPECT_NEAR(5.8, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(9, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2, count);
}

TEST_F(GaussianFitterTest, problem_waveform_5_find){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "4 3 3 2 2 2 2 1 1 1 1 2 6 9 16 32 65 111 158 186 186 "
        "158 115 71 39 20 11 10 11 13 14 12 11 8 7 6 5 5 "
        "4 3 3 3 3 3 2 2 2 2 2 2 2 2 2 2 2 1 1 1 1 3";

    parseWave(input, idxData, ampData);
    
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    ASSERT_EQ(2,peaks.size());
    EXPECT_NEAR(19.5, peaks.at(0)->location,.25);
    EXPECT_EQ(186, peaks.at(0)->amp);
    EXPECT_EQ(14, peaks.at(1)->amp);
    EXPECT_EQ(30, peaks.at(1)->location);
    EXPECT_NEAR(5.7, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(8, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2, count);
}
             
//6
TEST_F(GaussianFitterTest, problem_waveform_6_find){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = " 3 2 1 1 1 1 1 1 1 1 2 2 4 10 26 61 110 157 181 "
        "172 136 90 49 23 9 5 7 9 11 12 12 10 8 6 5 4 3 3 3 3 3 "
        "3 3 2 2 2 2 2 2 2 2 1 1 2 2 3 5 10 19";

    parseWave(input, idxData, ampData);
    
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(181, peaks.at(0)->amp);
    EXPECT_EQ(12, peaks.at(1)->amp);
    EXPECT_EQ(18, peaks.at(0)->location);
    EXPECT_NEAR(29.5, peaks.at(1)->location,.25);
    EXPECT_NEAR(5.5, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(6, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2, count);
}

TEST_F(GaussianFitterTest, problem_waveform_7_find){
    
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 0 0 0 2 0 1 1 1 1 1 1 1 2 4 10 25 56 98 141 174 "
        "189 186 164 126 83 47 25 16 13 11 11 11 10 10 8 7 6 6 "
        "6 5 5 4 4 3 3 2 2 3 3 3 5 3 3 2 2 2 2 2 2";
   
    parseWave(input, idxData, ampData);
    
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(1,peaks.size());
    EXPECT_EQ(189, peaks.at(0)->amp);
    EXPECT_EQ(21, peaks.at(0)->location);
    EXPECT_NEAR(7.9, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(1, count);
}

TEST_F(GaussianFitterTest, problem_waveform_8_find){
    
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 0 0 1 3 2 2 2 2 2 1 1 2 5 11 30 67 117 163 191 "
        "95 174 137 92 53 27 16 12 11 11 11 11 11 10 6 6 4 3 2 2 2 "
        "2 2 3 3 2 2 2 2 3 3 3 3 3 2 2 2 2 2 3";

    parseWave(input, idxData, ampData);
    
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(2,peaks.size());
    EXPECT_NEAR(191, peaks.at(0)->amp, 1);
    EXPECT_NEAR(174, peaks.at(1)->amp, 1);
    EXPECT_EQ(19, peaks.at(0)->location);
    EXPECT_EQ(21, peaks.at(1)->location);
    EXPECT_NEAR(4.8, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(4.4, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2, count);

}
             
//9
TEST_F(GaussianFitterTest, problem_waveform_9_find){
    
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 1 1 1 1 3 1 1 1 1 2 2 4 8 22 53 97 141 172 183 172 147 113 77 46 26 15 12 13 13 12 11 9 8 7 6 5 5 4 4 3 3 2 2 1 1 0 0 0 0 1 3 2 2 2 2 2 2 2 3";

    parseWave(input, idxData, ampData); 
   
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(2,peaks.size());
    EXPECT_NEAR(183, peaks.at(0)->amp, 1);
    EXPECT_NEAR(13, peaks.at(1)->amp, 1);
    EXPECT_EQ(19, peaks.at(0)->location);
    EXPECT_NEAR(28.5, peaks.at(1)->location,.25);
    EXPECT_NEAR(6.8, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(9, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2, count);
}

TEST_F(GaussianFitterTest, problem_waveform_10_find){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "20 13 8 5 5 4 3 5 9 26 62 112 157 184 180 149 106 64 33 16 8 8 9 11 12 12 9 8 6 5 5 4 4 3 3 3 3 3 3 3 3 3 2 2 4 2 2 2 4 3 3 2 1 1 1 1 3 2 2 2";

    parseWave(input, idxData, ampData); 
    
    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(2,peaks.size());
    EXPECT_NEAR(184, peaks.at(0)->amp, 1);
    EXPECT_NEAR(12, peaks.at(1)->amp, 1);
    EXPECT_EQ(13, peaks.at(0)->location);
    EXPECT_NEAR(24.5, peaks.at(1)->location,.25);
    EXPECT_NEAR(5.6, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(6, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2, count);

}

//testing guess.peaks() without flat areas
//convex, one peak waveform
TEST_F(GaussianFitterTest, FlatFreeTest1_find){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "9 11 12 13 15 22 34 68 54 42 27 21 17 15 13 12";

    parseWave(input, idxData, ampData);

    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(1,peaks.size());
    EXPECT_NEAR(68, peaks.at(0)->amp, 1);
    EXPECT_EQ(7, peaks.at(0)->location);
    EXPECT_NEAR(3.3, peaks.at(0)->fwhm, 1);

    EXPECT_EQ(1, count);
}

//concave, no peaks waveform
TEST_F(GaussianFitterTest, FlatFreeTest2_find){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "87 73 56 41 18 15 26 41 57 57 57 78 81 89 97";

    parseWave(input, idxData, ampData);

    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    ASSERT_EQ(0,peaks.size());
    EXPECT_EQ(0,count);

    //EXPECT_NEAR(68, peaks.at(0)->amp, 1);
   // EXPECT_EQ(8, peaks.at(0)->location);
}

//slope up, no peaks waveform
TEST_F(GaussianFitterTest, FlatFreeTest3_find){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "14 18 21 26 35 44 52 64 73 82 86 91 103";

    parseWave(input, idxData, ampData);

    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);
    EXPECT_EQ(0,peaks.size());
    EXPECT_EQ(0,count);
    //EXPECT_NEAR(68, peaks.at(0)->amp, 1);
    //EXPECT_EQ(8, peaks.at(0)->location);
}

//slope down, no peaks waveform
TEST_F(GaussianFitterTest, FlatFreeTest4_find){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "123	109 93 82 71 64 51 42 33 20 12";

    parseWave(input, idxData, ampData);

    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);
    EXPECT_EQ(0,peaks.size());
    EXPECT_EQ(0,count);
   // EXPECT_NEAR(68, peaks.at(0)->amp, 1);
   // EXPECT_EQ(8, peaks.at(0)->location);
}

//two peaks waveform
TEST_F(GaussianFitterTest, FlatFreeTest5_find){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "24 37 52 66 65 52 41 32 22 17 28 56 78 62 45 45 45 27";

    parseWave(input, idxData, ampData);

    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(2,peaks.size());
    EXPECT_NEAR(66, peaks.at(0)->amp, 1);
    EXPECT_NEAR(78, peaks.at(1)->amp, 1);
    EXPECT_EQ(3, peaks.at(0)->location);
    EXPECT_EQ(12, peaks.at(1)->location);
    EXPECT_NEAR(5.2, peaks.at(0)->fwhm, 1);
    EXPECT_NEAR(3.2, peaks.at(1)->fwhm, 1);

    EXPECT_EQ(2, count);

}

//flat areas waveform
TEST_F(GaussianFitterTest, FlatFreeTest6_find){

    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "13 17 17 17 23 32 32 32 32 67 67 67 73 73 89";

    parseWave(input, idxData, ampData);

    GaussianFitter fitter;
    fitter.noise_level = 9;
    std::vector<Peak*> peaks;
    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData, 200);

    EXPECT_EQ(0,peaks.size());
    EXPECT_EQ(0,count);
    //EXPECT_NEAR(67, peaks.at(0)->amp, 1);
   // EXPECT_EQ(12, peaks.at(0)->location);
}
*/
        //////////////////////////
