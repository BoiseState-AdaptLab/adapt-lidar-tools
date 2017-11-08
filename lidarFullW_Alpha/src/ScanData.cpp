#include "PulseData.hpp"
#include "pulsereader.hpp"
#include "pulsewriter.hpp"
#include "WaveReader.hpp"

/*
 * Hides the reading of PulseWaves
 */
void WaveReader::readWaves(std::string fileName){
  PULSEreadOpener pOpener;  //Pulse read opener object
  PULSEreader *pReader;     //Pulse reader object
  WAVESsampling *sampling;  //Wave sampling object
  pOpener.set_file_name(fileName.c_str());
  pReader = pOpener.open();

  while(pReader->read_pulse()){
    if(pReader->read_waves()){
      for(int i = 0; i < pReader->waves->get_number_of_samplings(); i++){
        sampling = pReader->waves->get_sampling(i);          

        //Based on the type of wave, populate data
        if(sampling->get_type() == PULSEWAVES_OUTGOING){

          /* Data is being populated from the 140823_183115_1_clipped_test.pls 
           * file located in the etc folder. The tests located further below 
           * checks the known values against the values that are being read 
           * from the .pls file
           */
          //pd->populateOutgoing(sampling, maxCount, pulseIndex);
          //TODO: We want to stream the data not read it all into memory
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

  }

}
