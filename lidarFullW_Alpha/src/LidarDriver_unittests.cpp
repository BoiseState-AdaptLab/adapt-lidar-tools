//
// Created by arezaii on 3/22/19.
//

#include "LidarDriver.hpp"
#include "gtest/gtest.h"
#include "Peak.hpp"
#include <math.h>

#define INPUT_TYPICAL \
"2 2 1 1 0 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 139 120 99 "\
"79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 "\
"2 1 1 1"

#define INPUT_RISING \
"1 1 2 3 3 4 5 6 6 7 8 9 9 10 20 20 30 40 50 50 53 58 60 62 64 68 70 80 90 "\
"100 120 145 156 178 194 200 213 219 219 220 220 230 230 259 459 500 560 600 "\
"612 630 640"

#define INPUT_FALLING \
"800 800 700 700 600 600 590 590 580 580 580 540 420 420 420 419 419 419"\
"419 412 394 320 219 218 217 217 216 216 202 199 189 187 179 178 175 174 "\
"140 120 80 50 40 30 20 10 5 2 2 2 2 2 1 1 1 0"\

#define INPUT_CONST "70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 "\
"70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 70 "\
"70 70 70 70 70 70 70 70"

class LidarDriverTest: public testing::Test{
public:
    std::vector<PulseData*> pulses;
    PulseData* pd;

protected:

    //Strings can be shorter than 100 but not more
    char *typicalInput;
    char *risingInput;
    char *fallingInput;
    char *constantInput;

    char** commonArgSpace;
    int numberOfArgs;
    int maxLengthOfArg;
    LidarDriver driver1;
    LidarDriver driver2;
    LidarDriver driver3;

    void SetUp() {
        typicalInput = (char*) malloc(sizeof(char) * 101);
        risingInput = (char*) malloc(sizeof(char) * 101);
        fallingInput = (char*) malloc(sizeof(char) * 101);
        constantInput = (char*) malloc(sizeof(char) * 101); //extra 1 for \0 character

        strncpy(typicalInput, INPUT_TYPICAL, 101);
        typicalInput[100] = '\0';
        strncpy(risingInput, INPUT_RISING, 101);
        risingInput[100] = '\0';
        strncpy(fallingInput, INPUT_FALLING, 101);
        fallingInput[100] = '\0';
        strncpy(constantInput, INPUT_CONST, 101);
        constantInput[100] = '\0';
    }

    void TearDown() {
        free(typicalInput);
        free(risingInput);
        free(fallingInput);
        free(constantInput);
    }
};


/******************************************************************************
*
* Test 1
*
******************************************************************************/
TEST_F(LidarDriverTest, get_z_activation_max_test){

    std::vector<Peak> peaks;

    char input[] = "2 2 1 1 0 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 "
            "139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

    char* ptr;
    ptr = strtok (input," ");
    int i=0;
    while (ptr != NULL){
        int z0 = atoi(ptr);
        Peak p;
        p.z_activation = z0;
        peaks.push_back(p);
        i++;
        ptr = strtok (NULL," ");
    }

    float val = driver1.get_extreme(&peaks,true,2,'z');

    EXPECT_EQ(200.00,val);

}


/******************************************************************************
*
* Test 2
*
******************************************************************************/
TEST_F(LidarDriverTest, get_z_activation_min_test){

	std::vector<Peak> peaks;

	char input[] = "2 2 1 1 2 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 139 120 99 79 63 50 46 43 43 40 "
				"35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

	char* ptr;
	ptr = strtok (input," ");
	int i=0;
	while (ptr != NULL){
		int z0 = atoi(ptr);
		Peak p;
		p.z_activation = z0;
		peaks.push_back(p);
		i++;
		ptr = strtok (NULL," ");
	}

	float val = driver1.get_extreme(&peaks,false,2,'z');

	EXPECT_EQ(1.00,val);

}

/******************************************************************************
*
* Test 3
*
******************************************************************************/
TEST_F(LidarDriverTest, get_z_activation_max_min_test){

	std::vector<Peak> peaks;

	char input[] = "2 2 1 1 2 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 139 120 99 79 63 50 46 43 43 40 "
	               "35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

	char* ptr;
	ptr = strtok (input," ");
	int i=0;
	while (ptr != NULL){
		int z0 = atoi(ptr);
		Peak p;
		p.z_activation = z0;
		peaks.push_back(p);
		i++;
		ptr = strtok (NULL," ");
	}

	float val = driver1.get_extreme_diff(&peaks,'z');

	EXPECT_EQ(199.00,val);

}

