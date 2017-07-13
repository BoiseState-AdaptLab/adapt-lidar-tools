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

}

/*
 * Displays all GPS data
 */
void AmplitudeData::displayAmplitudeData(){
  std::cout << "Outgoing Wave: \n" << std::endl;
  for(int i = 0, j = 1; i<outgoingWave.size(); i++, j++){
    std::cout << outgoingWave[i] << " ";
    if(j == 60){
      j = 0;
      std::cout << "\n" ;
    }
  }
  std::cout << "\nReturning Wave: \n" << std::endl;
  for(int i = 0, j = 1; i<returningWave.size(); i++, j++){
    std::cout << returningWave[i] << " ";
    if(j == 60){
      j = 0;
      std::cout << "\n" ;
    }
  }
}