/*
 * File name: testDriver.cpp
 * Created on: 18-May-2017
 * Author: ravi
 */
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cmdLine.hpp"
using namespace std;

int testCmdLine();

int main (){
  int failCount = 0;

  failCount += testCmdLine(); // Final fail count

  std::cerr << "All Testing Complete: " << failCount << " tests failed\n";

}

/**
 * This function tests various command line arguments
 */
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

  /**
   * This section contains all good tests
   */

  try{
    /* Test for no command line arguments
     * The program  name is considered an argument so we use noOfArgs = 1
     * If an exception is caught, then the test passes
     * Option: none
     * Argument: none
     * Expected result: Catch the exception and print usage
     */
    noOfArgs = 1;
    strncpy( someArgs[0],"test",4);
    parseCmdLineArgs(noOfArgs,someArgs);
    failCount++;
  }catch(const std::exception& e){
    passCount++;
  }

  try{	
    /* Test for 2 command line arguments
     * option: f
     * argument: none
     * Expected result: Indicate that an argument is required and print usage
     */
    noOfArgs = 2;
    strncpy( someArgs[1],"-f",2);
    parseCmdLineArgs(noOfArgs,someArgs);
    passCount++;
  }catch(const std::exception& e){
    std::cerr << e.what();
    failCount++;
  }

  try{
    /* Test for 2 command line arguments
     * option: h
     * argument: none
     * Expected result: Prints usage
     */
    noOfArgs = 2;
    strncpy( someArgs[1],"-h",2);
    parseCmdLineArgs(noOfArgs,someArgs);
    passCount++;
  }catch(const std::exception& e){
    std::cerr << e.what();
    failCount++;
  }

  /**
   * This section contains all bad tests
   */

  try{
    /* Test for 2 command line arguments
     * option: g
     * argument: none
     * Expected result: Indicate that the option is invalid and print usage
     */
    noOfArgs = 2;
    strncpy( someArgs[1],"-g",2);
    parseCmdLineArgs(noOfArgs,someArgs);
    passCount++;
  }catch(const std::exception& e){
    std::cerr << e.what();
    failCount++;
  }


  std::cerr << "Testing Complete: " << failCount << " of " <<
  failCount+passCount << " tests failed\n";
	return failCount;
}

