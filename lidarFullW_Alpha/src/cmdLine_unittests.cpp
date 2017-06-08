/*
 * File name: testDriver.cpp
 * Created on: 24-May-2017
 * Author: ravi
 */


#include "cmdLine.hpp"
#include "gtest/gtest.h"
#include <getopt.h>


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

/****************************************************************************
* 
* Begin valid command line option tests
* 
****************************************************************************/

// Tests valid short command line options
TEST_F(CmdLineTest, validShortCmdLineOpts) {
  optind = 0;
  int noOfArgs = 2;
  strncpy( commonArgSpace[0],"test",5);   
  strncpy(commonArgSpace[1],"-h",3);
  CmdLineArgs cmd;
  ASSERT_NO_THROW({
    cmd.parse(noOfArgs, commonArgSpace);
  });
  ASSERT_TRUE(cmd.printUsageMessage);

  CmdLineArgs cmd2;
  optind=0;
  noOfArgs = 3;
  strncpy(commonArgSpace[1],"-f",3);
  strncpy(commonArgSpace[2],"someFileName",13);
  ASSERT_NO_THROW({
    cmd2.parse(noOfArgs, commonArgSpace);
  });
  ASSERT_FALSE(cmd2.printUsageMessage);
  ASSERT_EQ("someFileName",cmd2.getInputFileName());
}

// Tests valid long command line options
TEST_F(CmdLineTest, validLongCmdLineOpts) {
  optind = 0;
  int noOfArgs = 2;
  strncpy( commonArgSpace[0],"test",5);   
  strncpy(commonArgSpace[1],"--help",7);
  CmdLineArgs cmd;
  ASSERT_NO_THROW({
    cmd.parse(noOfArgs, commonArgSpace);
  });
  ASSERT_TRUE(cmd.printUsageMessage);

  CmdLineArgs cmd2;
  optind = 0;
  noOfArgs = 3;
  strncpy(commonArgSpace[1],"--file",7);
  strncpy(commonArgSpace[2],"file",5);
  ASSERT_NO_THROW({
    cmd2.parse(noOfArgs, commonArgSpace);
  });
  ASSERT_FALSE(cmd2.printUsageMessage);
  ASSERT_EQ("file",cmd2.getInputFileName());
}



/****************************************************************************
* 
* Begin invalid command line option tests
* 
****************************************************************************/

// Tests missing command line arguments
TEST_F(CmdLineTest, missingCmdLineArg) {
  optind = 0;
  int noOfArgs = 1;
  strncpy(commonArgSpace[0],"test",5);
  CmdLineArgs cmd;
  EXPECT_ANY_THROW(cmd.parse(noOfArgs, commonArgSpace));
}

// Tests missing short option arguments
TEST_F(CmdLineTest, missingShortOptArg) {
  optind = 0;
  int noOfArgs = 2;
  strncpy(commonArgSpace[0],"test",5);
  strncpy(commonArgSpace[1],"-f",3);
  CmdLineArgs cmd;
  EXPECT_ANY_THROW(cmd.parse(noOfArgs, commonArgSpace));
}

// Tests missing long option arguments
TEST_F(CmdLineTest, missingLongOptArg) {
  optind = 0;
  int noOfArgs = 2;
  strncpy(commonArgSpace[0],"test",5);
  strncpy(commonArgSpace[1],"--file",7);
  CmdLineArgs cmd;
  EXPECT_ANY_THROW(cmd.parse(noOfArgs, commonArgSpace));
}

// Tests invalid short command line options
TEST_F(CmdLineTest, invalidShortCmdLineOpts) {
  optind = 0;
  int noOfArgs = 2;
  strncpy( commonArgSpace[0],"test",5);   
  strncpy(commonArgSpace[1],"-s",3);
  CmdLineArgs cmd;
  EXPECT_ANY_THROW(cmd.parse(noOfArgs, commonArgSpace));
}

// Tests invalid long command line options
TEST_F(CmdLineTest, invalidLongCmdLineOpts) {
  optind = 0;
  int noOfArgs = 2;
  strncpy( commonArgSpace[0],"test",5);   
  strncpy(commonArgSpace[1],"--who",6);
  CmdLineArgs cmd;
  EXPECT_ANY_THROW(cmd.parse(noOfArgs, commonArgSpace));
}


/* Call RUN_ALL_TESTS() in main().

 We do this by linking in src/gtest_main.cc file, which consists of
 a main() function which calls RUN_ALL_TESTS() for us.

 This runs all the tests you've defined, prints the result, and
 returns 0 if successful, or 1 otherwise.

 Notice that we didn't register the tests.  The
 RUN_ALL_TESTS() macro magically knows about all the tests we
 defined.
*/
