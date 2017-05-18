/*
 * File name: testDriver.cpp
 * Created on: 18-May-2017
 * Author: ravi
 */
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cmdLine.hpp"
using namespace std;

int testCmdLine();

int main (){
  int failCount = 0;

  failCount += testCmdLine();

  std::cerr << "All Testing Complete: " << failCount << " tests failed\n";

}

int testCmdLine(){

  int passCount = 0;
  int failCount = 0;

	int noOfArgs;
	char** someArgs;

  someArgs = (char**)malloc(sizeof(char*)*10);
  for(int i=0;i<10;i++){
    someArgs[i] = (char*)malloc(sizeof(char)*256);
  }
  if(someArgs == NULL){
    fprintf(stderr,"FAILURE: Malloc failed for testing\n");
    return 0;
  }

  try{
    //Test for only 1 command line argument
    noOfArgs = 1;
    parseCmdLineArgs(noOfArgs,NULL);
    failCount++;
  }catch(const std::exception& e){
    passCount++;
  }

  try{	
	//Test for 2 command line arguments
	  noOfArgs = 2;
    strncpy( someArgs[0],"test",4);  
    strncpy( someArgs[1],"-f",2);  
	  parseCmdLineArgs(noOfArgs,someArgs);
    passCount++;
  }catch(const std::exception& e){
    std::cerr << e.what();
    failCount++;
  }


  std::cerr << "Testing Complete: " << failCount << " of " << failCount+passCount << " tests failed\n";
	return failCount;
}
