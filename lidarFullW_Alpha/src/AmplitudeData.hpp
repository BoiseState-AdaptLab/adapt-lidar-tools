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
  std::vector<int> smoothSecondDifferencePeaks;
  
  std::vector<int> firstDifferencePeaks;
  std::vector<int> firstDifferencePeaksLocation;




  AmplitudeData();  
  void populate(WAVESsampling *sampling, int maxCount, long long pulseIndex);
  
  void calculateFirstDifference();
  void calculateSecondDifference();
  void calculateSmoothSecondDifference();

  void caclulateFirstDifferencePeaks();
  void findPeaks(std::vector<int> data);

  void displayWaveData();
  void displayPeaksAndLocations();

  int medianOfFive(int a, int b, int c, int d, int e);


};

#endif /* AMPLITUDEDATA_HPP_ */