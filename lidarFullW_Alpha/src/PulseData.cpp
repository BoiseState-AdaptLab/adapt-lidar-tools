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
  // enter default values

}


/*
 * Populate all the Wave data
 */
void PulseData::populate(WAVESsampling *sampling, 
                              int maxCount, long long pulseIndex){
  waveData.push_back(pulseIndex);
  for(int j = 0; j < sampling->get_number_of_segments(); j++ ){
    sampling->set_active_segment(j);                  
    for(int k = 0; k < maxCount; k++){
      if(k >= sampling->get_number_of_samples()){
        waveData.push_back(0);
      } 
      else{
        waveData.push_back(sampling->get_sample(k));
      }                    
    }
  }              
}

/*
 * Calculate the first diferences
 */
void PulseData::calculateFirstDifference(){
  int first, second, fDiff, count = 0;
  for(int i = 0; i< (int)waveData.size(); i++){
    first = waveData[i+1];
    second = waveData[i+2];

    fDiff = second - first;

    firstDifference.push_back(fDiff);        
    count++;

    if(count == 59){      
        count = 0;
        i = i+2;
    }
  }
}

/*
 * Calculate the second diferences
 */
void PulseData::calculateSecondDifference(){
  int first, second, sDiff, count =0;
  for(int i = 0; i< (int)firstDifference.size(); i++){  
    first = firstDifference[i];
    second = firstDifference[i+1];
    sDiff = std::abs(second - first); //Absolute value

    secondDifference.push_back(sDiff);
    count++;
    if (count == 58){
      count = 0;
      i = i+1;
    }
  
  }
}


/*
 * Calculate smooth second difference
 */
void PulseData::calculateSmoothSecondDifference(){
  int first, second, third, fourth, fifth;
  int median;
  int count = 1;  //Keeps track of the number of 
  for(int i = 0; i< (int)secondDifference.size(); i++){
    if(count == 1 || count == 2){
      smoothSecondDifference.push_back(secondDifference[i]);
      count++;
      continue;
    }
    first = secondDifference[i-2];
    second = secondDifference[i-1];
    third = secondDifference[i];
    fourth = secondDifference[i+1];
    fifth = secondDifference[i+2];
    median = medianOfFive(first, second, third, fourth, fifth);
    smoothSecondDifference.push_back(median);
    count++;
    if(count == 57){
      smoothSecondDifference.push_back(secondDifference[i+1]);
      smoothSecondDifference.push_back(secondDifference[i+2]);
      i = i+2;
      count = 1;
    }

  }

}


/*
 * Take in a vector and calculate its peaks
 */
void PulseData::findPeaks(std::vector<int> data){
  
  const int NOISE = 3; //Level up to and including which peaks will be excluded
  int wideStart = -1;  //The start of any current wide peak

 /* Sign of gradient
  * =  1 for increasing
  * =  0 for level AND PREVIOUSLY INCREASING (so potential wide peak)
  * = -1 for decreasing OR level, but previously decreasing
  * A sharp peak is identified by grad=1 -> grad=-1
  * A wide  peak is identified by grad=0 -> grad=-1
  */ int grad = -1;

  int count = 1;
  for(int i = 0; i<(int)data.size()-1; i++){
    //First index represents the pulse index
    if(count == 1){
      i = i+1;
      count = count + 1;
    }

    //Only possibility of a peak
    if(data[i+1] < data[i]){
      //Sharp peak
      if(grad == 1 && data[i] > NOISE){
        peaks.push_back(data[i]);    //Peak value
        peaksLocation.push_back(i);  //Peak location
      }
      //Wide peak
      else if(grad == 0 && data[i] > NOISE){
        peaks.push_back(data[wideStart]);
        peaksLocation.push_back(wideStart);
      }
      count++;
      grad = -1;
    }
    //Start of a wide peak
    else if (data[i+1] == data[i]){
      count++;
      if(grad == 1){
        wideStart = i;  //Index where the wide peak begins
        grad = 0;
      }
    }
    else{
      grad = 1;
      count++;
    }

    //Keep track of the returning wave index
    if (count == 60){
      count = 1;  
    }

  }
}


/*
 * Find the median of five values
 */
int PulseData::medianOfFive(int a, int b, int c, int d, int e){
  // makes a < b and c < d
  int temp;
  //sort a,b
  if(a > b){
    temp = a;
    a = b;
    b = temp;
  }  
  // sort c,d
  if(c > d){
    temp = c;
    c = d;
    d = temp;
  }  
  // eliminate the lowest
  if (a > c) {
    temp = a;
    a = c;
    c = temp;
  }

  // gets e in
  a = e;
  //sort a,b
  if(a > b){
    temp = a;
    a = b;
    b = temp;
  }  
  // sort c,d
  if(c > d){
    temp = c;
    c = d;
    d = temp;
  }  
  // eliminate the lowest
  if (a > c) {
    temp = a;
    a = c;
    c = temp;
  }

  // sort b,c
  if(b > c){
    temp = b;
    b = c;
    c =temp;
  }  

  if(b<d){
   return b; 
 }
 return d;
}

/*
 * Displays all wave data
 */
void PulseData::displayPulseData(){
  std::cout << "Wave: \n" << std::endl;
  int count = 1;
  for(int i = 0; i<(int)waveData.size(); i++){
    std::cout << waveData[i] << " ";
    count++;
    if(count == 62){
      count = 0;
      std::cout << std::endl ;
    }
  }
  std::cout << "\nFirst diff\n";
  for(int i = 0, j = 1; i<(int)firstDifference.size(); i++, j++){
    std::cout << firstDifference[i] << " ";
    if(j == 59){
      j = 0;
      std::cout << std::endl ;
    }
  }

  std::cout << "\nSecond diff\n";
  count = 1;  
  for(int i = 0; i<(int)secondDifference.size(); i++){
    std::cout << secondDifference[i] << " ";
    
    if(count == 58){
      count = 0;
      std::cout << std::endl ;
    }
    count++;
  }

  std::cout << "\nSmooth second diff\n";
  count = 1;
  for(int i = 0; i<(int)smoothSecondDifference.size(); i++){
    std::cout << smoothSecondDifference[i] << " ";
    if(count == 58){
      count = 0;
      std::cout << std::endl ;
    }
    count++;
  }

}

/*
 *Display the peak Wave and the location of the original wave
 */
void PulseData::displayPeaksAndLocations(){
  for(int i = 0; i<(int)peaks.size(); i++){
    std::cout << "Peak: " << peaks[i] << 
          " found at location: " << peaksLocation[i] << std::endl;    
  }
}

/*
 *Write peaks to file
 */
void PulseData::writePeaksToFile(){
  std::ofstream outfile;
  outfile.open("peaksAndLocations.csv");
  outfile << "Peak Index" << ",";
  outfile << "Peak Value"<< "\n";
  for(int i = 0; i<(int)peaks.size(); i++){
    outfile << peaksLocation[i] << ",";
    outfile << peaks[i] << "\n";
  }
}
