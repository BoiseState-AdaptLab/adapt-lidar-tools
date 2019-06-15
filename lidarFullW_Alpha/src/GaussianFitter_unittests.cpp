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

class GaussianFitterTest: public testing::Test{
    public:
        std::vector<PulseData*> pulses;
        PulseData* pd;

    protected:

        //Function to set up space used by all tests
        virtual void SetUp(){
        }
};

/****************************************************************************
 *
 * Test gaussianFitter() method on Nayani_clipped_test_1.pls
 *
 ****************************************************************************/
TEST_F(GaussianFitterTest, NayaniClipped1){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 2 1 1 0 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 "
        "179 160 139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11 "
        "8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

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

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(200,peaks.at(0)->amp);
    EXPECT_EQ(34 ,peaks.at(1)->amp);

}

TEST_F(GaussianFitterTest, NayaniClipped2){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 3 2 2 3 2 2 1 1 1 2 7 22 58 114 174 216 235 235 221 195 "
        "155 110 67 39 24 18 16 15 15 15 14 11 10 9 8 7 6 5 5 4 3 3 4 5 4 4 3 "
        "3 1 2 1 2 3 3 4 4 5 4 2";

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

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    fitter.guess_peaks(&peaks, ampData, idxData);

    EXPECT_EQ(1,peaks.size());
    EXPECT_EQ(235,peaks.at(0)->amp);
}

TEST_F(GaussianFitterTest, gaussianFitter){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;
    char input[] = "2 2 2 1 1 1 1 1 1 0 0 1 9 35 88 155 212 240 237 200 145 87 "
        "42 18 12 13 14 15 15 14 13 10 8 8 8 8 7 6 6 4 4 4 3 4 5 6 4 4 3 2 2 1 "
        "1 0 1 2 3 4 4 2";

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

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    fitter.guess_peaks(&peaks, ampData, idxData);
    EXPECT_EQ(1,peaks.size());
    EXPECT_EQ(240,peaks.at(0)->amp);
}

TEST_F(GaussianFitterTest, NayaniClipped3){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 3 4 3 3 2 1 1 0 1 1 1 1 1 6 29 79 147 207 238 235 198 "
        "141 86 43 20 11 11 13 15 15 15 13 11 8 5 5 4 6 6 7 7 5 4 4 3 4 4 5 4 "
        "3 3 2 1 1 3 3 3 2 3";

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

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    fitter.guess_peaks(&peaks, ampData, idxData);
    EXPECT_EQ(1,peaks.size());
    EXPECT_EQ(238,peaks.at(0)->amp);

    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData,10);

    EXPECT_EQ(1,count);
    //std::cerr << "--After guess_peaks and find_peaks--\n " << std::endl;
    //for(int i=0;i<count;i++){
    //  std::cerr<<peaks[i].amp<<std::endl;
    //}

}


TEST_F(GaussianFitterTest, NayaniClipped4){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 2 3 2 2 2 2 1 0 1 1 2 2 4 16 49 105 170 217 239 240 223 "
        "195 155 110 68 40 25 20 16 16 15 12 10 7 7 7 7 6 5 4 4 3 2 2 3 5 4 4 "
        "3 2 3 4 4 3 3 2 2 2 2";

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

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    fitter.guess_peaks(&peaks, ampData, idxData);
    EXPECT_EQ(1,peaks.size());
    EXPECT_EQ(240,peaks.at(0)->amp);

    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData,10);

    EXPECT_EQ(1,count);
    //std::cerr << "--After guess_peaks and find_peaks--\n " << std::endl;
    //for(int i=0;i<count;i++){
    //  std::cerr<<peaks[i].amp<<std::endl;
    //}

}

TEST_F(GaussianFitterTest, NayaniClipped5){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 1 1 1 0 1 3 2 3 4 5 7 16 38 72 109 136 150 150 140 118 "
        "87 56 32 23 30 53 82 111 133 145 146 135 121 106 96 87 71 52 35 23 19 "
        "16 14 10 8 8 7 6 6 4 4 4 5 5 6 5 4 3 1";

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

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    fitter.guess_peaks(&peaks, ampData, idxData);
    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(150,peaks.at(0)->amp);
    EXPECT_EQ(146,peaks.at(1)->amp);

    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData,10);

    EXPECT_EQ(2,count);
    //std::cerr << "--After guess_peaks and find_peaks--\n " << std::endl;
    //for(int i=0;i<count;i++){
    //  std::cerr<<peaks[i].amp<<std::endl;
    //}

}

