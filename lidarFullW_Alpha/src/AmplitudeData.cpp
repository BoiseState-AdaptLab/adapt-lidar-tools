/*
 * File name: AmplitudeData.cpp
 * Created on: 12-July-2017
 * Author: ravi
 */

#include <iostream>
#include "AmplitudeData.hpp"

//Default constructor
AmplitudeData::AmplitudeData(){
  // enter default values

}


/*
 * Populate all the GPS data
 */
void AmplitudeData::populateAmplitudeData(PULSEreader *pReader, WAVESsampling *sampling){
  int returnCount = 0;
  int maxCount = 120;
  for(int i = 0; i < pReader->waves->get_number_of_samplings() && returnCount < 1; i++){
    sampling = pReader->waves->get_sampling(i);

    for(int j = 0; j < sampling->get_number_of_segments(); j++ ){
      sampling->set_active_segment(j);
        
        for(int k = 0; k < maxCount; k++){
          
          if(k >= sampling->get_number_of_samples()){
              data.push_back(0);
          } 
          else{
              data.push_back(sampling->get_sample(k));
          }
        }
    }

  }
}

/*
 * Displays all GPS data
 */
void AmplitudeData::displayAmplitudeData(){
  for(int i = 0, j = 1; i<data.size(); i++, j++){
    std::cout << data[i] << " ";
    if(j == 60){
      j = 0;
      std::cout << "\n" ;
    }
  }
}