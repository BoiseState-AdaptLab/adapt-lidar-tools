/*
 * File name: testDriver.cpp
 * Created on: 24-May-2017
 * Author: ravi
 */


#include "cmdLine.hpp"
#include "gtest/gtest.h"


// Step 2. Use the TEST macro to define tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.
//
// <TechnicalDetails>
//
// In Google Test, tests are grouped into test cases.  This is how we
// keep test code organized.  You should put logically related tests
// into the same test case.
//
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.
//
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.
//
// </TechnicalDetails>


// Tests parseCmdLineArgs().

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
    std::cerr << "FAILURE: Malloc failed for testing" << 
      std::endl;
    return 1;
  }

// Tests valid short command line options.
TEST(CmdLineArgsTest, ValidShort) {
  // This test is named "ValidShort", and belongs to the "CmdLineArgsTest"
  // test case.

  noOfArgs = 2;
  strncpy( someArgs[0],"test",4);
   
  ASSERT_NO_THROW({
    strncpy(someArgs[1],"-h",2);
    parseCmdLineArgs(noOfArgs, someArgs);
  });

  ASSERT_NO_THROW({
    noOfArgs = 3;
    strncpy(someArgs[1],"-f",2);
    strncpy(someArgs[2],"file",4);
    parseCmdLineArgs(noOfArgs, someArgs);
  });

  // <TechnicalDetails>
  //
  // EXPECT_EQ(expected, actual) is the same as
  //
  //   EXPECT_TRUE((expected) == (actual))
  //
  // except that it will print both the expected value and the actual
  // value when the assertion fails.  This is very helpful for
  // debugging.  Therefore in this case EXPECT_EQ is preferred.
  //
  // On the other hand, EXPECT_TRUE accepts any Boolean expression,
  // and is thus more general.
  //
  // </TechnicalDetails>
}

// Tests valid long command line options
TEST(CmdLineArgsTest, ValidLong) {
  noOfArgs = 2;
  strncpy( someArgs[0],"test",4);
   
  ASSERT_NO_THROW({
    strncpy(someArgs[1],"--help",6);
    parseCmdLineArgs(noOfArgs, someArgs);
  });

  ASSERT_NO_THROW({
    noOfArgs = 3;
    strncpy(someArgs[1],"--file",6);
    strncpy(someArgs[2],"file",4);
    parseCmdLineArgs(noOfArgs, someArgs);
  });
}

// Tests missing option argument(short and long options)
TEST(CmdLineArgsTest, ValidLong) {
  noOfArgs = 2;
  strncpy( someArgs[0],"test",4);
  strncpy(someArgs[1],"-f",2);
  
  ASSERT_THROW(parseCmdLineArgs(noOfArgs, someArgs), missingArgException);

  strncpy(someArgs[1],"--file",6);
  ASSERT_THROW(parseCmdLineArgs(noOfArgs, someArgs), missingArgException);
}

// Tests invalid short command line options
TEST(CmdLineArgsTest, InvalidShort) {

  noOfArgs = 2;
  strncpy(someArgs[0],"test",4);
  strncpy(someArgs[1],"-e",2);

  ASSERT_THROW(parseCmdLineArgs(noOfArgs, someArgs), invalidOptionException);

  strncpy(someArgs[1],"-g",2);
  ASSERT_THROW(parseCmdLineArgs(noOfArgs, someArgs), invalidOptionException);
}


// Tests invalid long command line options
TEST(CmdLineArgsTest, InvalidLong) {

  noOfArgs = 2;
  strncpy( someArgs[0],"test",4);
  strncpy(someArgs[1],"--selp",6);
  
  ASSERT_THROW(parseCmdLineArgs(noOfArgs, someArgs), invalidOptionException);

  noOfArgs = 3;
  strncpy(someArgs[1],"-bile",2);
  strncpy(someArgs[2],"file",4);
  ASSERT_THROW(parseCmdLineArgs(noOfArgs, someArgs), invalidOptionException);
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
