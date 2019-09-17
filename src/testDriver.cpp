/*
 * File name: testDriver.cpp
 * Created on: 18-May-2017
 * Author: ravi
 */

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "cmdLine.hpp"
#include "parseFile.hpp"

// Activity level must be defined before spdlog is included.
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"


int testCmdLine();
int testFileReader();

int main (){

    // Setting up logger
    spdlog::set_pattern("[%^%=8l%$] %v");
    // Sets new pattern for timestamp
    
    auto logger = spdlog::create_async<spdlog::sinks::stdout_color_sink_mt>(
                   "logger");



    int failCount = 0;

    // Call all tests associated with cmdLine parsing
    failCount += testCmdLine();

    // Call all tests associated with fileReading
    failCount += testFileReader();

    spdlog::error("All Testing Complete: {} tests failed\n", failCount);
        
}

/**
 * This function tests the file reader
 */
int testFileReader(){

    int passCount = 0;
    int failCount = 0;

    return failCount;

}

/**
 * This function tests various command line arguments
 * Please note: optind = 0; This line must appear before
 * each unit test. The getopt_long library maintains this
 * variable as state and needs to be reset for each test
 * to make them independent
 */
int testCmdLine(){

    int passCount = 0;
    int failCount = 0;

    int noOfArgs;
    char** someArgs;

    // set up some space to play around with
    // command line parsing
    someArgs = (char**)malloc(sizeof(char*)*10);
    for(int i=0;i<10;i++){
        someArgs[i] = (char*)malloc(sizeof(char)*256);
    } 
    // if getting space failed just return with a failure
    if(someArgs == NULL){
        spdlog::error("FAILURE: Malloc failed for testing");  
            
        return 1;
    }

    /**
     * This section contains all good tests
     */

    try{
        optind = 0;
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
        spdlog::error("\nFAIL: Test 1 - No command line arguments");
        failCount++;
    }catch(const std::exception& e){
        passCount++;
        std::cerr << e.what();
    }

    try{
        optind = 0;
        /* Test for 2 command line arguments
         * option: f
         * argument: none
         * Expected result:Indicate that an argument is required and print usage
         */
        noOfArgs = 2;
        strncpy( someArgs[1],"-f",2);
        parseCmdLineArgs(noOfArgs,someArgs);
        failCount++;
        spdlog::error("FAIL: Test 2 - Valid option '-f' without argument"); 
           
    }catch(const std::exception& e){
        passCount++;
        std::cerr << e.what();
    }

    try{
        optind = 0;
        /* Test for 2 command line arguments
         * option: f
         * argument: none
         * Expected result:Indicate that an argument is required and print usage
         */
        noOfArgs = 2;
        strncpy( someArgs[1],"-g",2);
        parseCmdLineArgs(noOfArgs,someArgs);
        failCount++;
        spdlog::error("FAIL: Test 3 - Invalid option '-f' without argument");
            
    }catch(const std::exception& e){
        passCount++;
        std::cerr << e.what();
    }


    spdlog::error("\nTesting Complete: {} of {} tests failed", failCount, 
                  failCount+passCount);
    return failCount;
}


