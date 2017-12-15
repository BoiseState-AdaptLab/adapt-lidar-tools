/*
 * File name: PulseData.hpp
 * Created on: 12-July-2017
 * Author: ravi
 */

#ifndef PULSEDATA_HPP_
#define PULSEDATA_HPP_

#include <iostream>
#include <vector>

class PulseData{

private:

  std::vector<I32> *outgoingIdx;
  std::vector<I32> *outgoingWave;
  std::vector<I32> *returningIdx;
  std::vector<I32> *returningWave;

public:  
  PulseData();  

  void displayPulseData();
  void setOutgoing(std::vector<I32> *idxArray, std::vector<I32> *waveArray);
  void setReturning(std::vector<I32> *idxArray, std::vector<I32> *waveArray);

};

#endif /* PULSEDATA_HPP_ */
