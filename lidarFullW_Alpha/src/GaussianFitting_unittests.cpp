/*
 * File name: Waveform_unittests.cpp
 * Created on: 12-July-2017
 * Author: olschanowsky
 *
 *
 */

#include "gtest/gtest.h"


class WaveformGaussianFittingTest: public testing::Test{
  protected:

  // function to set up space used by all tests
  virtual void SetUp(){
  }

};

/*
* Given an array of data choose the peaks. 
* 
*/
TEST_F(WaveformGaussianFittingTest, findPeaks){

  // this is a single sample incoming waveform
  char outgoing[] = " 1 2 1 2 2 3 8 24 63 121 173 194 173 126 74 35 14 5 3 4 5 4 2 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0";
  char input[] = "2 2 2 1 1 1 1 1 1 0 0 1 9 35 88 155 212 240 237 200 145 87 42 18 12 13 14 15 15 14 13 10 8 8 8 8 7 6 6 4 4 4 3 4 5 6 4 4 3 2 2 1 1 0 1 2 3 4 4 2";
  std::Vector<double> waveIn;
    char* ptr;
  ptr = strtok (input," ");
  i=0;
  while (ptr != NULL){
    double t = (double)i;
    double y0 = atof(ptr);
    fprintf(stderr,"%f\n",y0);
    timeIn.push_back(t);
    waveIn.push_back(y0);
    i++;
    ptr = strtok (NULL," ");
  }
  ptr = strtok (outgoing," ");
  i=0;
  while (ptr != NULL){
    double t = (double)i;
    double y0 = atof(ptr);
    timeOut.push_back(t);
    waveOut.push_back(y0);
    i++;
    ptr = strtok (NULL," ");
  }
  
  // now that we have some sample data call the gaussian fit function
  WaveformGaussianFit wgf;
  wgf.fitPairData(waveOut,timeOut,waveIn,timeIn);
  ASSERTEQ(4,wgf.getPeakCount());
  
}