/******************************************************************************
*
* Test 4 *lot of redundancies with FlightLineData_unittests in this test
*
******************************************************************************/
TEST_F(LidarDriverTest, setup_flight_data_test){
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

	double known_bb_x_min = 516209.586;
	double known_bb_y_min = 4767921.375;
	double known_bb_z_min = 2084.585;
	double known_bb_x_max = 516211.942;
	double known_bb_y_max = 4767923.621;
	double known_bb_z_max = 2093.581;

	FlightLineData fld;
	std::string filename;
	filename = "etc/140823_183115_1_clipped_test.pls";

	EXPECT_NO_THROW(driver1.setup_flight_data(fld,filename));

	EXPECT_EQ(fld.hasNextPulse(),true);

	EXPECT_EQ (known_scanner_id, fld.scanner_id);
	EXPECT_DOUBLE_EQ (known_wave_length, fld.wave_length);
	EXPECT_NEAR (known_outgoing_pulse_width, fld.outgoing_pulse_width, 0.0000005);
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

	EXPECT_DOUBLE_EQ (known_bb_x_min, fld.bb_x_min);
	EXPECT_DOUBLE_EQ (known_bb_y_min, fld.bb_y_min);
	EXPECT_DOUBLE_EQ (known_bb_z_min, fld.bb_z_min);
	EXPECT_DOUBLE_EQ (known_bb_x_max, fld.bb_x_max);
	EXPECT_DOUBLE_EQ (known_bb_y_max, fld.bb_y_max);
	EXPECT_DOUBLE_EQ (known_bb_z_max, fld.bb_z_max);

}


/******************************************************************************
*
* Test 5
*
******************************************************************************/
TEST_F(LidarDriverTest, add_peaks_to_volume_test){

	std::vector<Peak> peaks;
	LidarVolume lidarVolume;
	FlightLineData fld;
	std::string filename;
	filename = "etc/140823_183115_1_clipped_test.pls";

	EXPECT_NO_THROW(driver1.setup_flight_data(fld,filename));


	double known_amp = 142;
	double known_loc = 19;
	double known_fwhm = 5;
	double known_fwhm_t_pos = 21;
	double known_fwhm_t_neg = 16;
	int known_pos = 0;
	int known_triggering_amp = 13;
	int known_triggering_loc = 14;
	double known_x_act = 253036;
	double known_y_act = 4330482;
	double known_z_act = 4159;

	Peak p;
	p.amp= known_amp;
	p.location = known_loc;
	p.fwhm = known_fwhm;
	p.fwhm_t_positive = known_fwhm_t_pos;
	p.fwhm_t_negative = known_fwhm_t_neg;
	p.position_in_wave = known_pos;
	p.triggering_amp = known_triggering_amp;
	p.triggering_location = known_triggering_loc;
	p.x_activation = known_x_act;
	p.y_activation = known_y_act;
	p.z_activation = known_z_act;

	peaks.push_back(p);
	//EXPECT_NO_THROW(driver1.setup_lidar_volume(fld,lidarVolume));
	//EXPECT_NO_THROW(driver1.add_peaks_to_volume(lidarVolume,peaks,1));


	//EXPECT_EQ(peaks,lidarVolume.volume);

}


/******************************************************************************
*
* Test 6
*
******************************************************************************/
TEST_F(LidarDriverTest, get_first_amp_max_test){

	std::vector<Peak> peaks;

	char input[] = "2 2 1 1 0 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 "
	               "139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

	char* ptr;
	ptr = strtok (input," ");
	int i=0;
	while (ptr != NULL){
		int z0 = atoi(ptr);
		Peak p;
		p.amp = z0;
		z0== 200 ? p.position_in_wave = 3 : p.position_in_wave=1;
		peaks.push_back(p);
		i++;
		ptr = strtok (NULL," ");
	}

	float val = driver1.get_extreme(&peaks,true,0,'a');

	EXPECT_EQ(192.00,val);

}


/******************************************************************************
*
* Test 7
*
******************************************************************************/
TEST_F(LidarDriverTest, get_first_amp_min_test){

	std::vector<Peak> peaks;

	char input[] = "2 2 1 1 2 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 139 120 99 79 63 50 46 43 43 40 "
	               "35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

	char* ptr;
	ptr = strtok (input," ");
	int i=0;
	while (ptr != NULL){
		int z0 = atoi(ptr);
		Peak p;
		p.amp = z0;
		z0== 1 ? p.position_in_wave = 3 : p.position_in_wave=1;
		peaks.push_back(p);
		i++;
		ptr = strtok (NULL," ");
	}

	float val = driver1.get_extreme(&peaks,false,0,'a');

	EXPECT_EQ(2.00,val);

}

