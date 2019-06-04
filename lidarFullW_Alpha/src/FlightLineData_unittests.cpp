// File name: FlightLineData_unittests.cpp
// Created on: 09- November-2017
// Author: ravi

#include "gtest/gtest.h"
#include "FlightLineData.hpp"
#include "PulseData.hpp"

class FlightLineDataTest: public testing::Test{
    protected:

        // function to set up space used by all tests
        virtual void SetUp(){
        }

};


/****************************************************************************
 *
 * Calling the setFlightLineData() function test
 *
 ****************************************************************************/
TEST_F(FlightLineDataTest, testFunctionStart){

    FlightLineData fld;

    std::string incorrect_file_name1 = "bin/140823_183115_1_clipped_test.pls";
    EXPECT_ANY_THROW (fld.setFlightLineData(incorrect_file_name1));

    std::string incorrect_file_name2 = "etc/140823_183115_1_clipped_test.dat";
    EXPECT_ANY_THROW (fld.setFlightLineData(incorrect_file_name2));

    std::string correct_file_name =  "etc/140823_183115_1_clipped_test.pls";
    EXPECT_NO_THROW (fld.setFlightLineData(correct_file_name));
}


/****************************************************************************
 *
 * Test the boundary mins and maxes
 *
 ****************************************************************************/
TEST_F(FlightLineDataTest, testBoundaryMinsAndMaxes){

    FlightLineData fld;

    std::string file_name =  "etc/140823_183115_1_clipped_test.pls";
    EXPECT_NO_THROW (fld.setFlightLineData(file_name));

    double known_bb_x_min = 516209.586;
    double known_bb_y_min = 4767921.375;
    double known_bb_z_min = 2084.585;
    double known_bb_x_max = 516211.942;
    double known_bb_y_max = 4767923.621;
    double known_bb_z_max = 2093.581;

    EXPECT_DOUBLE_EQ (known_bb_x_min, fld.bb_x_min);
    EXPECT_DOUBLE_EQ (known_bb_y_min, fld.bb_y_min);
    EXPECT_DOUBLE_EQ (known_bb_z_min, fld.bb_z_min);
    EXPECT_DOUBLE_EQ (known_bb_x_max, fld.bb_x_max);
    EXPECT_DOUBLE_EQ (known_bb_y_max, fld.bb_y_max);
    EXPECT_DOUBLE_EQ (known_bb_z_max, fld.bb_z_max);


}


/****************************************************************************
 *
 * Given the known values of the actual anchor, target and deviation,
 * we calculate the actual anchor, actual target and actual deviation
 * using FullWaveformIngestion functions
 *
 ****************************************************************************/
TEST_F(FlightLineDataTest, testInstrumentInformation){

    FlightLineData fld;

    // Known FlightLineData
    int known_scanner_id = 1;
    double known_wave_length = 1064;
    double known_outgoing_pulse_width = 4.822761;
    int known_scan_pattern = 2;
    int known_numer_of_mirror_facets = 4;
    double known_scan_frequency = 146.666672;
    double known_scan_angle_min = 0.00001;
    double known_scan_angle_max = 359.999969;
    double known_pulse_frequency = 400;
    double known_beam_diameter_at_exit_aperture = 10;
    double known_beam_divergence = 0.5;
    double known_minimal_range = 660.48053;
    double known_maximal_range = 1860.605225;

    std::string file_name =  "etc/140823_183115_1_clipped_test.pls";
    EXPECT_NO_THROW (fld.setFlightLineData(file_name));

    EXPECT_EQ (known_scanner_id, fld.scanner_id);
    EXPECT_DOUBLE_EQ (known_wave_length, fld.wave_length);
    EXPECT_NEAR (known_outgoing_pulse_width, fld.outgoing_pulse_width,
            0.0000005);
    EXPECT_EQ (known_scan_pattern, fld.scan_pattern);
    EXPECT_EQ (known_numer_of_mirror_facets, fld.number_of_mirror_facets);
    EXPECT_NEAR (known_scan_frequency, fld.scan_frequency, 0.000001);
    EXPECT_NEAR (known_scan_angle_min, fld.scan_angle_min, 0.0000001);
    EXPECT_NEAR (known_scan_angle_max, fld.scan_angle_max, 0.0000005);
    EXPECT_DOUBLE_EQ (known_pulse_frequency, fld.pulse_frequency);
    EXPECT_DOUBLE_EQ (known_beam_diameter_at_exit_aperture,
            fld.beam_diameter_at_exit_aperture);
    EXPECT_NEAR (known_beam_divergence, fld.beam_divergence, 0.01);
    EXPECT_NEAR (known_minimal_range, fld.minimal_range, 0.00001);
    EXPECT_NEAR (known_maximal_range, fld.maximal_range, 0.000001);
}

