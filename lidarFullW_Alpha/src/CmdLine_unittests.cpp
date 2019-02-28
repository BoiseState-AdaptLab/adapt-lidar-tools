/*
 * File name: CmdLine_unittests.cpp
 * Created on: 24-May-2017
 * Author: ravi
 */

#include "CmdLine.hpp"
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
  CmdLine cmd;
  CmdLine cmd2;
  CmdLine cmd3;

};


/****************************************************************************
*
* Begin valid command line option tests
*
****************************************************************************/

// Tests valid short command line options
TEST_F(CmdLineTest, validShortCmdLineOpts) {
  optind = 0; // Need to reset optind to 0 for each test
  numberOfArgs = 2;
  strncpy(commonArgSpace[0],"test",5);
  strncpy(commonArgSpace[1],"-h",3);
  ASSERT_NO_THROW({
    cmd.parse(numberOfArgs, commonArgSpace);
  });
  ASSERT_TRUE(cmd.printUsageMessage);

  optind = 0; // Need to reset optind to 0 for each test
  numberOfArgs = 3;
  strncpy(commonArgSpace[1],"-f",3);
  strncpy(commonArgSpace[2],"someFileName",13);
  ASSERT_NO_THROW({
    cmd2.parse(numberOfArgs, commonArgSpace);
  });
  ASSERT_FALSE(cmd2.printUsageMessage);
  ASSERT_EQ("someFileName",cmd2.getInputFileName());



}


TEST_F(CmdLineTest, fileNameNotFound){
  optind = 0; // Need to reset optind to 0 for each test
  numberOfArgs = 3;
  strncpy(commonArgSpace[1],"-f",3);
  strncpy(commonArgSpace[2],"someFileName",13);
  ASSERT_NO_THROW({
                    cmd2.parse_args(numberOfArgs, commonArgSpace);
                  });
  ASSERT_TRUE(cmd2.printUsageMessage);
  ASSERT_EQ("someFileName",cmd2.getInputFileName());
}

