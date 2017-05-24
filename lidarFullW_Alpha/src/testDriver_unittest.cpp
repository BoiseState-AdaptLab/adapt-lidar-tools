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

// Tests valid short command line arguments.
TEST(CmdLineArgsTest, ValidShort) {
  // This test is named "Valid", and belongs to the "CmdLineArgsTest"
  // test case.
  EXPECT_EQ(1, parseCmdLineArgs(1, "-h"));
  EXPECT_EQ(1, parseCmdLineArgs(2, "-f ../src/test"));

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

// Tests valid long command line arguments
TEST(CmdLineArgsTest, ValidLong) {
  EXPECT_EQ(1, parseCmdLineArgs(1, "--help"));
  EXPECT_EQ(1, parseCmdLineArgs(1, "--file ../src/test"));
}

// Tests invalid short command line arguments
TEST(CmdLineArgsTest, InvalidShort) {
  EXPECT_EQ(1, parseCmdLineArgs(1, "-e"));
  EXPECT_EQ(1, parseCmdLineArgs(1, "-g"));
  EXPECT_EQ(1, parseCmdLineArgs(2, "-o, test"));
  EXPECT_EQ(1, parseCmdLineArgs(2, "-i, someFile"));
}


// Tests invalid long command line arguments
TEST(CmdLineArgsTest, InvalidLong) {
  EXPECT_EQ(1, parseCmdLineArgs(1, "--help"));
  EXPECT_EQ(1, parseCmdLineArgs(1, "--file ../src/test"));
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