/******************************************************************************
*
* Test 8
*
******************************************************************************/
TEST_F(LidarDriverTest, get_first_amp_mean_test){

	std::vector<Peak> peaks;

    double abs_error = 0.05;

    double expected = 50.1346;

	char input[] = "2 2 1 1 2 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 139 120 99 79 63 50 46 43 43 40 "
	               "35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

	char* ptr;
	ptr = strtok (input," ");
	int i=0;
	while (ptr != NULL){
		int z0 = atoi(ptr);
		Peak p;
		p.amp = z0;
		z0== 2 ? p.position_in_wave = 3 : p.position_in_wave=1;
		peaks.push_back(p);
		i++;
		ptr = strtok (NULL," ");
	}

	float val = driver1.get_mean(&peaks,0,'a');

	EXPECT_NEAR(expected, val, abs_error);

}


/******************************************************************************
*
* Test 11
*
******************************************************************************/
TEST_F(LidarDriverTest, get_last_amp_max_test){

	std::vector<Peak> peaks;

	char input[] = "2 2 1 1 0 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 "
	               "139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

	char* ptr;
	ptr = strtok (input," ");
	int i=0;
	while (ptr != NULL){
		int z0 = atoi(ptr);
		Peak p;
		p.amp = z0;
		z0== 200 ? p.is_final_peak = false : p.is_final_peak=true;
		peaks.push_back(p);
		i++;
		ptr = strtok (NULL," ");
	}

	float val = driver1.get_extreme(&peaks,true,1,'a');

	EXPECT_EQ(192.00,val);

}


/******************************************************************************
*
* Test 12
*
******************************************************************************/
TEST_F(LidarDriverTest, get_last_amp_min_test){

	std::vector<Peak> peaks;

	char input[] = "2 2 1 1 2 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 139 120 99 79 63 50 46 43 43 40 "
	               "35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

	char* ptr;
	ptr = strtok (input," ");
	int i=0;
	while (ptr != NULL){
		int z0 = atoi(ptr);
		Peak p;
		p.amp = z0;
		z0== 1 ? p.is_final_peak = false : p.is_final_peak=true;
		peaks.push_back(p);
		i++;
		ptr = strtok (NULL," ");
	}

	float val = driver1.get_extreme(&peaks,false,1,'a');

	EXPECT_EQ(2.00,val);

}

/******************************************************************************
*
* Test 13
*
******************************************************************************/
TEST_F(LidarDriverTest, get_last_amp_mean_test){

	std::vector<Peak> peaks;

	char input[] = "2 2 1 1 2 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 139 120 99 79 63 50 46 43 43 40 "
	               "35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

	char* ptr;
	ptr = strtok (input," ");
	int i=0;
	while (ptr != NULL){
		int z0 = atoi(ptr);
		Peak p;
		p.amp = z0;
		z0== 200 ? p.is_final_peak = false : p.is_final_peak=true;
		peaks.push_back(p);
		i++;
		ptr = strtok (NULL," ");
	}

	float val = driver1.get_mean(&peaks,1,'a');

	EXPECT_EQ(38, roundf(val));

}


/******************************************************************************
*
* Test 14
*
******************************************************************************/
TEST_F(LidarDriverTest, get_all_amp_max_test){

	std::vector<Peak> peaks;

	char input[] = "2 2 1 1 0 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 "
	               "139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

	char* ptr;
	ptr = strtok (input," ");
	int i=0;
	while (ptr != NULL){
		int z0 = atoi(ptr);
		Peak p;
		p.amp = z0;
		peaks.push_back(p);
		i++;
		ptr = strtok (NULL," ");
	}

	float val = driver1.get_extreme(&peaks,true,2,'a');

	EXPECT_EQ(200.00,val);

}


/******************************************************************************
*
* Test 15
*
******************************************************************************/
TEST_F(LidarDriverTest, get_all_amp_min_test){

	std::vector<Peak> peaks;

	float expected = 1.00;
	float abs_error = 0.5;

	char input[] = "2 2 1 1 2 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 139 120 99 79 63 50 46 43 43 40 "
	               "35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

	char* ptr;
	ptr = strtok (input," ");
	int i=0;
	while (ptr != NULL){
		int z0 = atoi(ptr);
		Peak p;
		p.amp = z0;
		peaks.push_back(p);
		i++;
		ptr = strtok (NULL," ");
	}

	float val = driver1.get_extreme(&peaks,false,2,'a');

	EXPECT_NEAR(val, expected, abs_error);

}

