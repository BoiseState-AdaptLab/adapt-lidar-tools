//PulseData_unittest.cpp
//Created 12/15/2017
//Author: Nicholas Chapa

#include "gtest/gtest.h"
#include "PulseData.hpp"
#include "pulsereader.hpp"
#include "pulsewriter.hpp"

class PulseDataTest: public testing::Test{
  public:
    std::vector<int> indexArrayOut;
    std::vector<int> waveArrayOut;
    std::vector<int> indexArrayIn;
    std::vector<int> waveArrayIn;
    
    PulseData pulseData;
    
    virtual void SetUp(){
        pulseData = new PulseData();
    
        indexArrayOut = { 1, 2, 3 }
        waveArrayOut = { 34, 45, 67 }
        indexArrayIn = { 4, 5, 6 }
        waveArrayIn = { 76, 65, 43 }
    }
    
TEST_F(PulseDataTest, testSetterGetter){
    pulseData.setOutgoing(&indexArrayOut, &waveArrayOut);
    pulseData.setReturning(&indexArrayIn, &waveArrayIn);
    
}
}







