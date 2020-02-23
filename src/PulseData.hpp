#ifndef ADAPTLIDARTOOLS_PULSEDATA_HPP
#define ADAPTLIDARTOOLS_PULSEDATA_HPP

#include <vector>

struct PulseData{
    std::vector<int> outgoingIndex;
    std::vector<int> outgoingAmplitude;

    std::vector<int> returningIndex;
    std::vector<int> returningAmplitude;
};

#endif // ADAPTLIDARTOOLS_PULSEDATA_HPP