TEST_F(GaussianFitterTest, NayaniClipped6){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 1 1 1 2 3 2 2 2 3 5 8 10 11 10 8 7 9 14 28 51 88 131 171 "
        "193 193 173 140 117 117 135 151 148 122 88 58 37 23 16 12 11 12 12 12 "
        "10 10 10 10 10 8 6 5 5 4 4 3 3 4 3 2";

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

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    fitter.guess_peaks(&peaks, ampData, idxData);
    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(193,peaks.at(0)->amp);
    EXPECT_EQ(151,peaks.at(1)->amp);

    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData,10);

    EXPECT_EQ(2,count);
    //std::cerr << "--After guess_peaks and find_peaks--\n " << std::endl;
    //for(int i=0;i<count;i++){
    //  std::cerr<<peaks[i].amp<<std::endl;
    //}

}

TEST_F(GaussianFitterTest, NayaniClipped7){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 0 1 0 0 1 2 1 2 1 1 1 2 8 19 39 65 87 98 97 89 82 76 79 "
        "93 114 130 137 141 152 165 168 153 119 77 46 26 19 15 13 13 13 13 11 "
        "10 8 6 6 5 4 4 3 2 2 2 2 3 4 4 4";

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

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    fitter.guess_peaks(&peaks, ampData, idxData);
    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(98,peaks.at(0)->amp);
    EXPECT_EQ(168,peaks.at(1)->amp);

    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData,10);

    EXPECT_EQ(2,count);
    //std::cerr << "--After guess_peaks and find_peaks--\n " << std::endl;
    //for(int i=0;i<count;i++){
    //  std::cerr<<peaks[i].amp<<std::endl;
    //}

}


TEST_F(GaussianFitterTest, NayaniClipped8){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "4 3 2 2 1 1 1 1 1 3 3 6 11 17 23 26 26 25 27 42 76 124 170 "
        "200 211 213 215 219 221 209 178 133 90 60 42 30 23 20 21 20 19 16 13 "
        "11 9 6 5 4 3 3 3 3 4 6 6 6 4 3 1 2";

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

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    fitter.guess_peaks(&peaks, ampData, idxData);
    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(26,peaks.at(0)->amp);
    EXPECT_EQ(221,peaks.at(1)->amp);

    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData,10);

    EXPECT_EQ(2,count);
    //std::cerr << "--After guess_peaks and find_peaks--\n " << std::endl;
    //for(int i=0;i<count;i++){
    //  std::cerr<<peaks[i].amp<<std::endl;
    //}
}


/****************************************************************************
 *
 * Test gaussianFitter() method on 140823_152425_1.pls
 *
 ****************************************************************************/

//Exceeding max no of iterations
TEST_F(GaussianFitterTest, max_iter_1){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 2 2 2 2 3 2 3 3 2 2 3 4 5 15 39 80 124 150 147 119 80 46 "
        "23 10 6 4 5 7 9 8 6 5 4 2 2 2 2 3 4 4 4 3 2 1 3 4 4 3 3 2 2 3 3 5 10 "
        "18 23 25 23";

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

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    fitter.guess_peaks(&peaks, ampData, idxData);
    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(150,peaks.at(0)->amp);
    EXPECT_EQ(25,peaks.at(1)->amp);


    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData,10);

    EXPECT_EQ(2,count);
    //std::cerr << "--After guess_peaks and find_peaks--\n " << std::endl;
    //for(int i=0;i<count;i++){
    //  std::cerr<<peaks[i].amp<<std::endl;
    //}
}

//Exceeding max no of iterations
TEST_F(GaussianFitterTest, max_iter_2){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "3 3 2 2 2 2 3 2 4 4 4 6 18 47 89 125 139 124 89 53 25 9 3 "
        "2 3 6 8 10 9 7 5 4 3 3 1 1 2 3 3 4 4 3 2 2 2 2 2 3 2 2 3 2 2 3 4 8 16 "
        "23 26 25";

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

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    fitter.guess_peaks(&peaks, ampData, idxData);
    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(139,peaks.at(0)->amp);
    EXPECT_EQ(26,peaks.at(1)->amp);


    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData,10);

    EXPECT_EQ(2,count);
    //std::cerr << "--After guess_peaks and find_peaks--\n " << std::endl;
    //for(int i=0;i<count;i++){
    //  std::cerr<<peaks[i].amp<<std::endl;
    //}
}