/******************************************************************************
*
* Test 16
*
******************************************************************************/
TEST_F(LidarDriverTest, get_all_amp_mean_test){

    std::vector<Peak> peaks;

    double avg = 43.71666667;
    float abs_error = 0.005;

    char input[] = "2 2 1 1 2 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 "
        "179 160 139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11"
        " 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

    char* ptr;
    ptr = strtok (input," ");
    int i=0;
    while (ptr != NULL){
        int z0 = atoi(ptr);
        Peak p;
        p.amp = z0;
        peaks.push_back(p);
        i++;
        ptr = strtok (NULL," ");
    }

    float val = driver1.get_mean(&peaks,2,'a');

    EXPECT_NEAR(avg, val, abs_error);

}

/******************************************************************************
*
* Test 17
*
******************************************************************************/
TEST_F(LidarDriverTest, get_all_amp_deviation_test){

    std::vector<Peak> peaks;

    double avg = 43.71666667;

    double expected = 59.98835767;
    double abs_error = 0.05;

    char input[] = "2 2 1 1 2 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 "
        "179 160 139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11"
        " 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";


    char* ptr;
    ptr = strtok (input," ");
    int i=0;
    while (ptr != NULL){
        int z0 = atoi(ptr);
        Peak p;
        p.amp = z0;
        peaks.push_back(p);
        i++;
        ptr = strtok (NULL," ");
    }

    double val = driver1.get_deviation(&peaks, avg, 2,'a');

    EXPECT_NEAR(val, expected, abs_error);
}

/******************************************************************************
*
* Test 18
*
******************************************************************************/
TEST_F(LidarDriverTest, get_all_amp_skewtosis_skewness_test){

    std::vector<Peak> peaks;

    double avg = 43.71666667;
    double stdev = 59.98835767;

    double expected = 1.523627739;
    double abs_error = 0.05;

    char input[] = "2 2 1 1 2 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 "
        "179 160 139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11"
        " 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";


    char* ptr;
    ptr = strtok (input," ");
    int i=0;
    while (ptr != NULL){
        int z0 = atoi(ptr);
        Peak p;
        p.amp = z0;
        peaks.push_back(p);
        i++;
        ptr = strtok (NULL," ");
    }

    double val = driver1.get_skewtosis(&peaks, avg, stdev, 2, 'a', 3);

    EXPECT_NEAR(val, expected, abs_error);
}

/******************************************************************************
*
* Test 19
*
******************************************************************************/
TEST_F(LidarDriverTest, get_all_amp_skewtosis_kurtosis_test){

    std::vector<Peak> peaks;

    double avg = 43.716667;
    double stdev = 59.9883576667636;

    double expected = 3.97964179113; //https://www.easycalculation.com/statistics/kurtosis.php
	double abs_error = 0.05;

    char input[] = "2 2 1 1 2 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 "
        "179 160 139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 "
        " 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";


    char* ptr;
    ptr = strtok (input," ");
    int i=0;
    while (ptr != NULL){
        int z0 = atoi(ptr);
        Peak p;
        p.amp = z0;
        peaks.push_back(p);
        i++;
        ptr = strtok (NULL," ");
    }

    double val = driver1.get_skewtosis(&peaks, avg, stdev, 2, 'a', 4);

    EXPECT_NEAR(val, expected, abs_error);
}

/******************************************************************************
*
* Test 20
*
******************************************************************************/
TEST_F(LidarDriverTest, get_first_amp_deviation_test){

    std::vector<Peak> peaks;

	double avg = 38.3275862068966;

    double expected = 53.3987562592582;
    double abs_error = 0.05;

    char input[] = "2 2 1 1 0 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 "
        "139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

    char* ptr;
    ptr = strtok (input," ");
    int i=0;
    while (ptr != NULL){
        int z0 = atoi(ptr);
        Peak p;
        p.amp = z0;
        z0== 200 ? p.position_in_wave = 3 : p.position_in_wave=1;
        peaks.push_back(p);
        i++;
        ptr = strtok (NULL," ");
    }

    double val = driver1.get_deviation(&peaks, avg, 0,'a');

    EXPECT_NEAR(val, expected, abs_error);
}

