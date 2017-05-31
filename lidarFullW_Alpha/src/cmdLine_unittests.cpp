/*
 * File name: testDriver.cpp
 * Created on: 24-May-2017
 * Author: ravi
 */


#include "cmdLine.hpp"
#include "gtest/gtest.h"


class CmdLineTest : public testing::Test {
  protected:

  virtual void SetUp(){
    numberOfArgs = 10;
    maxLengthOfArg = 256;
    commonArgSpace = allocateTestArgs(numberOfArgs,maxLengthOfArg); 
  }

  static char** allocateTestArgs(int N,int M){
    char** someArgs = (char**)malloc(sizeof(char*)*N);
    for(int i=0;i<10;i++){
      someArgs[i] = (char*)malloc(sizeof(char)*M);
    } 
    return someArgs;
  }

  char** commonArgSpace;
  int numberOfArgs;
  int maxLengthOfArg;
};

// Tests valid short command line options
TEST_F(CmdLineTest, validShortOption) {

  int noOfArgs = 2;
  strncpy( commonArgSpace[0],"test",4);
   
  strncpy(commonArgSpace[1],"-h",2);
  ASSERT_NO_THROW({
    parseCmdLineArgs(noOfArgs, commonArgSpace);
  });

  noOfArgs = 3;
  strncpy(commonArgSpace[1],"-f",2);
  strncpy(commonArgSpace[2],"someFileName",4);
  ASSERT_NO_THROW({
    parseCmdLineArgs(noOfArgs, commonArgSpace);
  });

}

// Tests valid long command line options
TEST_F(CmdLineTest, validLongOption) {

  int noOfArgs = 2;
  strncpy( commonArgSpace[0],"test",4);   
  strncpy(commonArgSpace[1],"--help",6);
  ASSERT_NO_THROW({
    parseCmdLineArgs(noOfArgs, commonArgSpace);
  });

  noOfArgs = 3;
  strncpy(commonArgSpace[1],"--file",6);
  strncpy(commonArgSpace[2],"file",4);
  ASSERT_NO_THROW({
    parseCmdLineArgs(noOfArgs, commonArgSpace);
  });

}

// Tests missing command line arguments
TEST_F(CmdLineTest, missingCmdLineArg) {

  int noOfArgs = 1;
  strncpy( commonArgSpace[0],"test",4);
  EXPECT_ANY_THROW(parseCmdLineArgs(noOfArgs, commonArgSpace));
}

// Tests invalid command line arguments
TEST_F(CmdLineTest, invalidCmdLineArgs) {

  int noOfArgs = 2;
  strncpy( commonArgSpace[0],"test",4);   
  strncpy(commonArgSpace[1],"-s",2);
  EXPECT_ANY_THROW(parseCmdLineArgs(noOfArgs, commonArgSpace));

  strncpy(commonArgSpace[1],"--who",5);
  EXPECT_ANY_THROW(parseCmdLineArgs(noOfArgs, commonArgSpace));
}



// Step 3. Call RUN_ALL_TESTS() in main().
//
// We do this by linking in src/gtest_main.cc file, which consists of
// a main() function which calls RUN_ALL_TESTS() for us.
//
// This runs all the tests you've defined, prints the result, and
// returns 0 if successful, or 1 otherwise.
//
// Did you notice that we didn't register the tests?  The
// RUN_ALL_TESTS() macro magically knows about all the tests we
// defined.  Isn't this convenient?
