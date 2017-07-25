/*
 * File name: AmplitudeData_unittests.cpp
 * Created on: 18-July-2017
 * Author: ravi
 *
 * This test requres the sample 140823_183115_1_clipped_test.pls file 
 * and its corresponding wvs file to be in the etc directory to run
 *
 */

#include "gtest/gtest.h"
#include "AmplitudeData.hpp"
#include "pulsereader.hpp"
#include "pulsewriter.hpp"


class AmplitudeDataTest: public testing::Test{
  public:  
  AmplitudeData outgoingWave;
  AmplitudeData returningWave;
  protected:

  // function to set up space used by all tests
  virtual void SetUp(){
  
  std::string fileName=  "etc/140823_183115_1_clipped_test.pls";
  PULSEreadOpener pOpener;  // Create a pulse read opener object
  PULSEreader *pReader;     // Create a pulse reader object
  WAVESsampling *sampling;  // Create a wave sampling object
  pOpener.set_file_name(fileName.c_str());
  pReader = pOpener.open();
  int maxCount = 60;
  long long pulseIndex = 0; // Keep track of the index

  //Populate the wave data
  while(pReader->read_pulse()){

    //Read the waves
    if(pReader->read_waves()){
      for(int i = 0; i < pReader->waves->get_number_of_samplings(); i++){
        sampling = pReader->waves->get_sampling(i);

        //Based on the type of wave, populate data
        if(sampling->get_type() == PULSEWAVES_OUTGOING){
          outgoingWave.populate(pReader, sampling, maxCount, pulseIndex);

        }
        else if(sampling->get_type() == PULSEWAVES_RETURNING){
          returningWave.populate(pReader, sampling, maxCount, pulseIndex);
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
    
    pulseIndex++;
  }
  }

};

/*
* Given the true values of the GPS file we read the pulse file and compare 
* the scanned values to the true values
*/
TEST_F(AmplitudeDataTest, outgoingWave){

  /****************************************************************************
  * 
  * This section reads the wave file
  * 
  ****************************************************************************/

  /****************************************************************************
  * 
  * Test outgoing wave data
  * 
  ****************************************************************************/

  I32 truthOutgoingWaveData [244] = {0,2,2,2,3,2,2,8,28,70,128,177,192,167,118,
                     68,31,12,5,4,5,5,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     1,1,2,1,2,2,3,8,24,63,121,173,194,173,126,74,35,14,5,
                     3,4,5,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     2,6,5,5,5,3,2,6,21,59,116,168,192,175,128,75,36,15,5,
                     3,4,5,5,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     3,3,3,2,2,2,3,6,21,59,115,168,192,176,130,79,39,16,7,
                     6,6,7,6,3,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  for(int i = 0; i<=243; i++){
    ASSERT_EQ(truthOutgoingWaveData[i], outgoingWave.waveData[i]);
  }

  /****************************************************************************
  * 
  * Test returning wave data
  * 
  ****************************************************************************/
  I32 truthReturningWaveData [122] = {1,2,2,2,1,1,1,1,1,1,0,0,1,9,35,88,155,212,
                    240,237,200,145,87,42,18,12,13,14,15,15,14,13,10,8,8,8,8,7,
                    6,6,4,4,4,3,4,5,6,4,4,3,2,2,1,1,0,1,2,3,4,4,2,
                    2,1,2,2,3,2,2,1,1,3,2,2,3,5,19,58,121,186,228,238,214,164,
                    106,58,26,13,10,12,15,17,17,16,13,10,7,6,7,6,6,4,6,6,6,5,6,
                    6,6,6,5,4,4,2,2,1,2,2,1,2,2,2,2};

  for(int i = 0; i<=122; i++){
    EXPECT_EQ(truthReturningWaveData[i], returningWave.waveData[i]);
  }

  /****************************************************************************
  * 
  * Test first difference
  * 
  ****************************************************************************/
int truthFirstDifferenceOutgoingWave [236] = {0,0,1,-1,0,6,20,42,58,49,15,-25,
                  -49,-50,-37,-19,-7,-1,1,0,-2,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                  1,-1,1,0,1,5,16,39,58,52,21,-21,-47,-52,-39,-21,-9,-2,1,1,-1,
                  -2,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,0,
                  -1,0,0,-2,-1,4,15,38,57,52,24,-17,-47,-53,-39,-21,-10,-2,1,1,
                  0,-2,-2,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,0,0,
                  0,-1,0,0,1,3,15,38,56,53,24,-16,-46,-51,-40,-23,-9,-1,0,1,-1,
                  -3,-2,-1,0,0,1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,0};
  for(int i = 0; i<=236; i++){
    ASSERT_EQ(truthFirstDifferenceOutgoingWave[i],
              outgoingWave.firstDifference[i]);
  }
}
