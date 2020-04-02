
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

bool PulseWavesProducer::done() const{
    return !flightData.hasNextPulse();
}

PulseWavesProducer::operator bool() const{
    return valid;
}