// Tests valid long command line options
TEST_F(CmdLineTest, validLongCmdLineOpts) {
  optind = 0; // Need to reset optind to 0 for each test
  numberOfArgs = 2;
  strncpy( commonArgSpace[0],"test",5);
  strncpy(commonArgSpace[1],"--help",7);
  ASSERT_NO_THROW({
    cmd.parse(numberOfArgs, commonArgSpace);
  });
  ASSERT_TRUE(cmd.printUsageMessage);

  optind = 0; // Need to reset optind to 0 for each test
  numberOfArgs = 3;
  strncpy(commonArgSpace[1],"--file",7);
  strncpy(commonArgSpace[2],"file",5);
  ASSERT_NO_THROW({
    cmd2.parse(numberOfArgs, commonArgSpace);
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
  optind = 0; // Need to reset optind to 0 for each test
  numberOfArgs = 1;
  strncpy(commonArgSpace[0],"test",5);
  EXPECT_NO_THROW(cmd.parse(numberOfArgs, commonArgSpace));
}

// Tests missing short option arguments
TEST_F(CmdLineTest, missingShortOptArg) {
  optind = 0; // Need to reset optind to 0 for each test
  numberOfArgs = 2;
  strncpy(commonArgSpace[0],"test",5);
  strncpy(commonArgSpace[1],"-f",3);
  EXPECT_NO_THROW(cmd.parse(numberOfArgs, commonArgSpace));
}

TEST_F(CmdLineTest, unsupportedShortArg) {
    optind = 0; // Need to reset optind to 0 for each test
    numberOfArgs = 2;
    strncpy(commonArgSpace[1], "-g", 3);
    ASSERT_NO_THROW({
                        cmd3.parse(numberOfArgs, commonArgSpace);
                    });
    ASSERT_TRUE(cmd3.printUsageMessage);
}

TEST_F(CmdLineTest, unsupportedLongArg) {
    optind = 0; // Need to reset optind to 0 for each test
    numberOfArgs = 2;
    strncpy(commonArgSpace[1], "--guess", 8);
    ASSERT_NO_THROW({
                        cmd3.parse(numberOfArgs, commonArgSpace);
                    });
    ASSERT_TRUE(cmd3.printUsageMessage);
}

// Tests missing long option arguments
TEST_F(CmdLineTest, missingLongOptArg) {
  optind = 0; // Need to reset optind to 0 for each test
  numberOfArgs = 2;
  strncpy(commonArgSpace[0],"test",5);
  strncpy(commonArgSpace[1],"--file",7);
  EXPECT_NO_THROW(cmd.parse(numberOfArgs, commonArgSpace));
}

// Tests invalid short command line options
TEST_F(CmdLineTest, invalidShortCmdLineOpts) {
  optind = 0; // Need to reset optind to 0 for each test
  numberOfArgs = 2;
  strncpy( commonArgSpace[0],"test",5);
  strncpy(commonArgSpace[1],"-s",3);
  EXPECT_NO_THROW(cmd.parse(numberOfArgs, commonArgSpace));
  ASSERT_TRUE(cmd.printUsageMessage);
}

// Tests invalid long command line options
TEST_F(CmdLineTest, invalidLongCmdLineOpts) {
  optind = 0; // Need to reset optind to 0 for each test
  numberOfArgs = 2;
  strncpy( commonArgSpace[0],"test",5);
  strncpy(commonArgSpace[1],"--who",6);
  EXPECT_NO_THROW(cmd.parse(numberOfArgs, commonArgSpace));
}

/****************************************************************************
*
* Begin filename trimming tests
*
****************************************************************************/

// Tests for input file names beginning in the root path
TEST_F(CmdLineTest, rootBasedFilePathTrim) {
    optind = 0; // Need to reset optind to 0 for each test
    numberOfArgs = 3;
    strncpy( commonArgSpace[0],"test",5);
    strncpy(commonArgSpace[1],"-f",6);
    strncpy(commonArgSpace[2],"/etc/test/inputfile1.pls",30);
    EXPECT_NO_THROW(cmd2.parse(numberOfArgs, commonArgSpace));
    ASSERT_FALSE(cmd2.printUsageMessage);
    ASSERT_EQ("inputfile1",cmd2.getTrimmedFileName());
    ASSERT_EQ("/etc/test/inputfile1.pls",cmd2.getInputFileName());
}

// Tests for input file names beginning in sibling path
TEST_F(CmdLineTest, siblingBasedFilePathTrim) {
    optind = 0; // Need to reset optind to 0 for each test
    numberOfArgs = 3;
    strncpy( commonArgSpace[0],"test",5);
    strncpy(commonArgSpace[1],"-f",6);
    strncpy(commonArgSpace[2],"../etc/test/inputfile1.pls",30);
    EXPECT_NO_THROW(cmd2.parse(numberOfArgs, commonArgSpace));
    ASSERT_FALSE(cmd2.printUsageMessage);
    ASSERT_EQ("inputfile1",cmd2.getTrimmedFileName());
    ASSERT_EQ("../etc/test/inputfile1.pls",cmd2.getInputFileName());
}

// Tests for input file names beginning in child path
TEST_F(CmdLineTest, childBasedFilePathTrim) {
    optind = 0; // Need to reset optind to 0 for each test
    numberOfArgs = 3;
    strncpy( commonArgSpace[0],"test",5);
    strncpy(commonArgSpace[1],"-f",6);
    strncpy(commonArgSpace[2],"etc/test/inputfile1.pls",30);
    EXPECT_NO_THROW(cmd2.parse(numberOfArgs, commonArgSpace));
    ASSERT_FALSE(cmd2.printUsageMessage);
    ASSERT_EQ("inputfile1",cmd2.getTrimmedFileName());
    ASSERT_EQ("etc/test/inputfile1.pls",cmd2.getInputFileName());
}

// Tests for input file names beginning in same path
TEST_F(CmdLineTest, selfBasedFilePathTrim) {
    optind = 0; // Need to reset optind to 0 for each test
    numberOfArgs = 3;
    strncpy( commonArgSpace[0],"test",5);
    strncpy(commonArgSpace[1],"-f",6);
    strncpy(commonArgSpace[2],"inputfile1.pls",30);
    EXPECT_NO_THROW(cmd2.parse(numberOfArgs, commonArgSpace));
    ASSERT_FALSE(cmd2.printUsageMessage);
    ASSERT_EQ("inputfile1",cmd2.getTrimmedFileName());
    ASSERT_EQ("inputfile1.pls",cmd2.getInputFileName());
}

// Tests for input file names beginning in sibling path and filename contains dots
TEST_F(CmdLineTest, siblingBasedFilePathTrimDotFileName) {
    optind = 0; // Need to reset optind to 0 for each test
    numberOfArgs = 3;
    strncpy( commonArgSpace[0],"test",5);
    strncpy(commonArgSpace[1],"-f",6);
    strncpy(commonArgSpace[2],"../etc/test/input.file.1.pls",30);
    EXPECT_NO_THROW(cmd2.parse(numberOfArgs, commonArgSpace));
    ASSERT_FALSE(cmd2.printUsageMessage);
    ASSERT_EQ("input.file.1",cmd2.getTrimmedFileName());
    ASSERT_EQ("../etc/test/input.file.1.pls",cmd2.getInputFileName());
}


/****************************************************************************
*
* Begin output filename tests
*
****************************************************************************/

TEST_F(CmdLineTest, output_filename_gaussian){
    optind = 0; // Need to reset optind to 0 for each test
    numberOfArgs = 3;
    strncpy( commonArgSpace[0],"test",5);
    strncpy(commonArgSpace[1],"-f",6);
    strncpy(commonArgSpace[2],"inputfile1.pls",15);
    EXPECT_NO_THROW(cmd.parse(numberOfArgs, commonArgSpace));
    ASSERT_EQ("inputfile1_gaussian.tif",cmd.get_output_filename());
}

TEST_F(CmdLineTest, output_filename_first_diff){
    optind = 0; // Need to reset optind to 0 for each test
    numberOfArgs = 4;
    strncpy( commonArgSpace[0],"test",5);
    strncpy(commonArgSpace[1],"-f",5);
    strncpy(commonArgSpace[2],"inputfile1.pls",15);
    strncpy(commonArgSpace[3],"-d",5);
    EXPECT_NO_THROW(cmd.parse(numberOfArgs, commonArgSpace));
    ASSERT_EQ("inputfile1_firstDiff.tif",cmd.get_output_filename());
}

/* Call RUN_ALL_TESTS() in main().

 We do this by linking in src/gtest_main.cc file, which consists of
 a main() function which calls RUN_ALL_TESTS() for us.

 This runs all the tests defined, prints the result, and
 returns 0 if successful, or 1 otherwise.

*/
