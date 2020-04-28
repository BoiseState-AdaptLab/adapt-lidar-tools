#include <cassert>
#include <vector>

#include "spdlog/spdlog.h"

#include "Common.hpp"
#include "GaussianFitter.hpp"
#include "Peak.hpp"
#include "PreProcess.hpp"

namespace Common{

bool fitWaveform(std::vector<int>& indexData, std::vector<int>& amplitudeData, const Options& options, std::vector<GaussianFitter::Gaussian>& results){
    results.clear();
    assert(indexData.size() == amplitudeData.size());
    if(indexData.empty()){
        spdlog::trace("[Common] No data");
        return true;
    }

    if(options.reduceNoise){
        PreProcess::reduceNoise(amplitudeData, options.noiseLevel);
        PreProcess::reduceNoise(amplitudeData, options.noiseLevel/3, 2.0);
    }

    if(options.smoothData){
        PreProcess::smoothData(indexData, amplitudeData);
    }

    //@@TODO Any scenarios in which the guesser fails?
    GaussianFitter::estimateGaussians(indexData, amplitudeData, options.minPeakAmp, results);

    bool fitValid = true;
    if(options.nlsFitting && !results.empty()){
        fitValid = GaussianFitter::fitGaussians(indexData, amplitudeData, options.minPeakAmp, results);
    }

    return fitValid;
}
}   // namespace Common
