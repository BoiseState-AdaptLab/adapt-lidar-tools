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
  //possible parameters
  std::vector<I32> waveData;

  std::vector<int> firstDifference;
  std::vector<int> secondDifference;
  std::vector<int> smoothSecondDifference;
  
  std::vector<int> peaks;
  std::vector<int> peaksLocation;




  PulseData();  
  void populate(WAVESsampling *sampling, int maxCount, long long pulseIndex);
  
  void calculateFirstDifference();
  void calculateSecondDifference();
  void calculateSmoothSecondDifference();

  void caclulateFirstDifferencePeaks();
  void findPeaks(std::vector<int> data);

  void displayPulseData();
  void displayPeaksAndLocations();

  void writePeaksToFile();

  int medianOfFive(int a, int b, int c, int d, int e);


};

#endif /* PULSEDATA_HPP_ */
