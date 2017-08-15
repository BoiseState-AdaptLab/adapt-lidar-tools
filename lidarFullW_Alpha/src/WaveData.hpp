/*
 * File name: WaveData.hpp
 * Created on: 12-July-2017
 * Author: ravi
 */

#ifndef WAVEDATA_HPP_
#define WAVEDATA_HPP_

#include <iostream>
#include <vector>
#include "pulsereader.hpp"
#include "pulsewriter.hpp"

class WaveData{

private:
public:  
  //possible parameters
  std::vector<I32> waveData;

  std::vector<int> firstDifference;
  std::vector<int> secondDifference;
  std::vector<int> smoothSecondDifference;
  
  std::vector<int> peaks;
  std::vector<int> peaksLocation;




  WaveData();  
  void populate(WAVESsampling *sampling, int maxCount, long long pulseIndex);
  
  void calculateFirstDifference();
  void calculateSecondDifference();
  void calculateSmoothSecondDifference();

  void caclulateFirstDifferencePeaks();
  void findPeaks(std::vector<int> data);

  void displayWaveData();
  void displayPeaksAndLocations();

  void writePeaksToFile();

  int medianOfFive(int a, int b, int c, int d, int e);


};

#endif /* WAVEDATA_HPP_ */
