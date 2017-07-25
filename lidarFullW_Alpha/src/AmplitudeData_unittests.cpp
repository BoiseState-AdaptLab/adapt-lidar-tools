/*
 * File name: AmplitudeData.cpp
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
  protected:

  // function to set up space used by all tests
  virtual void SetUp(){
  }

};

/*
* Given the true values of the GPS file we read the pulse file and compare 
* the scanned values to the true values
*/
TEST_F(AmplitudeDataTest, outgoingWave){

  /****************************************************************************
  * 
  * Begin tests
  * 
  ****************************************************************************/
  
  /*
   * This section reads the wave file
   */
  std::string fileName=  "etc/140823_183115_1_clipped_test.pls";
  PULSEreadOpener pOpener;  // Create a pulse read opener object
  PULSEreader *pReader;     // Create a pulse reader object
  WAVESsampling *sampling;  // Create a wave sampling object
  pOpener.set_file_name(fileName.c_str());
  pReader = pOpener.open();
  int maxCount = 60;
  long long pulseIndex = 0; // Keep track of the index


  AmplitudeData outgoingWave;
  AmplitudeData returningWave;


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

  for(int i =0; i<=243; i++){
    ASSERT_EQ(truthOutgoingWaveData[i], outgoingWave.waveData[i]);
  }

   
}
