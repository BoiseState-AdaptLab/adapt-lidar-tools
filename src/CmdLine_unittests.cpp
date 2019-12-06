/*
* File name: CmdLine_unittests.cpp
* Created on: 24-May-2017
* Author: ravi
*/

#include "CmdLine.hpp"
#include "gtest/gtest.h"
#include <getopt.h>
#include <iostream>
#include <fstream>


class CmdLineTest : public testing::Test {
    protected:

        virtual void SetUp(){
            numberOfArgs = 10;
            maxLengthOfArg = 256;
            commonArgSpace = allocateTestArgs(numberOfArgs,maxLengthOfArg);
            //Set required arguments
            strncpy(commonArgSpace[0],"test",5);
            strncpy(commonArgSpace[1],"-f",3);
            strncpy(commonArgSpace[2],"do_not_use.pls",15);
            strncpy(commonArgSpace[3],"-e",3);
            strncpy(commonArgSpace[4],"1",2);
            //Stifle command line error messages
            cmd.quiet = true;
            //Create test files
            std::ofstream pls ("do_not_use.pls");
            pls.close();
            std::ofstream wvs ("do_not_use.wvs");
            wvs.close();
        }

        static char** allocateTestArgs(int N,int M){
            char** someArgs = (char**)malloc(sizeof(char*)*N);
            for(int i=0;i<10;i++){
                someArgs[i] = (char*)malloc(sizeof(char)*M);
            }
            return someArgs;
        }

        void TearDown()  {
            for (int i=0 ;i<10;i++){
                free(commonArgSpace[i]);
            }
            free(commonArgSpace);
            //Delete test files
            std::remove("do_not_use.pls");
            std::remove("do_not_use.wvs");
        }

        char** commonArgSpace;
        int numberOfArgs;
        int maxLengthOfArg;
        CmdLine cmd;
        CmdLine cmd2;
        CmdLine cmd3;

};

//Tests no inputted arguments
TEST_F(CmdLineTest, noArguments){
    optind = 0;
    numberOfArgs = 1;
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);
}

/****************************************************************************
 *
 * File Name Tests
 *
 ****************************************************************************/
// Tests file was correctly set
TEST_F(CmdLineTest, validFileName){
    //This also tests file trimming for files in the same path
    optind = 0;
    numberOfArgs = 5;
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_FALSE(cmd.printUsageMessage);
    EXPECT_EQ("do_not_use.pls", cmd.getInputFileName(true));
    EXPECT_EQ("do_not_use.wvs", cmd.getInputFileName(false));
}

//Tests file was incorrectly set
TEST_F(CmdLineTest, invalidFileName){
    //Delete pls file
    std::remove("do_not_use.pls");

    //Invalid pls
    optind = 0;
    numberOfArgs = 5;
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);

    //Delete wvs file and create pls file
    std::remove("do_not_use.wvs");
    std::ofstream pls ("do_not_use.pls");
    pls.close();
    
    //Invalid wvs
    optind = 0;
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);

    //No file
    optind = 0;
    numberOfArgs = 4;
    strncpy(commonArgSpace[2],"-e",3);
    strncpy(commonArgSpace[3],"1",2);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);
}

//Tests file was correctly trimmed with various paths
TEST_F(CmdLineTest, fileTrimmingTestPath){
    numberOfArgs = 5;

    //Same path, child path, parent path
    std::vector<std::string> paths = {"./", "etc/", "../"};
    for (auto it = paths.begin(); it != paths.end(); ++it){
        //Create test files
        std::ofstream pls2 (*it + std::string("do_not_use.pls"));
        pls2.close();
        std::ofstream wvs2 (*it + std::string("do_not_use.wvs"));
        wvs2.close();

        optind = 0;
        strncpy (commonArgSpace[2],
            (*it + std::string("do_not_use.pls")).c_str(),15 + it->length());
        ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
        ASSERT_FALSE(cmd.printUsageMessage);
        EXPECT_EQ("do_not_use", cmd.getTrimmedFileName(true));
        EXPECT_EQ("do_not_use", cmd.getTrimmedFileName(false));
  
        //Delete test files
        std::remove((*it + std::string("do_not_use.pls")).c_str());
        std::remove((*it + std::string("do_not_use.wvs")).c_str());
    }
}

