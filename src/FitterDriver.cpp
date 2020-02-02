//This is a temp file to directly run the new fitter. It should be deleted once it is no longer needed
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

#include "spdlog/spdlog.h"

#include "Fitter.hpp"

enum class Test {NayaniClipped1, NayaniClipped7, real2};

std::pair<std::vector<Fitter::Gaussian>, std::vector<double>> getTestData(Test test){
    switch(test){
        case Test::NayaniClipped1:
            return {
                {
                    {10,    15,     0.5},
                    {10,    20,     0.5},
                    {10,    31,     0.5},
                    {10,    38,     0.5}
                },
                {1,1,0,0,0,0,0,0,0,0,1,4,13,35,73,120,161,189,199,199,191,178,159,138,119,98,78,62,49,45,42,42,39,34,30,27,28,32,33,30,23,16,10,7,7,5,5,4,4,3,3,3,3,5,4,3,2,1,0,0}
            };
        case Test::NayaniClipped7:
            return {
                {
                    {97,    18,     3.88498},
                    {167,   31,     4.40505},
                    {12,    40.5,   4.14005}
                },
                {0,0,0,0,0,0,0,0,0,0,0,0,1,6,17,37,63,85,96,95,87,80,74,77,91,112,128,135,139,150,163,166,151,117,75,44,24,17,13,11,11,11,11,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,2}
            };
        case Test::real2:
            return {
                {
                    {50,    15,     1},
                    {20,    25,     1},
                    {200,   34,     1},
                    {150,   38,     1},
                    {20,    48,     1}
                },
                {1,1,0,0,0,0,0,0,0,0,0,1,5,16,30,41,40,32,20,10,4,2,4,5,10,12,12,10,14,35,81,140,191,217,220,211,200,194,191,182,164,137,110,85,61,43,29,23,19,19,17,14,13,10,8,7,6,5,4,3,2,2,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
            };
    }
}

int main(){
    spdlog::set_level(spdlog::level::trace);

    Test test = 
        Test::real2;

    auto testData = getTestData(test);

    auto guesses = testData.first;
    auto ampData = testData.second;

    std::vector<double> idxData(ampData.size(), 0.0);
    std::iota(idxData.begin(), idxData.end(), 0.0); //Generate range [0.....ampData.size()-1]

    Fitter::fitGaussians(idxData, ampData, guesses);

}
