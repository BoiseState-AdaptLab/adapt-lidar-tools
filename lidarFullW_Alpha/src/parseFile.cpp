/*
 * parseFile.cpp
 * Created on: May 19, 2017
 * Author: ravi
 */

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parseFile.hpp"

using namespace std;

void parseFile(char *fileName){
  ifstream file;

  file.open(fileName);
  string line;

  if (!file) //checks to see if file opens properly
  {
      cerr << "Error: Could not find the requested file.";
  }
  else
  {
      if (getline(file, line)) cout << line << endl; // Get and print the line.
      file.close(); // Remember to close the file.
  }
}



