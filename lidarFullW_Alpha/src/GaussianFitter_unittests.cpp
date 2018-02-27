//File name: GaussianFitter_unittests.cpp
//Created on: 13-October-2017
//Author: ravi

#include "GaussianFitter.hpp"
#include "PulseData.hpp"
#include "pulsereader.hpp"
#include "pulsewriter.hpp"
#include "gtest/gtest.h"
#include "Peak.hpp"

class GaussianFitterTest: public testing::Test{
  public:
    std::vector<PulseData*> pulses;
    PulseData* pd;

  protected:

  //Function to set up space used by all tests
  virtual void SetUp(){
  }
};

/******************************************************************************
*
* Test gaussianFitter() method on returning wave data at pulse index 1
* "2 2 2 1 1 1 1 1 1 0 0 1 9 35 88 155 212 240 237 200 145 87 42 18 12 13 14
*  15 15 14 13 10 8 8 8 8 7 6 6 4 4 4 3 4 5 6 4 4 3 2 2 1 1 0 1 2 3 4 4 2"
*
******************************************************************************/
TEST_F(GaussianFitterTest, NayaniClipped2){

  // create a vector of integers
  std::vector<int> idxData;
  std::vector<int> ampData;
  
  char input[] = "2 2 1 1 0 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

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
  std::vector<int> guesses = fitter.guessPeaks(ampData);

  EXPECT_EQ(3,guesses.size());
  EXPECT_EQ(19,guesses[0]);
  EXPECT_EQ(32 ,guesses[1]);
  EXPECT_TRUE(39 == guesses[2]);

}

TEST_F(GaussianFitterTest, NayaniClipped){

  // create a vector of integers
  std::vector<int> idxData;
  std::vector<int> ampData;

  char input[] = "2 3 2 2 3 2 2 1 1 1 2 7 22 58 114 174 216 235 235 221 195 155 110 67 39 24 18 16 15 15 15 14 11 10 9 8 7 6 5 5 4 3 3 4 5 4 4 3 3 1 2 1 2 3 3 4 4 5 4 2";

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
  std::vector<Peak> peaks;
  int count = fitter.findPeaks(&peaks,ampData,idxData);
  EXPECT_TRUE(count == 3);
}

TEST_F(GaussianFitterTest, gaussianFitter){

  // create a vector of integers
  std::vector<int> idxData;
  std::vector<int> ampData;
  char input[] = "2 2 2 1 1 1 1 1 1 0 0 1 9 35 88 155 212 240 237 200 145 87 42 18 12 13 14 15 15 14 13 10 8 8 8 8 7 6 6 4 4 4 3 4 5 6 4 4 3 2 2 1 1 0 1 2 3 4 4 2";

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

  //TODO: check the results

}
