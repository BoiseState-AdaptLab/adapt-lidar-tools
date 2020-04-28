
#include "spdlog/spdlog.h"

#include "PulseWavesProducer.hpp"

PulseWavesProducer::PulseWavesProducer(const std::string& fileName){
    bool failed = flightData.setFlightLineData(fileName);
    if(failed){
        spdlog::error("Bad file \"{}\"", fileName);
    }
}

PulseWavesProducer::~PulseWavesProducer(){
    flightData.closeFlightLineData();
    //@@TODO change flight line data to use destructor in the future.
}

const FlightLineData& PulseWavesProducer::getFlightLineData() const{
    return flightData;
}

void PulseWavesProducer::producePulse(PulseData& data){
    flightData.getNextPulse(data);
}

bool PulseWavesProducer::done() const{
    return !flightData.hasNextPulse();
}
