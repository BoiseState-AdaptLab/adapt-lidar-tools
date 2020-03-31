#include <algorithm>
#include <cmath>
#include <vector>

#include "PreProcess.hpp"

namespace PreProcess{

//See PreProcess.hpp for docs
void reduceNoise(std::vector<int>& amplitudeData, int noiseLevel, double aggression){
    const double scaleFactor = aggression*noiseLevel;

    for(int& i : amplitudeData){
        if(i > noiseLevel) continue;

        const double newValue = i * i / scaleFactor;
        i = static_cast<int>(std::round(newValue)); //@@TODO: Should we just trunc instead?
    }
}

//See PreProcess.hpp for docs
void smoothData(const std::vector<int>& indexData, std::vector<int>& amplitudeData){
    if(indexData.empty() || amplitudeData.empty()) return;

    const int max = *std::max_element(amplitudeData.begin(), amplitudeData.end());

    int prevValue = amplitudeData.front();
    for(std::size_t i = 1; i < amplitudeData.size()-1; ++i){
        //Account for segmented waves
        if(indexData[i] - indexData[i] != 1 || indexData[i+1] - indexData[i] != 1){
            prevValue = amplitudeData[i];
            continue;
        }

        const double delta = amplitudeData[i] - (prevValue+amplitudeData[i]+amplitudeData[i+1])/3.0;
        prevValue = amplitudeData[i];

        const double diff = amplitudeData[i]-max;
        const double scaleFactor = std::pow(diff/max, 2);
        amplitudeData[i] -= delta*scaleFactor;
    }
}

}   // namespace PreProcess
