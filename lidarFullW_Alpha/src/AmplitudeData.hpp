/*
 * File name: AmplitudeData.hpp
 * Created on: 12-July-2017
 * Author: ravi
 */

#ifndef AMPLITUDEDATA_HPP_
#define AMPLITUDEDATA_HPP_

#include <iostream>
#include <vector>
#include "pulsereader.hpp"
#include "pulsewriter.hpp"

class AmplitudeData{

private:
public:  
  //possible parameters
  std::vector<I32> outgoingWave;
  std::vector<I32> returningWave;

  std::vector<int> outgoingFirstDifference;
  std::vector<int> outgoingSecondDifference;

  std::vector<int> returningFirstDifference;
  std::vector<int> returningSecondDifference;


  AmplitudeData();  
  void populateAmplitude(PULSEreader *pReader, WAVESsampling *sampling, \
                         int maxCount, long long pulseIndex);
  void displayAmplitudeData();
  void calculateFirstAndSecondDifference();


};

#endif /* AMPLITUDEDATA_HPP_ */