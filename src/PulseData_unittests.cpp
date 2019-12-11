// File name: LidarVolume_unittests.cpp
// Created on: 15-December-2017
// Author: nicholas chapa
// Modified by: ravi

#include <string>
#include <iostream>
#include <sstream>
#include "gtest/gtest.h"
#include "PulseData.hpp"

class PulseDataTest: public testing::Test{
	public:


		virtual void SetUp(){

		}

};

TEST_F(PulseDataTest, testSetterGetter){

	PulseData pulseData;
	std::string realStream;
	std::string streamData;
	
	//memory cleaned up in deconstructor of pulseData
	int* indexArrayOut = new int[3] {0, 1, 2};
	int* waveArrayOut = new int[3] {34, 45, 67};
	int* indexArrayIn = new int[3] {0, 1, 2};
	int* waveArrayIn = new int[3] {76, 65, 43};

	size_t size = 3;
	struct vector idxOut = {indexArrayOut, size, size};
	struct vector waveOut = {waveArrayOut, size, size};
	struct vector idxIn = {indexArrayIn, size, size};
	struct vector waveIn = {waveArrayIn, size, size};

	pulseData.setOutgoing(&idxOut, &waveOut);
	pulseData.setReturning(&idxIn, &waveIn);

	std::ostringstream stream;
	pulseData.displayPulseData(&stream);

	streamData = stream.str();

	realStream = "\nOutgoing:\n\n\tindices:\n\t0 1 2 \n\tamplitudes:\n" 
        "\t34 45 67 \n\nReturning Wave:\n\n\tindices:\n\t0 1 2 \n\t" 
        "amplitudes:\n\t76 65 43 ";

	// testing for string will go here
	ASSERT_EQ( streamData, realStream);
}







