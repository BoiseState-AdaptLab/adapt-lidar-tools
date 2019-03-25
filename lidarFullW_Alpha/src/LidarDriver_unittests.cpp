//
// Created by arezaii on 3/22/19.
//

#include "LidarDriver.hpp"
#include "gtest/gtest.h"
#include "Peak.hpp"

class LidarDriverTest: public testing::Test{
public:
	std::vector<PulseData*> pulses;
	PulseData* pd;

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

	// create a vector of integers
	std::vector<int> idxData;
	std::vector<int> ampData;
	std::vector<Peak> peaks;

	char input[] = "2 2 1 1 0 1 1 2 2 2 2 6 14 36 74 121 162 190 200 200 192 179 160 139 120 99 79 63 50 46 43 43 40 35 31 28 29 33 34 31 24 17 11 8 7 6 5 6 5 4 4 5 5 6 5 5 2 1 1 1";

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

	float val = driver1.get_z_activation_extreme(&peaks,true);

	EXPECT_EQ(200.00,val);

}