#include <cassert>

#include "spdlog/spdlog.h"

#include "Common.hpp"
#include "Fitter.hpp"
#include "Peak.hpp"

namespace Common{

//See Common.hpp for docs
void fitWaveforms(const std::vector<std::vector<int>>& indices, std::vector<std::vector<int>>& amplitudes, int noiseLevel, bool nlsFitting, std::vector<std::vector<Peak>>& results){
    //@@TODO runtime check instead?
    assert(indices.size() == amplitudes.size());
    assert(!indices.empty());

    results.resize(indices.size()); //Reallocs if indices.size() > capacity, otherwise just updates the size (no realloc)

    for(std::size_t i = 0, n = indices.size(); i < n; ++i){
        //Fitter::smoothAmplitude(amplitudes[i]);   @@TODO write our own smoother, the current one seems super weirdS
        std::vector<Fitter::Gaussian> guesses;  //Shouldn't realloc each time
        Fitter::guessGaussians(indices[i], amplitudes[i], noiseLevel, guesses);

        if(nlsFitting){
            bool result = Fitter::fitGaussians(indices[i], amplitudes[i], guesses);
            if(!result){
                //@@TODO dump the peaks?
                spdlog::error("Failed to fit waveform");
            }
        }

        //@@TODO this is where we would verify the peaks.
        
        results[i].clear(); //Set size to zero, don't dump memory
        for(const Fitter::Gaussian& gaussian : guesses){
            //@@TODO this is where we would fill in the additional peak params (trig loc, trig amp)
            Peak peak;
            peak.amp = gaussian.a;
            peak.location = gaussian.b;
            peak.fwhm = cToFWHM * gaussian.c;
            results[i].push_back(peak);
        }
    }
}

}   // namespace Common
