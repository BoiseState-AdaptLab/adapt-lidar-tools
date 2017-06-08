/*
 * File name: lidar_driver.cpp
 * Created on: 17-May-2017
 * Author: ravi
 */
#include <iostream>
#include "cmdLine.hpp"
#include "parseFile.hpp"

using namespace std;

int main (int argc,char *argv[]){

  CmdLineArgs cla;
  cla.parse(argc,argv);
  if(cla.printUsageMessage == true)
    std::cout <<cla.getUsageMessage();

  return 0;
}

