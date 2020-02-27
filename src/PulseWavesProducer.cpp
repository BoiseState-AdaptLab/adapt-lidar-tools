
#include "spdlog/spdlog.h"

#include "PulseWavesProducer.hpp"

PulseWavesProducer::PulseWavesProducer(const std::string& fileName){
    bool failed = flightData.setFlightLineData(fileName);
    if(failed){
        spdlog::error("Bad file \"{}\"", fileName);
        valid = false;
    }
}

PulseWavesProducer::~PulseWavesProducer(){
    flightData.closeFlightLineData();
    //@@TODO
}

void PulseWavesProducer::producePulse(PulseData& data){
    flightData.getNextPulse(data);
}

void PulseWavesProducer::postProcess(std::vector<Peak>& peaks, const PulseData& data) const{
    flightData.calc_xyz_activation(peaks, data);
}

bool PulseWavesProducer::done() const{
    return !flightData.hasNextPulse();
}

PulseWavesProducer::operator bool() const{
    return valid;
}