/****************************************************************************
 *
 *
 ****************************************************************************/
TEST_F(FlightLineDataTest, testGetNextPulse){

    FlightLineData fld;

    std::string file_name =  "etc/140823_183115_1_clipped_test.pls";
    EXPECT_NO_THROW (fld.setFlightLineData(file_name));

    PulseData pd;

    EXPECT_NO_THROW(fld.getNextPulse(&pd));

}

/****************************************************************************
 *
 * Given the known values of the actual geoascii_params, make sure we
 * can parse it
 *
 ****************************************************************************/
TEST_F(FlightLineDataTest, testGeoAsciiParsing){

    FlightLineData fld;
    std::stringstream geoascii_params(" UTM 11 / NAD 83|UTM 11| NAD 83 ");
    std::vector<std::string> tokens;
    fld.tokenize_geoascii_params_to_vector( &geoascii_params,&tokens);
    EXPECT_EQ(tokens.size(),4);
    EXPECT_EQ(tokens.at(0),"UTM 11");
    EXPECT_EQ(tokens.at(1),"NAD 83");
    EXPECT_EQ(tokens.at(2),"UTM 11");
    EXPECT_EQ(tokens.at(3),"NAD 83");
}

/****************************************************************************
 *
 * Given a vector of strings, see if we find the UTM string index
 *
 ****************************************************************************/
TEST_F(FlightLineDataTest, testUTMParsing){

    FlightLineData fld;
    std::vector<std::string> tokens;
    tokens.push_back("UTM 11");
    tokens.push_back("NAD 83");
    tokens.push_back("WGS 84");
    tokens.push_back("UTM 11");
    int idx = fld.locate_utm_field( &tokens);
    EXPECT_EQ(idx,0);

}

/****************************************************************************
 *
 * Given a vector of strings, see if we can find the geog_cs index
 *
 ****************************************************************************/
TEST_F(FlightLineDataTest, testGeogCSParsing){

    FlightLineData fld;
    std::vector<std::string> tokens;
    tokens.push_back("UTM 11");
    tokens.push_back("NAD 83");
    tokens.push_back("WGS 84");
    tokens.push_back("UTM 11");
    int idx = fld.locate_geog_cs_field(&tokens);
    EXPECT_EQ(idx,1);

}


/****************************************************************************
 *
 * Given a vector of strings with no UTM data, make sure we find none.
 *
 ****************************************************************************/
TEST_F(FlightLineDataTest, testUTMParsing_not_found){

    FlightLineData fld;
    std::vector<std::string> tokens;
    tokens.push_back("UFL 11");
    tokens.push_back("NAD 83");
    tokens.push_back("WGS 84");
    tokens.push_back("UPP 11");
    int idx = fld.locate_utm_field( &tokens);
    EXPECT_EQ(idx,-1);

}

/****************************************************************************
 *
 * Given a vectof of strings with no geog_cs data, make sure we find none.
 *
 ****************************************************************************/
TEST_F(FlightLineDataTest, testGeogCSParsing_not_found){

    FlightLineData fld;
    std::vector<std::string> tokens;
    tokens.push_back("UTM 11");
    tokens.push_back("UTM 12");
    tokens.push_back("WND 12");
    tokens.push_back("NPP 12");
    tokens.push_back("OPP 10");
    int idx = fld.locate_geog_cs_field(&tokens);
    EXPECT_EQ(idx,-1);

}
