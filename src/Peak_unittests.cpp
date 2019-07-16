/*
 * File Name: Peak_unittests.cpp
 * Created On: 11 June 2019
 * Author: Aaron Orenstein
*/

#include "Peak.hpp"
#include "gtest/gtest.h"

class PeakTest : public testing::Test{
    protected:
        virtual void SetUp(){

            //peak0: backscatter coefficient tests
            //Set activation point coordinates
            peak0->x_activation = 3;
            peak0->y_activation = 4;
            peak0->z_activation = 12;
            //Set amplitude and fwhm
            peak0->amp = 10;
            peak0->fwhm = 5;
            //----------------------------------------------------

            //peak1: toString tests
            peak1->amp = expected_double;
            peak1->location = expected_double;
            peak1->fwhm = expected_double;

            peak1->is_final_peak = expected_bool;
            peak1->position_in_wave = expected_int;

            peak1->triggering_amp = expected_int;
            peak1->triggering_location = expected_int;

            peak1->x_activation = expected_double;
            peak1->y_activation = expected_double;
            peak1->z_activation = expected_double;

            peak1->x = expected_double;
            peak1->y = expected_double;
            peak1->z = expected_double;

            peak1->rise_time = expected_double;

            peak1->backscatter_coefficient = expected_double;

            //peak1->num samples
            //----------------------------------------------------
        }

        virtual void TearDown(){
            delete(peak0);
            delete(peak1);
        }

        Peak* peak0 = new Peak();
        Peak* peak1 = new Peak();

        double expected_double = 1.234567;
        int expected_int = 1234567;
        bool expected_bool = true;

        testing::AssertionResult stringsMatch(std::string expected,
                                              std::string actual) {
            if (expected == actual) {
                return testing::AssertionSuccess();
            } else {
                return testing::AssertionFailure() << "Expected: " <<
                    expected << "\ngot: " << actual;
            }
        }
};

//-------------------------------------------------------------------------
//Peak0 tests / calcBackscatter tests
//-------------------------------------------------------------------------

//Tests backscatter coefficient calculation
TEST_F(PeakTest, backscatterTest){
    EXPECT_NO_THROW(peak0->calcBackscatter(10, 5, 1e-1, 0, 0, 0));
    EXPECT_NEAR(119.9607135,peak0->backscatter_coefficient,.1);
}

//Tests infinite backscatter coefficient
TEST_F(PeakTest, backscatterDivZeroTest){
    EXPECT_NO_THROW(peak0->calcBackscatter(0, 5, 1e-1, 0, 0, 0));
    EXPECT_EQ(INFINITY,peak0->backscatter_coefficient);
}

//-------------------------------------------------------------------------
//Peak1 tests / toString tests
//-------------------------------------------------------------------------

//Test that toString can produce a single valid double
TEST_F(PeakTest, toString_valid_singleDouble) {
    std::vector<int> varlist_valid_singleDouble = {1};
    std::string expect_valid_singleDouble = "1.234567";

    std::string str;
    EXPECT_NO_THROW(peak1->toString(str, varlist_valid_singleDouble));
    EXPECT_TRUE(stringsMatch(expect_valid_singleDouble, str));
}

//Test that toString can produce a single valid int
TEST_F(PeakTest, toString_valid_singleInt) {
    std::vector<int> varlist_valid_singleInt = {7};
    std::string expect_valid_singleInt = "1234567";

    std::string str;
    EXPECT_NO_THROW(peak1->toString(str, varlist_valid_singleInt));
    EXPECT_TRUE(stringsMatch(expect_valid_singleInt, str));
}

//Test that toString can produce a single valid bool
TEST_F(PeakTest, toString_valid_singleBool) {
    std::vector<int> varlist_valid_singleBool = {4};
    std::string expect_valid_singleBool = "True";

    std::string str;
    EXPECT_NO_THROW(peak1->toString(str, varlist_valid_singleBool));
    EXPECT_TRUE(stringsMatch(expect_valid_singleBool, str));
}

//Test that toString can produce all valid inputs
TEST_F(PeakTest, toString_valid_all) {
    std::vector<int> varlist_valid_all = {1,2,3,4,5,6,7,8,9,10};
    std::string expect_valid_all =
        "1.234567, 1.234567, 1.234567, True, 1234567, 1234567, 1234567, "
        "1.234567, 1.234567, 1.234567, 1.234567, 1.234567, 1.234567, "
        "(Samples not yet supported)";

    std::string str;
    EXPECT_NO_THROW(peak1->toString(str, varlist_valid_all));
    EXPECT_TRUE(stringsMatch(expect_valid_all, str));
}

//Test that toString can handle a single invalid input
TEST_F(PeakTest, toString_invalid_single) {
    std::vector<int> varlist_invalid_single = {20};
    std::string expect_invalid_single = "(Invalid arg)";

    std::string str;
    EXPECT_NO_THROW(peak1->toString(str, varlist_invalid_single));
    EXPECT_TRUE(stringsMatch(expect_invalid_single, str));
}

//Test that toString can handle multiple invalid inputs
TEST_F(PeakTest, toString_invalid_multiple) {
    std::vector<int> varlist_invalid_multiple = {20,21,22};
    std::string expect_invalid_multiple =
        "(Invalid arg), (Invalid arg), (Invalid arg)";

    std::string str;
    EXPECT_NO_THROW(peak1->toString(str, varlist_invalid_multiple));
    EXPECT_TRUE(stringsMatch(expect_invalid_multiple, str));
}

//Test that toString can handle mixed valid and invalid inputs
TEST_F(PeakTest, toString_mixed) {
    std::vector<int> varlist_mixed = {1,20,2};
    std::string expect_mixed = "1.234567, (Invalid arg), 1.234567";

    std::string str;
    EXPECT_NO_THROW(peak1->toString(str, varlist_mixed));
    EXPECT_TRUE(stringsMatch(expect_mixed, str));
}

//Test that toString can handle 0 args
TEST_F(PeakTest, toString_empty) {
    std::vector<int> varlist_empty = {};
    std::string expect_empty = "";

    std::string str;
    EXPECT_NO_THROW(peak1->toString(str, varlist_empty));
    EXPECT_TRUE(stringsMatch(expect_empty, str));
}
