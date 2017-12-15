/*
 * File name: PulseData_unittests.cpp
 * Created on: 18-July-2017
 * Author: ravi
 *
 * This test requres the sample 140823_183115_1_clipped_test.pls file 
 * and its corresponding wvs file to be in the etc directory to run
 *
 */

#include "gtest/gtest.h"
#include "PulseData.hpp"
#include "pulsereader.hpp"
#include "pulsewriter.hpp"


class PulseDataTest: public testing::Test{
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








  }
};

/******************************************************************************
* 
* Test number of pulses
* 
******************************************************************************/
TEST_F(PulseDataTest, numberOfPulses){
  ASSERT_EQ(4,(int)pulses.size());
}

/******************************************************************************
* 
* Test outgoing wave data at pulse index 0
* 
******************************************************************************/
TEST_F(PulseDataTest, outgoingWaveData0){

  I32 truthOutgoingWaveData[61] = {0,2,2,2,3,2,2,8,28,70,128,177,192,167,118,
                     68,31,12,5,4,5,5,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                     
  //Test size
 ASSERT_EQ(61,pulses[0]->outgoingWave.size());

  //Test data
  for(int i = 0; i<=60; i++){
    ASSERT_EQ(truthOutgoingWaveData[i], pulses[0]->outgoingWave[i]);
  }
}

/******************************************************************************
* 
* Test outgoing wave data at pulse index 1
* 
******************************************************************************/
TEST_F(PulseDataTest, outgoingWaveData1){

  I32 truthOutgoingWaveData[61] = {1,1,2,1,2,2,3,8,24,63,121,173,194,173,126,74,
                     35,14,5,3,4,5,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                     
  //Test size
 ASSERT_EQ(61,pulses[1]->outgoingWave.size());

  //Test data
  for(int i = 0; i<=60; i++){
    ASSERT_EQ(truthOutgoingWaveData[i], pulses[1]->outgoingWave[i]);
  }
}

/******************************************************************************
* 
* Test outgoing wave data at pulse index 2
* 
******************************************************************************/
TEST_F(PulseDataTest, outgoingWaveData2){

  I32 truthOutgoingWaveData[61] = {2,6,5,5,5,3,2,6,21,59,116,168,192,175,128,75,
                    36,15,5,3,4,5,5,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                     
  //Test size
 ASSERT_EQ(61,pulses[2]->outgoingWave.size());

  //Test data
  for(int i = 0; i<=60; i++){
    ASSERT_EQ(truthOutgoingWaveData[i], pulses[2]->outgoingWave[i]);
  }
}

/******************************************************************************
* 
* Test outgoing wave data at pulse index 3
* 
******************************************************************************/
TEST_F(PulseDataTest, outgoingWaveData3){

  I32 truthOutgoingWaveData[61] = {3,3,3,2,2,2,3,6,21,59,115,168,192,176,130,79,
                    39,16,7,6,6,7,6,3,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                     
  //Test size
 ASSERT_EQ(61,pulses[3]->outgoingWave.size());

  //Test data
  for(int i = 0; i<=60; i++){
    ASSERT_EQ(truthOutgoingWaveData[i], pulses[3]->outgoingWave[i]);
  }
}

/******************************************************************************
* 
* Test if pulses have a returning wave
* 
******************************************************************************/
TEST_F(PulseDataTest, hasReturningWave){
  
  //If pulses[index]returningWave.size() > 1, then it contains data
  ASSERT_FALSE(pulses[1]->returningWave.size() <= 0);
  ASSERT_FALSE(pulses[2]->returningWave.size() <= 0); 
  ASSERT_TRUE(pulses[1]->returningWave.size() == 61);
  ASSERT_TRUE(pulses[2]->returningWave.size() == 61); 

  //No data
  ASSERT_TRUE(pulses[0]->returningWave.size() == 0); 
  ASSERT_TRUE(pulses[3]->returningWave.size() == 0); 
  ASSERT_FALSE(pulses[0]->returningWave.size() >= 1); 
  ASSERT_FALSE(pulses[3]->returningWave.size() >= 1); 

}

/******************************************************************************
* 
* Test returning wave data at pulse index 1
* 
******************************************************************************/
TEST_F(PulseDataTest, returningWaveData1){

  I32 truthReturningWaveData[61] = {1,2,2,2,1,1,1,1,1,1,0,0,1,9,35,88,155,212,
                    240,237,200,145,87,42,18,12,13,14,15,15,14,13,10,8,8,8,8,7,
                    6,6,4,4,4,3,4,5,6,4,4,3,2,2,1,1,0,1,2,3,4,4,2};
  //Test size
  ASSERT_EQ(61,pulses[1]->returningWave.size()); 

  //Test data
  for(int i = 0; i<=60; i++){
    EXPECT_EQ(truthReturningWaveData[i], pulses[1]->returningWave[i]);
  }
}

/******************************************************************************
* 
* Test returning wave data at pulse index 2
* 
******************************************************************************/
TEST_F(PulseDataTest, returningWaveData2){
  I32 truthReturningWaveData[61] = {2,1,2,2,3,2,2,1,1,3,2,2,3,5,19,58,121,186,
                    228,238,214,164,106,58,26,13,10,12,15,17,17,16,13,10,7,6,7,
                    6,6,4,6,6,6,5,6,6,6,6,5,4,4,2,2,1,2,2,1,2,2,2,2};
  //Test size
  ASSERT_EQ(61,pulses[2]->returningWave.size()); 

  //Test data
  for(int i = 0; i<=60; i++){
    EXPECT_EQ(truthReturningWaveData[i], pulses[2]->returningWave[i]);
  }
}
ASSERT_EQ(59,pulses[1]->firstDifference.size()); 

  //Test data
  for(int i = 0; i<=58; i++){
    ASSERT_EQ(truthFirstDiffReturningWave[i],pulses[1]->firstDifference[i]);
  }
}
