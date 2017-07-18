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
void AmplitudeData::populateAmplitude(PULSEreader *pReader, \
                                      WAVESsampling *sampling, int maxCount, \
                                      long long pulseIndex){
  for(int i = 0; i < pReader->waves->get_number_of_samplings(); i++){
              sampling = pReader->waves->get_sampling(i);

    //Outgoing wave?
    if(sampling->get_type() == PULSEWAVES_OUTGOING){
      outgoingWave.push_back(pulseIndex);
      for(int j = 0; j < sampling->get_number_of_segments(); j++ ){
        sampling->set_active_segment(j);                  
        for(int k = 0; k < maxCount; k++){
          if(k >= sampling->get_number_of_samples()){
            outgoingWave.push_back(0);
          } 
          else{
            outgoingWave.push_back(sampling->get_sample(k));
          }                    
        }
      }              
    }

    //Returning wave?
    else if(sampling->get_type() == PULSEWAVES_RETURNING){
      returningWave.push_back(pulseIndex);
      for(int j = 0; j < sampling->get_number_of_segments(); j++ ){
        sampling->set_active_segment(j);                  
        for(int k = 0; k < maxCount; k++){
          if(k >= sampling->get_number_of_samples()){
            returningWave.push_back(0);
          } 
          else{
            returningWave.push_back(sampling->get_sample(k));
          }                    
        }
      } 
    }

    //Unknown wavetype
    else{
      std::cout << "Unknown type: " << sampling->get_type() << std::endl;
    }

  }
}

void AmplitudeData::calculateFirstAndSecondDifference(){
  int first, second, third;
  double firstDifference1, firstDifference2, secondDifference1, secondDifference2;
  for(int i = 0; i< outgoingWave.size(); i++){
    if(outgoingWave.size()-i == 3){
    }
    else{
      first = outgoingWave.at(i);
      second = outgoingWave.at(i+1);
      third = outgoingWave.at(i+2);

      firstDifference1 = second - first;
      firstDifference2 = third - second;


      outgoingFirstDifference.push_back(firstDifference1);
      outgoingFirstDifference.push_back(firstDifference2);
    }
  }
  for(int i = 0; i< returningWave.size(); i++){
    if(outgoingWave.size()-i == 3){
    }
    else{
      first = returningWave.at(i);
      second = returningWave.at(i+1);
      third = returningWave.at(i+2);

      firstDifference1 = second - first;
      firstDifference2 = third - second;


      returningFirstDifference.push_back(firstDifference1);
      outgoingFirstDifference.push_back(firstDifference2);
    }
  }
}



/*
 * Displays all wave data
 */
void AmplitudeData::displayAmplitudeData(){
  std::cout << "Outgoing Wave: \n" << std::endl;
  for(int i = 0, j = 1; i<outgoingWave.size(); i++, j++){
    std::cout << outgoingWave[i] << " ";
    if(j == 61){
      j = 0;
      std::cout << std::endl ;
    }
  }
  std::cout << "\nReturning Wave: \n" << std::endl;
  for(int i = 0, j = 1; i<returningWave.size(); i++, j++){
    std::cout << returningWave[i] << " ";
    if(j == 61){
      j = 0;
      std::cout << std::endl ;
    }
  }
}