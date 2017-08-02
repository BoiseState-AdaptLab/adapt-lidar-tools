/*
 * File name: AmplitudeData.cpp
 * Created on: 12-July-2017
 * Author: ravi
 */

#include <iostream>
#include <cmath>
#include "AmplitudeData.hpp"

//Default constructor
AmplitudeData::AmplitudeData(){
  // enter default values

}


/*
 * Populate all the Wave data
 */
void AmplitudeData::populate(WAVESsampling *sampling, 
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
void AmplitudeData::calculateFirstDifference(){
  int first, second, fDiff, count = 0;
  for(int i = 0; i< waveData.size(); i++){
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
void AmplitudeData::calculateSecondDifference(){
  int first, second, sDiff, count =0;
  for(int i = 0; i< firstDifference.size(); i++){  
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
void AmplitudeData::calculateSmoothSecondDifference(){
  int first, second, third, fourth, fifth;
  int median;
  int count = 1;  //Keeps track of the number of 
  for(int i = 0; i< secondDifference.size(); i++){
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
 * Check if values change from +ve to -ve
 */
void AmplitudeData::countPeaks(){
    for(int i = 0; i<firstDifference.size()-1; i++){
      if(firstDifference[i] > 0 && firstDifference[i+1] < 0){

      }
    }

}


/*
 * Find the median of five values
 */
int AmplitudeData::medianOfFive(int a, int b, int c, int d, int e){
  // makes a < b and c < d
  int temp;
  //sort a,b
  if(a > b){
    temp =a;
    a =b;
    b=temp;
  }  
  // sort c,d
  if(c > d){
    temp =c;
    c =d;
    d=temp;
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
    temp =a;
    a =b;
    b=temp;
  }  
  // sort c,d
  if(c > d){
    temp =c;
    c =d;
    d=temp;
  }  
  // eliminate the lowest
  if (a > c) {
    temp = a;
    a = c;
    c = temp;
  }

  // sort b,c
  if(b > c){
    temp =b;
    b =c;
    c=temp;
  }  

  if(b<d){
   return b; 
 }
 return d;
}

/*
 * Displays all wave data
 */
void AmplitudeData::displayData(){
  std::cout << "Wave: \n" << std::endl;
  int count = 1;
  for(int i = 0; i<waveData.size(); i++){
    std::cout << waveData[i] << " ";
    count++;
    if(count == 62){
      count = 0;
      std::cout << std::endl ;
    }
  }
  std::cout << "\nFirst diff\n";
  for(int i = 0, j = 1; i<firstDifference.size(); i++, j++){
    std::cout << firstDifference[i] << " ";
    if(j == 59){
      j = 0;
      std::cout << std::endl ;
    }
  }

  std::cout << "\nSecond diff\n";
  count = 1;  
  for(int i = 0; i<secondDifference.size(); i++){
    std::cout << secondDifference[i] << " ";
    
    if(count == 58){
      count = 0;
      std::cout << std::endl ;
    }
    count++;
  }

  std::cout << "\nSmooth second diff\n";
  count = 1;
  for(int i = 0; i<smoothSecondDifference.size(); i++){
    std::cout << smoothSecondDifference[i] << " ";
    if(count == 58){
      count = 0;
      std::cout << std::endl ;
    }
    count++;
  }

}