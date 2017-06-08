/*
 * parseFile.cpp
 * Created on: May 19, 2017
 * Author: ravi
 */

#include <iostream>
#include <fstream>
#include <string>
#include "parseFile.hpp"

void parseFile(char *fileName){
  std::ifstream file;

  file.open(fileName);
  std::string line;

  if (!file){ //checks to see if file opens properly
    std::cerr << "Error: Could not find the requested file." << std::endl;
  }
  else{
    if (getline(file, line)){
      std::cout << line << std::endl; // Get and print the line.
      file.close(); // Remember to close the file.
    }
  }
}



