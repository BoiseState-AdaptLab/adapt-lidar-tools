// File name: PulseData.cpp
// Created on: 12-July-2017
// Author: ravi

#include <iostream>
#include <cmath>
#include "PulseData.hpp"
#include <fstream>
#include <cstdlib>

//Default constructor
PulseData::PulseData(){
  outgoingIdx.clear();
  outgoingWave.clear();
  returningIdx.clear();
  returningWave.clear();
}

void PulseData::setOutgoing(std::vector<int> *idxArray,
                            std::vector<int> *waveArray){

  outgoingIdx = *idxArray;
  outgoingWave = *waveArray;
}

void PulseData::setReturning(std::vector<int> *idxArray,
                             std::vector<int> *waveArray){
  returningIdx = *idxArray;
  returningWave = *waveArray;
}


//Displays all wave data 
void PulseData::displayPulseData(std::ostream *outStream){

  *outStream << "\nOutgoing:\n" << std::endl;
  *outStream << "\tindices:" << std::endl;
  *outStream << "\t";
  for(int i = 0; i<(int)outgoingIdx.size(); i++){
    *outStream << outgoingIdx.at(i) << " ";
  }
  *outStream << "\n\tamplitudes:" << std::endl;
  *outStream << "\t";
  for(int i = 0; i<(int)outgoingWave.size(); i++){
    *outStream << outgoingWave.at(i) << " ";
  }

  *outStream << "\n\nReturning Wave:\n" << std::endl;
  *outStream << "\tindices:" << std::endl;
  *outStream << "\t";
  for(int i = 0; i<(int)returningIdx.size(); i++){
    *outStream << returningIdx.at(i) << " ";
  }
  *outStream << "\n\tamplitudes:" << std::endl;
  *outStream << "\t";
  for(int i = 0; i<(int)returningWave.size(); i++){
    *outStream << returningWave.at(i) << " ";
  }
}

/* Experiment with smoothing */
void PulseData::smoothing_expt(std::vector<int> *waveArray){

  for(int i=2; i<waveArray->size()-1;i++){
    if(waveArray->at(i) < 7){
      int temp = (waveArray->at(i-2) + waveArray->at(i-1) + 
                  waveArray->at(i)+waveArray->at(i+1))/4;

      if(abs(temp-waveArray->at(i))<2){
        waveArray->at(i) = temp; 
      }
    }
  }
}