//Tests file was correctly trimmed with odd file names
TEST_F(CmdLineTest, fileTrimmingTestName){
    //Create test files
    std::ofstream pls2 ("do.not.use.pls");
    pls2.close();
    std::ofstream wvs2 ("do.not.use.wvs");
    wvs2.close();

    optind = 0;
    numberOfArgs = 5;
    strncpy (commonArgSpace[2],"do.not.use.pls",15);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_FALSE(cmd.printUsageMessage);
    EXPECT_EQ("do.not.use", cmd.getTrimmedFileName(true));
    EXPECT_EQ("do.not.use", cmd.getTrimmedFileName(false));

    //Delete test files
    std::remove("do.not.use.pls");
    std::remove("do.not.use.wvs");
}

/****************************************************************************
 *
 * Product Option Tests
 *
 ****************************************************************************/
//Tests valid product variable char
TEST_F(CmdLineTest, validProductOption){
    //Single argument options
    numberOfArgs = 5;
    std::vector<std::string> vars = {"-e", "-a", "-w", "-r"};
    for (auto it = vars.begin(); it != vars.end(); ++it){
        optind = 0;
        strncpy(commonArgSpace[3],(*it).c_str(),3);
        ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
        ASSERT_FALSE(cmd.printUsageMessage);
    }

    //Multiple argument options
    optind = 0;
    numberOfArgs = 6;
    strncpy(commonArgSpace[3],"-b",3);
    strncpy(commonArgSpace[5], "6", 3);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_FALSE(cmd.printUsageMessage);
    EXPECT_EQ(6, cmd.calibration_constant);
}

//Tests invalid product variable char
TEST_F(CmdLineTest, invalidProductOption){
    //Invalid character
    optind = 0;
    numberOfArgs = 5;
    strncpy(commonArgSpace[3],"-g",3);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);

    //Not enough arguments
    optind = 0;
    strncpy(commonArgSpace[3],"-b",3);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);
    EXPECT_EQ(0,cmd.calibration_constant);
}

//Tests for an invalid product number
TEST_F(CmdLineTest, invalidProductNumber){
    //No product number
    optind = 0;
    numberOfArgs = 4;
    strncpy(commonArgSpace[4],"",0);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);

    //Invalid product number
    optind = 0;
    numberOfArgs = 5;
    strncpy(commonArgSpace[4],"19",3);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);

    optind = 0;
    strncpy(commonArgSpace[4],"-1",3);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);

    optind = 0;
    strncpy(commonArgSpace[4],"3000000000000",3);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);
}

//Tests requesting all products for a single variable
TEST_F(CmdLineTest, allProductNums){
    //All product numbers for elevation
    optind = 0;
    numberOfArgs = 5;
    strncpy(commonArgSpace[4],"0",2);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_FALSE(cmd.printUsageMessage);
    ASSERT_EQ(cmd.selected_products.size(), 18);
    for (size_t i = 0; i < 18; i++){
        EXPECT_EQ(cmd.selected_products.at(i),i+1);
    }

    //All product numbers for backscatter coefficient
    optind = 0;
    numberOfArgs = 6;
    strncpy(commonArgSpace[3],"-b",3);
    strncpy(commonArgSpace[4],"0",2);
    strncpy(commonArgSpace[5],"9e-8",5);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_FALSE(cmd.printUsageMessage);
    ASSERT_EQ(cmd.selected_products.size(), 18);
    for (size_t i = 0; i < 18; i++){
        EXPECT_EQ(cmd.selected_products.at(i),i+73);
    }
}

//Tests requesting ALL products
TEST_F(CmdLineTest, allProducts){
    optind = 0;
    numberOfArgs = 5;
    strncpy(commonArgSpace[3],"--all",6);
    strncpy(commonArgSpace[4],"9e-8",5);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_FALSE(cmd.printUsageMessage);
    ASSERT_EQ(cmd.selected_products.size(), 102);
    for (size_t i = 0; i < 102; i++){
        EXPECT_EQ(cmd.selected_products.at(i),i+1);
    }
}