/******************************************************************************
*
* Test 21
*
******************************************************************************/
TEST_F(LidarDriverTest, get_first_amp_skewtosis_skewness_test){

    std::vector<Peak> peaks;

	double avg = 38.3275862068966;
    double stdev = 53.3987562592582;

    double expected = 1.67397950413312;
    double abs_error = 0.05;

    char input[] = "2 2 1 1 0 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 "
        "139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

    char* ptr;
    ptr = strtok (input," ");
    int i=0;
    while (ptr != NULL){
        int z0 = atoi(ptr);
        Peak p;
        p.amp = z0;
        z0== 200 ? p.position_in_wave = 3 : p.position_in_wave=1;
        peaks.push_back(p);
        i++;
        ptr = strtok (NULL," ");
    }

    double val = driver1.get_skewtosis(&peaks, avg, stdev, 0, 'a', 3);

    EXPECT_NEAR(val, expected, abs_error);
}

/******************************************************************************
*
* Test 22
*
******************************************************************************/
TEST_F(LidarDriverTest, get_first_amp_skewtosis_kurtosis_test){

    std::vector<Peak> peaks;

	double avg = 38.3275862068966;
	double stdev = 53.3987562592582;

    double expected =  4.64158429732; //https://www.easycalculation.com/statistics/kurtosis.php
    double abs_error = 0.05;

   char input[] = "2 2 1 1 0 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 "
        "139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

    char* ptr;
    ptr = strtok (input," ");
    int i=0;
    while (ptr != NULL){
        int z0 = atoi(ptr);
        Peak p;
        p.amp = z0;
        z0== 200 ? p.position_in_wave = 3 : p.position_in_wave=1;
        peaks.push_back(p);
        i++;
        ptr = strtok (NULL," ");
    }

    double val = driver1.get_skewtosis(&peaks, avg, stdev, 0, 'a', 4);

    EXPECT_NEAR(val, expected, abs_error);
}

/******************************************************************************
*
* Test 23
*
******************************************************************************/
TEST_F(LidarDriverTest, get_last_amp_deviation_test){

    std::vector<Peak> peaks;

    double avg = 38.3275862068966;

    double expected = 53.3987562592582;
    double abs_error = 0.05;

    char input[] = "2 2 1 1 0 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 "
        "139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

    char* ptr;
    ptr = strtok (input," ");
    int i=0;
    while (ptr != NULL){
        int z0 = atoi(ptr);
        Peak p;
        p.amp = z0;
        z0== 200 ? p.is_final_peak = false : p.is_final_peak=true;
        peaks.push_back(p);
        i++;
        ptr = strtok (NULL," ");
    }


    double val = driver1.get_deviation(&peaks, avg, 1,'a');

    EXPECT_NEAR(val, expected, abs_error);
}

/******************************************************************************
*
* Test 24
*
******************************************************************************/
TEST_F(LidarDriverTest, get_last_amp_skewtosis_skewness_test){

    std::vector<Peak> peaks;

    double avg = 38.3275862068966;
	double stdev = 53.3987562592582;

	double expected = 1.67397950413312;
	double abs_error = 0.05;

    char input[] = "2 2 1 1 0 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 "
        "139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

    char* ptr;
    ptr = strtok (input," ");
    int i=0;
    while (ptr != NULL){
        int z0 = atoi(ptr);
        Peak p;
        p.amp = z0;
        z0== 200 ? p.is_final_peak = false : p.is_final_peak=true;
        peaks.push_back(p);
        i++;
        ptr = strtok (NULL," ");
    }

    double val = driver1.get_skewtosis(&peaks, avg, stdev, 1, 'a', 3);

    EXPECT_NEAR(val, expected, abs_error);
}

/******************************************************************************
*
* Test 25
*
******************************************************************************/
TEST_F(LidarDriverTest, get_last_amp_skewtosis_kurtosis_test){

    std::vector<Peak> peaks;

	double avg = 38.3275862068966;
	double stdev = 53.3987562592582;

    double expected = 4.64158429732; //https://www.easycalculation.com/statistics/kurtosis.php
    double abs_error = 0.05;

    char input[] = "2 2 1 1 0 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 "
        "139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

    char* ptr;
    ptr = strtok (input," ");
    int i=0;
    while (ptr != NULL){
        int z0 = atoi(ptr);
        Peak p;
        p.amp = z0;
        z0== 200 ? p.is_final_peak = false : p.is_final_peak=true;
        peaks.push_back(p);
        i++;
        ptr = strtok (NULL," ");
    }

    double val = driver1.get_skewtosis(&peaks, avg, stdev, 1, 'a', 4);

    EXPECT_NEAR(val, expected, abs_error);
}
