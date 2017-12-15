//PulseData_unittest.cpp
//Created 12/15/2017
//Author: Nicholas Chapa

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

    indexArrayIn.push_back(76);
    indexArrayIn.push_back(65);
    indexArrayIn.push_back(43);

    pulseData.setOutgoing(&indexArrayOut, &waveArrayOut);
    pulseData.setReturning(&indexArrayIn, &waveArrayIn);
    
}