/****************************************************************************
 *
 * Non-Product Option Tests
 *
 ****************************************************************************/
//Tests for a valid non-product option
TEST_F(CmdLineTest, validNonProductOption){
    //Shouldn't trigger usage message
    optind = 0;
    numberOfArgs = 6;
    strncpy(commonArgSpace[5],"-d",3);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_FALSE(cmd.printUsageMessage);
    ASSERT_FALSE(cmd.useGaussianFitting);

    //Should trigger usage message
    optind = 0;
    strncpy(commonArgSpace[5],"-h",3);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);
}

//Tests for an invalid non-product option
TEST_F(CmdLineTest, invalidNonProductOption){
    optind = 0;
    numberOfArgs = 6;
    strncpy(commonArgSpace[5],"-p",3);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);
}

//Tests for a float argument that is out of range.
TEST_F(CmdLineTest, floatOutOfRangeTest){
    optind = 0;
    numberOfArgs = 7;
    strncpy(commonArgSpace[5],"-m",3);
    strncpy(commonArgSpace[6],"1.0e+39",8); // To large for floating point.
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);

    cmd.printUsageMessage = false;
    // Do the same for the calibration constant option to make sure it works too
    optind = 0;
    numberOfArgs = 8;
    strncpy(commonArgSpace[5],"-b",3);
    strncpy(commonArgSpace[6],"1,2",4);
    strncpy(commonArgSpace[7],"1.0e+309",8); // This one is a double, not a fl.
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);

    cmd.printUsageMessage = false;
    //And do the same for all as well
    optind = 0;
    numberOfArgs = 8;
    strncpy(commonArgSpace[5],"bruh",3);
    strncpy(commonArgSpace[6],"1.0e+309",8);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);
}
/****************************************************************************
 *
 * Long Option Tests
 *
 ****************************************************************************/
//Tests valid long file option
TEST_F(CmdLineTest, validLongFileOption){
    //--file test
    optind = 0;
    numberOfArgs = 5;
    strncpy(commonArgSpace[1],"--file",7);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_FALSE(cmd.printUsageMessage);
}

//Tests valid long product options
TEST_F(CmdLineTest, validLongProductOption){
    //long product options
    //single argument
    numberOfArgs = 5;
    std::vector<std::string> opts = {"--elevation", "--width", "--amplitude",
        "--risetime"};
    for (auto it = opts.begin(); it != opts.end(); ++it){
        optind = 0;
        strncpy(commonArgSpace[3],(*it).c_str(),it->length() + 1);
        ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
        ASSERT_FALSE(cmd.printUsageMessage);
    }

    //multiple arguments
    optind = 0;
    numberOfArgs = 6;
    strncpy(commonArgSpace[3],"--backscatter",14);
    strncpy(commonArgSpace[5],"1.5E2",6);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_FALSE(cmd.printUsageMessage);
    EXPECT_EQ(150,cmd.calibration_constant);
}

//Tests valid lon non-product options
TEST_F(CmdLineTest, validLongNonProductOption){
    //Shoudln't trigger usage message
    optind = 0;
    numberOfArgs = 6;
    strncpy(commonArgSpace[5],"--firstdiff",12);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_FALSE(cmd.printUsageMessage);
    ASSERT_FALSE(cmd.useGaussianFitting);

    //Should trigger usage message
    optind = 0;
    strncpy(commonArgSpace[5],"--help",7);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);
}

//Tests missing arguments for long options
TEST_F(CmdLineTest, validLongOptionMissingArgument){
    //multipl arguments product option
    optind = 0;
    numberOfArgs = 5;
    strncpy(commonArgSpace[3],"--backscatter",14);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);

    //single argument product option
    //invalid product number
    optind = 0;
    strncpy(commonArgSpace[4],"100",4);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);

    //missing product number
    optind = 0;
    numberOfArgs = 4;
    strncpy(commonArgSpace[3],"--elevation",12);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);

    //missing file
    optind = 0;
    strncpy(commonArgSpace[1],"--file",8);
    strncpy(commonArgSpace[2],"-e",3);
    strncpy(commonArgSpace[3],"1",2);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);
}

