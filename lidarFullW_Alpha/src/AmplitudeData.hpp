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
  std::vector<I32> waveData;

  std::vector<int> firstDifference;
  std::vector<int> secondDifference;
  std::vector<int> smoothSecondDifference;
  
  std::vector<int> peaks;


  AmplitudeData();  
  void populate(WAVESsampling *sampling, int maxCount, long long pulseIndex);
  void displayData();
  void calculateFirstDifference();
  void calculateSecondDifference();
  void calculateSmoothSecondDiference();

  void countPeaks();

  int medianOfFive(int a, int b, int c, int d, int e);


};

#endif /* AMPLITUDEDATA_HPP_ */