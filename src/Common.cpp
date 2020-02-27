#include <cassert>

#include "spdlog/spdlog.h"

#include "Common.hpp"
#include "Fitter.hpp"
#include "Peak.hpp"

namespace Common{

void fitWaveform(const std::vector<int>& indexData, const std::vector<int>& amplitudeData, int noiseLevel, bool useNLSFitting, std::vector<Peak>& results){
    assert(indexData.size() == amplitudeData.size());
    if(indexData.empty()){
        spdlog::trace("No data");
        return;
    }

    std::vector<Fitter::Gaussian> guesses;
    Fitter::guessGaussians(indexData, amplitudeData, noiseLevel, guesses);

    if(useNLSFitting){
        bool result = Fitter::fitGaussians(indexData, amplitudeData, guesses);
        if(!result){
            //@@TODO dump the peaks?
            spdlog::error("Failed to fit waveform");
        }
    }

    results.clear();
    for(const Fitter::Gaussian& gaussian : guesses){
        //@@TODO this is where we would fill in the additional peak params (trig loc, trig amp). We would also probably verify the results here?
        Peak peak;
        peak.amp = gaussian.a;
        peak.location = gaussian.b;
        peak.fwhm = cToFWHM * gaussian.c;
        results.push_back(peak);
    }
}
}   // namespace Common
