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
 * Populate all the Wave data
 */
void AmplitudeData::populate(PULSEreader *pReader, WAVESsampling *sampling, 
                              int maxCount, long long pulseIndex){
  waveData.push_back(pulseIndex);
  for(int j = 0; j < sampling->get_number_of_segments(); j++ ){
    sampling->set_active_segment(j);                  
    for(int k = 0; k < maxCount; k++){
      if(k >= sampling->get_number_of_samples()){
        waveData.push_back(0);
      } 
      else{
        waveData.push_back(sampling->get_sample(k));
      }                    
    }
  }              
}

/**
void AmplitudeData::calculateFirstAndSecondDifference(){
  int first, second, third;
  double firstDifference1, firstDifference2, secondDifference;
  for(int i = 0; i< outgoingWave.size(); i++){
    if(outgoingWave.size()-i == 3){
    }
    else{
      first = outgoingWave.at(i);
      second = outgoingWave.at(i+1);
      third = outgoingWave.at(i+2);

      firstDifference1 = second - first;
      firstDifference2 = third - second;
      secondDifference = firstDifference2-firstDifference1;


      outgoingFirstDifference.push_back(firstDifference1);
      outgoingFirstDifference.push_back(firstDifference2);
      outgoingSecondDifference.push_back(secondDifference);
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
      returningFirstDifference.push_back(firstDifference2);
      returningSecondDifference.push_back(secondDifference);
    }
  }
}
*/



/*
 * Displays all wave data
 */
void AmplitudeData::displayData(){
  std::cout << "Wave: \n" << std::endl;
  for(int i = 0, j = 1; i<waveData.size(); i++, j++){
    std::cout << waveData[i] << " ";
    if(j == 61){
      j = 0;
      std::cout << std::endl ;
    }
  }
}