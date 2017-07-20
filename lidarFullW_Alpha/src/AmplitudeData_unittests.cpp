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

  AmplitudeData ad;
  std::vector<I32> waveData;
  std::vector<int> firstDifference;
  std::vector<int> secondDifference;

  // function to set up space used by all tests
  virtual void SetUp(){
  }

};

/*
* Given the true values of the GPS file we read the pulse file and compare 
* the scanned values to the true values
*/
TEST_F(AmplitudeDataTest, outgoingWave){

  std::string fileName=  "etc/140823_183115_1_clipped_test.pls";

  /*
   * This section reads the wave file
   */
  AmplitudeData ad;       // Create an AmplitudeData object

  PULSEreadOpener pOpener;  // Create a pulse read opener object
  PULSEreader *pReader;     // Create a pulse reader object
  pOpener.set_file_name(fileName.c_str());
  pReader = pOpener.open();
  long long pulseIndex = 0; // Keep track of the index

  /****************************************************************************
  * 
  * Begin tests
  * 
  ****************************************************************************/
  I32[61] truthWaveData = {0,2,2,2,3,2,2,8,28,70,128,177,192,167,118,68,31,12,
                           5,4,5,5,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                           0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  while(pReader->read_pulse()){
    if(pReader->read_waves()){
      for(int i = 0; i < pReader->waves->get_number_of_samplings(); i++){
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
  }
   
}
