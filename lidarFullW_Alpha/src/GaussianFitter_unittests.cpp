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
  EXPECT_EQ(31 ,guesses[1]);
  EXPECT_TRUE(38 == guesses[2]);

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
  std::vector<int> guesses = fitter.guessPeaks(ampData);
  EXPECT_EQ(2,guesses.size());
  EXPECT_EQ(18,guesses[0]);
  EXPECT_EQ(29 ,guesses[1]);
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
  std::vector<Peak> peaks;
  std::vector<int> guesses = fitter.guessPeaks(ampData);
  EXPECT_EQ(3,guesses.size());

}

TEST_F(GaussianFitterTest, NayaniClipped3){

  // create a vector of integers
  std::vector<int> idxData;
  std::vector<int> ampData;

  char input[] = "1 3 4 3 3 2 1 1 0 1 1 1 1 1 6 29 79 147 207 238 235 198 141 86 43 20 11 11 13 15 15 15 13 11 8 5 5 4 6 6 7 7 5 4 4 3 4 4 5 4 3 3 2 1 1 3 3 3 2 3";

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
  std::vector<int> guesses = fitter.guessPeaks(ampData);
  EXPECT_EQ(3,guesses.size());
  EXPECT_EQ(19,guesses[0]);
  EXPECT_EQ(30 ,guesses[1]);
  EXPECT_EQ(41 ,guesses[2]);

  int count = fitter.findPeaks(&peaks,ampData,idxData);

  EXPECT_EQ(3,count);
  for(int i=0;i<count;i++){
    std::cerr<<peaks[i].amp<<std::endl;
  }

}


TEST_F(GaussianFitterTest, NayaniClipped4){

  // create a vector of integers
  std::vector<int> idxData;
  std::vector<int> ampData;

  char input[] = "2 2 3 2 2 2 2 1 0 1 1 2 2 4 16 49 105 170 217 239 240 223 195 155 110 68 40 25 20 16 16 15 12 10 7 7 7 7 6 5 4 4 3 2 2 3 5 4 4 3 2 3 4 4 3 3 2 2 2 2";

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
  std::vector<int> guesses = fitter.guessPeaks(ampData);
  EXPECT_EQ(3,guesses.size());
  EXPECT_EQ(20,guesses[0]);
  EXPECT_EQ(30 ,guesses[1]);
  EXPECT_EQ(36 ,guesses[2]);

  int count = fitter.findPeaks(&peaks,ampData,idxData);

  EXPECT_EQ(3,count);
  for(int i=0;i<count;i++){
    std::cerr<<peaks[i].amp<<std::endl;
  }

}

TEST_F(GaussianFitterTest, NayaniClipped5){

  // create a vector of integers
  std::vector<int> idxData;
  std::vector<int> ampData;

  char input[] = "1 1 1 1 0 1 3 2 3 4 5 7 16 38 72 109 136 150 150 140 118 87 56 32 23 30 53 82 111 133 145 146 135 121 106 96 87 71 52 35 23 19 16 14 10 8 8 7 6 6 4 4 4 5 5 6 5 4 3 1";

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
  std::vector<int> guesses = fitter.guessPeaks(ampData);
  EXPECT_EQ(2,guesses.size());
  EXPECT_EQ(18,guesses[0]);
  EXPECT_EQ(31 ,guesses[1]);

  int count = fitter.findPeaks(&peaks,ampData,idxData);

  EXPECT_EQ(2,count);
  for(int i=0;i<count;i++){
    std::cerr<<peaks[i].amp<<std::endl;
  }

}

TEST_F(GaussianFitterTest, NayaniClipped6){

  // create a vector of integers
  std::vector<int> idxData;
  std::vector<int> ampData;

  char input[] = "1 1 1 1 2 3 2 2 2 3 5 8 10 11 10 8 7 9 14 28 51 88 131 171 193 193 173 140 117 117 135 151 148 122 88 58 37 23 16 12 11 12 12 12 10 10 10 10 10 8 6 5 5 4 4 3 3 4 3 2";

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
  std::vector<int> guesses = fitter.guessPeaks(ampData);
  EXPECT_EQ(2,guesses.size());
  EXPECT_EQ(25,guesses[0]);
  EXPECT_EQ(31 ,guesses[1]);

  int count = fitter.findPeaks(&peaks,ampData,idxData);

  EXPECT_EQ(2,count);
  for(int i=0;i<count;i++){
    std::cerr<<peaks[i].amp<<std::endl;
  }

}

TEST_F(GaussianFitterTest, NayaniClipped7){

  // create a vector of integers
  std::vector<int> idxData;
  std::vector<int> ampData;

  char input[] = "1 0 1 0 0 1 2 1 2 1 1 1 2 8 19 39 65 87 98 97 89 82 76 79 93 114 130 137 141 152 165 168 153 119 77 46 26 19 15 13 13 13 13 11 10 8 6 6 5 4 4 3 2 2 2 2 3 4 4 4";

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
  std::vector<int> guesses = fitter.guessPeaks(ampData);
  EXPECT_EQ(2,guesses.size());
  EXPECT_EQ(18,guesses[0]);
  EXPECT_EQ(31 ,guesses[1]);

  int count = fitter.findPeaks(&peaks,ampData,idxData);

  EXPECT_EQ(2,count);
  for(int i=0;i<count;i++){
    std::cerr<<peaks[i].amp<<std::endl;
  }

}
TEST_F(GaussianFitterTest, NayaniClipped8){

  // create a vector of integers
  std::vector<int> idxData;
  std::vector<int> ampData;

  char input[] = "4 3 2 2 1 1 1 1 1 3 3 6 11 17 23 26 26 25 27 42 76 124 170 200 211 213 215 219 221 209 178 133 90 60 42 30 23 20 21 20 19 16 13 11 9 6 5 4 3 3 3 3 4 6 6 6 4 3 1 2";

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
  std::vector<int> guesses = fitter.guessPeaks(ampData);
  EXPECT_EQ(3,guesses.size());
  EXPECT_EQ(16,guesses[0]);
  EXPECT_EQ(28 ,guesses[1]);
  EXPECT_EQ(38 ,guesses[2]);

  int count = fitter.findPeaks(&peaks,ampData,idxData);

  EXPECT_EQ(3,count);
  for(int i=0;i<count;i++){
    std::cerr<<peaks[i].amp<<std::endl;
  }

}

//  gsl: svd.c:148: ERROR: SVD decomposition failed to converge
//  Default GSL error handler invoked.
//  /cm/local/apps/slurm/var/spool/job66099/slurm_script: line 17: 138960 Aborted
TEST_F(GaussianFitterTest, 140823_152426_2){

  // create a vector of integers
  std::vector<int> idxData;
  std::vector<int> ampData;

  char input[] = "0 1 1 1 2 1 1 1 1 1 2 2 4 8 15 24 34 41 41 36 29 31 51 90 132 161 163 137 97 57 29 13 7 6 8 11 12 11 8 7 8 17 32 50 60 58 46 30 18 10 4 2 2 4 6 6 6 6 5 4";

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
  std::vector<int> guesses = fitter.guessPeaks(ampData);
  EXPECT_EQ(4,guesses.size());
  EXPECT_EQ(41,guesses[18]);
  EXPECT_EQ(163 ,guesses[25]);
  EXPECT_EQ(11 ,guesses[37]);
  EXPECT_EQ(58, guesses[44]);

  int count = fitter.findPeaks(&peaks,ampData,idxData);

  EXPECT_EQ(4,count);
  for(int i=0;i<count;i++){
    std::cerr<<peaks[i].amp<<std::endl;
  }

}