//Tests invalid long option
TEST_F(CmdLineTest, invalidLongOption){
    optind = 0;
    numberOfArgs = 6;
    strncpy(commonArgSpace[5],"--oops",7);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_TRUE(cmd.printUsageMessage);
}

/****************************************************************************
 *
 * Output filename tests
 *
 ****************************************************************************/
//Tests correct naming of method
TEST_F(CmdLineTest, outputFileNameMethod){
    optind = 0;
    numberOfArgs = 5;
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_FALSE(cmd.printUsageMessage);
    EXPECT_EQ("do_not_use_max_first_elev_gaussian.tif",
            cmd.get_output_filename(1));

    optind = 0;
    numberOfArgs = 6;
    strncpy(commonArgSpace[5],"-d",3);
    ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
    ASSERT_FALSE(cmd.printUsageMessage);
    EXPECT_EQ("do_not_use_max_first_elev_firstDiff.tif",
            cmd.get_output_filename(1));
}

//Tests correct naming of variable
TEST_F(CmdLineTest, outputFileNameVariable){
    //Set calibration coefficient for backscatter test
    strncpy(commonArgSpace[5],"645",4);

    std::vector<std::string> opts = {"-e", "-a", "-w", "-r", "-b"};
    std::vector<std::string> names = {"elev", "amp", "width", "riseTime",
        "backscatter"};
    for (size_t i = 0; i < names.size(); i ++){
        optind = 0;
        numberOfArgs = opts.at(i) == "-b" ? 6 : 5;
        strncpy(commonArgSpace[3],opts.at(i).c_str(),3);
        std::string expectedName = std::string("do_not_use_max_first_")
            + names.at(i) + std::string("_gaussian.tif");
        ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
        ASSERT_FALSE(cmd.printUsageMessage);
        EXPECT_EQ(expectedName.c_str(),cmd.get_output_filename(
            cmd.selected_products.at(0)));
    }
}

//Tests correct naming of used peaks
TEST_F(CmdLineTest, outputFileNamePeaks){
    std::vector<std::string> nums = {"1", "7", "13"};
    std::vector<std::string> names = {"first", "last", "all"};
    for (int i = 0; i < 3; i ++){
        optind = 0;
        numberOfArgs = 5;
        strncpy(commonArgSpace[4],nums.at(i).c_str(),nums.at(i).length());
        std::string expectedName = std::string("do_not_use_max_")
            + names.at(i) + std::string("_elev_gaussian.tif");
        ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
        ASSERT_FALSE(cmd.printUsageMessage);
        EXPECT_EQ(expectedName.c_str(),cmd.get_output_filename(
            cmd.selected_products.at(0)));
    }
}

//Tests correct naming of calculation
TEST_F(CmdLineTest, outputFileNameCalculation){
    std::vector<std::string> names = {"max", "min", "mean", "stdev", "skew",
        "kurt"};
    for (int i = 0; i < 6; i ++){
        optind = 0;
        numberOfArgs = 5;
        strncpy(commonArgSpace[4],std::to_string(i+1).c_str(),2);
        std::string expectedName = std::string("do_not_use_")
            + names.at(i) + std::string("_first_elev_gaussian.tif");
        ASSERT_NO_THROW(cmd.parse_args(numberOfArgs,commonArgSpace));
        ASSERT_FALSE(cmd.printUsageMessage);
        EXPECT_EQ(expectedName.c_str(),cmd.get_output_filename(
            cmd.selected_products.at(0)));
    }
}

/* Call RUN_ALL_TESTS() in main().

   We do this by linking in src/gtest_main.cc file, which consists of
   a main() function which calls RUN_ALL_TESTS() for us.

   This runs all the tests defined, prints the result, and
   returns 0 if successful, or 1 otherwise.

*/
