/*
 * File name: GaussianFitting_unittests.cpp
 * Created on: 13-October-2017
 * Author: ravi
 *
 *
 */
#include "GaussianFitting.hpp"
#include "PulseData.hpp"
#include "pulsereader.hpp"
#include "pulsewriter.hpp"
#include "gtest/gtest.h"



class GaussianFittingTest: public testing::Test{
  public:  
    std::vector<PulseData*> pulses;
    PulseData* pd;

  protected:

  //Function to set up space used by all tests
  virtual void SetUp(){

    //Read the wave file
    std::string fileName=  "etc/140823_183115_1_clipped_test.pls";
    PULSEreadOpener pOpener;  //Pulse read opener object
    PULSEreader *pReader;     //Pulse reader object
    WAVESsampling *sampling;  //Wave sampling object
    pOpener.set_file_name(fileName.c_str());
    pReader = pOpener.open();
    int maxCount = 60;
    long long pulseIndex = 0; // Keep track of the index


    // CRO: This loop won't be able to be here either.
    // We want to stream the data not read it all into memory
    //Populate the wave data
    while(pReader->read_pulse()){

      //Read the waves
      if(pReader->read_waves()){
        pd = new PulseData();
        for(int i = 0; i < pReader->waves->get_number_of_samplings(); i++){          
          sampling = pReader->waves->get_sampling(i);          

          //Based on the type of wave, populate data
          if(sampling->get_type() == PULSEWAVES_OUTGOING){

          /* Data is being populated from the 140823_183115_1_clipped_test.pls 
           * file located in the etc folder. The tests located further below 
           * checks the known values against the values that are being read 
           * from the .pls file
           */
            pd->populateOutgoing(sampling, maxCount, pulseIndex);

          }
          else if(sampling->get_type() == PULSEWAVES_RETURNING){

          /* Data is being populated from the 140823_183115_1_clipped_test.pls 
           * file located in the etc folder. The tests located further below 
           * checks the known values against the values that are being read 
           * from the .pls file
           */            
            pd->populateReturning(sampling, maxCount, pulseIndex);
          }
          else{
            std::cout << "Unknown type: " << sampling->get_type() \
                      << std::endl;
          }
        }
      }
      //No waves
      else{
        std::cout <<"NO DATA!\n" << std::endl;
      }
      pulses.push_back(pd);
      pulseIndex++;
    }

    // CRO: This should not be part of setup. Rather they need to 
    // be called in the unit tests themselves.
    for(int i = 0; i < (int)pulses.size(); i++){
      pulses[i]->calculateFirstDifference();
      pulses[i]->calculateSecondDifference();
      pulses[i]->calculateSmoothSecondDifference();
      pulses[i]->findPeaks(pulses[i]->returningWave, 6, 61);
    }

  }

  GaussianFitting gfit;
};

/******************************************************************************
* 
* Test gaussianFitting() method on returning wave data at pulse index 1
* "2 2 2 1 1 1 1 1 1 0 0 1 9 35 88 155 212 240 237 200 145 87 42 18 12 13 14 
*  15 15 14 13 10 8 8 8 8 7 6 6 4 4 4 3 4 5 6 4 4 3 2 2 1 1 0 1 2 3 4 4 2"
* 
******************************************************************************/
TEST_F(GaussianFittingTest, gaussianFitting){
  int truthPeaks[2] = {240,15};
  int truthPeaksLocation[2] = {18,29};
  //TODO CRO The test will have an object preader
  // test should start by doing a readPulse(preader, amplData, timeData, &n)
  // the return value on readPules should be 0 unless there is an error
  EXPECT_EQ(2, gfit.getNumPeaks());
  EXPECT_NO_THROW(gfit.findPeaks(double* amplData, double* timeData, size_t n, /
                  struct peak* peaks, size_t numPeaks));
  WaveDescription waveDescription;
  gfit.fit(pulseData,&waveDescription)
  // get stuff out of the wave description
  EXPECT_EQ(truthPeaks[0],wd.getAmp(0))



  
}

/******************************************************************
 *
 * Gaussian Curve Data Size test
 *
 * ***************************************************************/
TEST_F(GaussianFittingTest, curveDataSizeTest){
	
	return null;
}

/*****************************************************************
 *
 * Gaussian Peak Amplitude test
 *
 * *************************************************************/
TEST_F(GaussianFittingTest, peakAmplitudeTest){

	return null;
}

/****************************************************************
 *
 * Gaussian Peak Width test
 *
 * *************************************************************/
TEST_F(GaussianFittingTest, peakWidthTest){

	return null;
}

/****************************************************************
 *
 * Gaussian Peak Position test
 *
 * *************************************************************/
TEST_F(GaussianFittingTest, peakPositionTest){

	return null;
}

/*******************************************************************
 *
 *First Derivative test
 *
 * *****************************************************************/
TEST_F(GaussianFittingTest, firstDerivativeTest){

	return null;
}

/*******************************************************************
 *
 * Second Derivative test
 *
 * ****************************************************************/
TEST_F(GaussianFittingTest, secondDerivativeTest){
	
	return null;
}

/******************************************************************
 *
 * Gaussian Smoothing test
 *
 * ***************************************************************/
TEST_F(GaussianFittingTest, smoothingTest){
	return null;
}

/********************************************************************
 * 
 * Gaussian number of peaks test
 * 
 * *****************************************************************/
TEST_F(GaussianFittingTest, numberOfPeaksTest){
	return null;	
}

/*******************************************************************
 *
 * Gaussian median test
 *
 * ****************************************************************/
TEST_F(GaussianFittingTest, medianTest){
	return null;
}

