/*
 * File name: cmd_line.cpp
 * Created on: 02-June-2017
 * Author: ravi
 */

#include <iostream>
#include <fstream>
#include <getopt.h>
#include <sstream>
#include "FullWaveFormIngestion.hpp"

using namespace std;


//Default constructor
FullWaveFormIngestion::FullWaveFormIngestion(){
  // enter default values
  time = 1000.;

}

std::string CmdLineArgs::getInputFileName(){
  return inputFileName;
}

/*
* Calculate the actual coordinate
* Could be either actual anchor or actual target for x, y and z
*/
double FullWaveFormIngestion::calculateActualCoordinate(double coordinate, 
                              double scaleFactor, double offset){
  double actualCoordinate = (coordinate * scaleFactor) + offset;
  return actualCoordinate;
}

/*
* Calculate the deviation
* Could be for x, y or z
*/
double FullWaveFormIngestion::calculateDeviation(double anchor, double target){
  double deviation = (anchor - target)/time;
  return deviation;
}

/*
* Calculate the return location
* Could be for x, y or z
*/
double FullWaveFormIngestion::calculateReturnLocation(double actualAnchor, 
                              double deviation, double time){
  double returnLocation = actualAnchor + (time * deviation);
  return returnLocation;
}
