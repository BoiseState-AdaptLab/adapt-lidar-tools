/*
 * File name: GaussianFitter_unittests.cpp
 * Created on: 13-October-2017
 * Author: ravi
 *
 *
 */
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
  struct peaks peaklist = fitter.findPeaks(ampData,idxData);

  //TODO: check the results
  
}
