//PulseData_unittest.cpp
//Created 12/15/2017
//Author: Nicholas Chapa
#include <string>
#include <iostream>
#include <sstream>
#include "gtest/gtest.h"
#include "PulseData.hpp"
#include "pulsereader.hpp"
#include "pulsewriter.hpp"

class PulseDataTest: public testing::Test{
  public:
    
    
    virtual void SetUp(){
    
    }
    
};

TEST_F(PulseDataTest, testSetterGetter){

    PulseData pulseData;
    std::string realStream;

    std::vector<int> indexArrayOut;
    std::vector<int> waveArrayOut;
    std::vector<int> indexArrayIn;
    std::vector<int> waveArrayIn;

    indexArrayOut.push_back(0);
    indexArrayOut.push_back(1);
    indexArrayOut.push_back(2);

    waveArrayOut.push_back(34);
    waveArrayOut.push_back(45);
    waveArrayOut.push_back(67);

    indexArrayIn.push_back(0);
    indexArrayIn.push_back(1);
    indexArrayIn.push_back(2);

    waveArrayIn.push_back(76);
    waveArrayIn.push_back(65);
    waveArrayIn.push_back(43);

    pulseData.setOutgoing(&indexArrayOut, &waveArrayOut);
    pulseData.setReturning(&indexArrayIn, &waveArrayIn);
    
    std::ostringstream stream;
    pulseData.displayPulseData(&stream);

    realStream = "Outgoing:\n\tindices:\n0 1 2 \tamplitudes:\n34 45 67 \nReturning Wave:\n\tindices:\n0 1 2 \tamplitudes:\n76 65 43 ";
    
    std::string a; 
    a = "abc";
    std::string b; 
    b = "abc";

    // testing for string will go here    
    ASSERT_EQ( a, b);
}