//Exceeding max no of iterations
TEST_F(GaussianFitterTest, max_iter_3){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "3 2 2 1 1 2 3 4 4 4 4 3 5 12 35 77 127 161 164 136 93 53 "
        "25 11 6 4 6 8 10 11 10 8 7 5 4 3 2 2 3 3 3 5 5 4 4 2 2 2 1 2 2 3 4 8 "
        "15 24 32 33 27 19";

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

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    fitter.guess_peaks(&peaks, ampData, idxData);
    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(164,peaks.at(0)->amp);
    EXPECT_EQ(33,peaks.at(1)->amp);


    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData,10);

    EXPECT_EQ(2,count);
    //std::cerr << "--After guess_peaks and find_peaks--\n " << std::endl;
    //for(int i=0;i<count;i++){
    //  std::cerr<<peaks[i].amp<<std::endl;
    //}
}

//Exceeding max no of iterations
TEST_F(GaussianFitterTest, max_iter_4){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "2 3 2 2 1 1 1 1 1 2 2 6 8 17 29 48 68 81 88 87 86 86 84 73 "
        "57 40 30 28 29 31 32 34 33 32 28 25 22 19 16 13 12 10 8 8 8 8 9 9 11 "
        "15 19 20 18 14 9 6 6 4 4 5 3 4 4 3 4 5 4 4 5 4 3 2 2 1 1 3 3 3 3 3 4 "
        "5 5 5 3 3 2 2 1 1 1 2 2 1 1 1 3 2 3 2 2 2 2 3 2 2 2 1 2 4 4 4 4 2 1 1 "
        "2 3 4 4";

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

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    fitter.guess_peaks(&peaks, ampData, idxData);
    EXPECT_EQ(4,peaks.size());
    EXPECT_EQ(88,peaks.at(0)->amp);
    EXPECT_EQ(34,peaks.at(1)->amp);
    EXPECT_EQ(9,peaks.at(2)->amp);
    EXPECT_EQ(20,peaks.at(3)->amp);


    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData,10);

    EXPECT_EQ(4,count);
    //std::cerr << "--After guess_peaks and find_peaks--\n " << std::endl;
    //for(int i=0;i<count;i++){
    //  std::cerr<<peaks[i].amp<<",";
    //  std::cerr<<peaks[i].location<<std::endl;
    //}
}

//Exceeding max no of iterations
TEST_F(GaussianFitterTest, max_iter_5){

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

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    fitter.guess_peaks(&peaks, ampData, idxData);
    ASSERT_EQ(7,peaks.size());
    EXPECT_EQ(88,peaks.at(0)->amp);
    EXPECT_EQ(34,peaks.at(1)->amp);
    EXPECT_EQ(20,peaks.at(2)->amp);
    EXPECT_EQ(25,peaks.at(3)->amp);
    EXPECT_EQ(13,peaks.at(4)->amp);
    EXPECT_EQ(132,peaks.at(5)->amp);
    EXPECT_EQ(22,peaks.at(6)->amp);

    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData,10);

    EXPECT_EQ(7,count);
    //std::cerr << "--After guess_peaks and find_peaks--\n " << std::endl;
    //for(int i=0;i<count;i++){
    //  std::cerr<<peaks[i].amp<<std::endl;
    //}
}

//Triggering location: -2147483648 not in range: 60
//Using file 140823_152425_1.pls
TEST_F(GaussianFitterTest, trig_loc_1){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 1 0 2 2 3 2 2 1 2 2 1 2 6 22 55 101 148 175 183 176 171 "
        "172 167 146 112 73 45 30 21 17 14 14 13 12 10 7 6 6 6 5 4 4 5 4 4 3 3 "
        "2 3 2 3 2 1 1 2 2 3 4 2";

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

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    fitter.guess_peaks(&peaks, ampData, idxData);
    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(183,peaks.at(0)->amp);
    EXPECT_EQ(172,peaks.at(1)->amp);

    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData,10);

    EXPECT_EQ(2,count);
    //std::cerr << "--After guess_peaks before find Peaks--\n " << std::endl;
    //for(int i=0;i<count;i++){
    //  std::cerr<<peaks[i].amp<<std::endl;
    //}
}

//Triggering location: -2147483648 not in range: 60
//Using file 140823_152425_1.pls
TEST_F(GaussianFitterTest, trig_loc_2){

    // create a vector of integers
    std::vector<int> idxData;
    std::vector<int> ampData;

    char input[] = "1 0 0 1 1 1 1 2 2 2 3 4 5 13 28 56 91 124 143 141 125 112 "
        "114 127 135 127 102 74 49 31 19 14 10 10 10 10 8 6 5 4 4 4 4 4 4 4 3 "
        "3 3 4 3 3 2 2 1 0 0 1 2 1";

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

    // now that we have the input vectors call the gaussianFitter
    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    fitter.guess_peaks(&peaks, ampData, idxData);
    EXPECT_EQ(2,peaks.size());
    EXPECT_EQ(143, peaks.at(0)->amp);
    EXPECT_EQ(135, peaks.at(1)->amp);

    fitter.smoothing_expt(&ampData);
    int count = fitter.find_peaks(&peaks,ampData,idxData,10);

    EXPECT_EQ(2,count);
    //std::cerr << "--After guess_peaks before find Peaks--\n " << std::endl;
    //for(int i=0;i<count;i++){
    //  std::cerr<<peaks[i].amp<<std::endl;
    //}
}

