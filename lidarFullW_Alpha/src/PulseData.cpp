/*
 * File name: PulseData.cpp
 * Created on: 12-July-2017
 * Author: ravi
 */

#include <iostream>
#include <cmath>
#include "PulseData.hpp"
#include <fstream>

//Default constructor
PulseData::PulseData(){
}

void PulseData::setOutgoing(std::vector<I32> *idxArray,
                            std::vector<I32> *waveArray){

  outgoingIdx = idxArray;
  outgoingWave = waveArray;
}

void PulseData::setReturning(std::vector<I32> *idxArray, 
                             std::vector<I32> *waveArray){
  returningIdx = idxArray;
  returningWave = waveArray;
}



/*
 * Displays all wave data
 */
void PulseData::displayPulseData(osstream outStream){

  outStream << "Outgoing:\n" << std::endl;
  outStream << "\tindices:\n" << std::endl;
  for(int i = 0; i<(int)outgoingIdx.size(); i++){
    outStream << outgoingIdx[i] << " ";
  }
  outStream << "\tamplitudes:\n" << std::endl;
  for(int i = 0; i<(int)outgoingWave.size(); i++){
    outStream << outgoingWave[i] << " ";
  }

  outStream << "\nReturning Wave:\n" << std::endl;
  outStream << "\tindices:\n" << std::endl;
  for(int i = 0; i<(int)returningIdx.size(); i++){
    outStream << returningWave[i] << " ";
  }
  outStream << "\tamplitudes:\n" << std::endl;
  for(int i = 0; i<(int)returningWave.size(); i++){
    outStream << returningWave[i] << " ";
  }
}
