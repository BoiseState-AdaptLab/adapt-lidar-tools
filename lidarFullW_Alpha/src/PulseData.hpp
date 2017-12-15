/*
 * File name: PulseData.hpp
 * Created on: 12-July-2017
 * Author: ravi
 */

#ifndef PULSEDATA_HPP_
#define PULSEDATA_HPP_

#include <iostream>
#include <vector>
#include "pulsereader.hpp"
#include "pulsewriter.hpp"

class PulseData{

private:
public:  
  std::vector<I32> outgoingWave;
  std::vector<I32> returningWave;
  PulseData();  
  void populateOutgoing(WAVESsampling *sampling, int maxCount, 
                        long long pulseIndex);
  void populateReturning(WAVESsampling *sampling, int maxCount, 
                        long long pulseIndex);
  void displayPulseData();

};

#endif /* PULSEDATA_HPP_ */
