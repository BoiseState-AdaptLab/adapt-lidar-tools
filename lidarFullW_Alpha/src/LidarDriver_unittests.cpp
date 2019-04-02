//
// Created by arezaii on 3/22/19.
//

#include "LidarDriver.hpp"
#include "gtest/gtest.h"
#include "Peak.hpp"
#include <math.h>

class LidarDriverTest: public testing::Test{
public:
	std::vector<PulseData*> pulses;
	PulseData* pd;

protected:


	char** commonArgSpace;
	int numberOfArgs;
	int maxLengthOfArg;
	LidarDriver driver1;
	LidarDriver driver2;
	LidarDriver driver3;
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

	EXPECT_EQ(50, roundf(val));

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

	EXPECT_EQ(1.00,val);

}

/******************************************************************************
*
* Test 16
*
******************************************************************************/
TEST_F(LidarDriverTest, get_all_amp_mean_test){

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
		peaks.push_back(p);
		i++;
		ptr = strtok (NULL," ");
	}

	float val = driver1.get_mean(&peaks,2,'a');

	EXPECT_EQ(44, roundf(val));

}
