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


void AmplitudeData::calculateFirstDifference(){
  int first, second;
  int fDiff;
  for(int i = 1; i< waveData.size(); i++){
    if(i% 60 ==0 || i%61 ==0){
    }
    else{
      first = waveData[i];
      second = waveData[i+1];

      fDiff = second - first;

      firstDifference.push_back(fDiff);
    }
  }
}

void AmplitudeData::calculateSecondDifference(){
  int first, second;
  int sDiff;
  for(int i = 0; i< firstDifference.size(); i++){
    if(i% 58 ==0 || i%59 ==0 ){
    }
    else{
      first = firstDifference[i];
      second = firstDifference[i+1];

      sDiff = second - first;

      secondDifference.push_back(sDiff);
    }
  }
}



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
  std::cout << "First Diff\n";
  for(int i =0, j=1; i<firstDifference.size(); i++, j++){
    std::cout << firstDifference[i] << " ";
    if(j == 59){
      j = 0;
      std::cout << std::endl ;
    }
  }
  std::cout << "\nSecond Diff\n";
  for(int i =0, j=1; i<secondDifference.size(); i++, j++){
    std::cout << secondDifference[i] << " ";
    if(j == 58){
      j = 0;
      std::cout << std::endl ;
    }
  }
}