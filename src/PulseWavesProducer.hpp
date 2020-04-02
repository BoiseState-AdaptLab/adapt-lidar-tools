#ifndef ADAPTLIDARTOOLS_PULSEWAVESPRODUCER_HPP
#define ADAPTLIDARTOOLS_PULSEWAVESPRODUCER_HPP

#include <string>
#include <vector>

#include "Peak.hpp"
#include "PulseData.hpp"

#include "FlightLineData.hpp"

class PulseWavesProducer{
public:
    PulseWavesProducer(const std::string& fileName);
    ~PulseWavesProducer();

    //PulseProducer requirements
    void producePulse(PulseData& data);
    bool done() const;

    //Returns true if producer is ready to use
    operator bool() const;

private:
    FlightLineData flightData;
    bool valid = true;
};

#endif // ADAPTLIDARTOOLS_PULSEWAVESPRODUCER_HPP
