/*
 * File name: testDriver.cpp
 * Created on: 18-May-2017
 * Author: ravi
 */
#include <iostream>
#include <vector>
#include "cmdLine.hpp"
using namespace std;


int main (){

  int passCount = 0;
  int failCount = 0;

	int noOfArgs;
	char* someArgs;

  someArgs = (char*)malloc(sizeof(char)*256);
  if(someArgs == NULL){
    fprintf(stderr,"FAILURE: Malloc failed for testing\n");
    return 0;
  }

  try{
	//Test for only 1 command line argument
	noOfArgs = 1;
	parseCmdLineArgs(noOfArgs,NULL);
    passCount++;
  }catch(const std::exception& e){
    std::cerr << e.what();
    failCount++;
  }

  try{	
	//Test for 2 command line arguments
	noOfArgs = 2;
	parseCmdLineArgs(noOfArgs,&someArgs);
    passCount++;
  }catch(const std::exception& e){
    std::cerr << e.what();
    failCount++;
  }


  std::cerr << "Testing Complete: " << failCount << " of " << failCount+passCount << " tests failed\n";
	return 0;
}