//Test gaussian fitting iterations
//Using file 140823_152425_1.pls
TEST_F(GaussianFitterTest, num_iterations_10){
    
    std::vector<int> idxData;
    std::vector<int> ampData;
    
    char input[] = "1 0 0 1 1 1 1 2 2 2 3 4 5 13 28 56 91 124 143 141 125 112 "
        "114 127 135 127 102 74 49 31 19 14 10 10 10 10 8 6 5 4 4 4 4 4 4 4 3 "
         "3 3 4 3 3 2 2 1 0 0 1 2 1";
    
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

    GaussianFitter fitter;
    std::vector<Peak*> peaks;
    std::vector<size_t> num_iters = {25,50,100,250,500};

    for (auto it = num_iters.begin(); it != num_iters.end(); ++it){
        fitter.guess_peaks(&peaks, ampData, idxData);
        EXPECT_EQ(2,peaks.size());
        EXPECT_EQ(143, peaks.at(0)->amp);
        EXPECT_EQ(135, peaks.at(1)->amp);

        fitter.smoothing_expt(&ampData);
        int count = fitter.find_peaks(&peaks,ampData,idxData,*it);
        std::cout << "Number of iterations: " << *it << std::endl;
        std::cout << "Equation: " << fitter.get_equation(0) << std::endl;

        EXPECT_EQ(2,count);
    }
}

TEST_F(GaussianFitterTest, fitting){
    char normal[] = "1 5 10 25 50 100 150 100 50 25 10 5 1";
    char skew_l[] = "1 15 45 100 150 125 100 80 76 67 54 42 34 20 19 14 12 10 6 5 4 2 1";
    char skew_r[] = "1 2 4 5 6 10 12 14 19 20 34 42 54 67 76 80 100 125 150 100 45 15 1";
    
    for (int j = 0; j < 3; j ++){
    std::ofstream myFile;
    myFile.open(std::string("output") + std::to_string(j) + std::string(".csv"));
    std::vector<std::vector<double>> data(2);

    std::vector<int> idxData;
    std::vector<int> ampData;

    //fails at 10 iterations
    //char input[] = "1 1 1 1 1 4 12 39 90 149 188 187 150 97 51 22 8 4 5 4 4 3 2 1 0 0 0 0";
    //peasses at 10 iterations
    //char input[] = "35 36 30 20 11 6 3 7 23 62 119 172 194 176 128 77 38 14 6 4 5 4 4 3 2 1 1 2";

    char* ptr;
    ptr = strtok (j == 0 ? normal : j == 1 ? skew_l : skew_r," ");
    int i=0;
    while (ptr != NULL){
        int y0 = atoi(ptr);
        ampData.push_back(y0);
        idxData.push_back(i);
        data.at(0).push_back(i);
        data.at(1).push_back(y0);
        i++;
        ptr = strtok (NULL," ");
    }

    GaussianFitter fitter;
    std::vector<Peak*> peaks;

    for (size_t i = 0; fitter.pass == 0; i++){
        fitter.smoothing_expt(&ampData);
        int count = fitter.find_peaks(&peaks,ampData,idxData,i);
        std::cout << "Number of iterations: " << i << std::endl;
        std::cout << "Equation: " << fitter.get_equation(0) << std::endl;
        std::vector<double> temp;
        for (auto it = data.at(0).begin(); it != data.at(0).end(); ++it){
            double val = peaks.at(0)->amp * exp(-0.5 * pow((*it - peaks.at(0)->location) / (peaks.at(0)->fwhm / 2), 2));
            temp.push_back(val);
        }
        data.push_back(temp);
    }
    myFile << "T," << "A,";
    for (int i = 0; i < data.size() - 2; i ++){
        myFile << i << " Iters" << (i != data.size() - 3 ? "," : "");
    }
    myFile << std::endl;
    for (auto it = data.at(0).begin(); it != data.at(0).end(); ++it){
        for (int i = 0; i < data.size(); i ++){
            myFile << data.at(i).at(*it) << (i != data.size() - 1 ? "," : "");
        }
        myFile << std::endl;
    }
    myFile.close();
    }
}
