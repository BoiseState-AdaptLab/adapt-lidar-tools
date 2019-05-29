/*
 * File name: PulseData.hpp
 * Created on: 12-July-2017
 * Author: ravi
 */

#ifndef PULSEDATA_HPP_
#define PULSEDATA_HPP_

#include <iostream>
#include <vector>

class PulseData{

    private:

    public:
        std::vector<int> outgoingIdx;
        std::vector<int> outgoingWave;
        std::vector<int> returningIdx;
        std::vector<int> returningWave;

        PulseData();
        void displayPulseData(std::ostream *outStream);
        void setOutgoing(std::vector<int> *idxArray,
                std::vector<int> *waveArray);
        void setReturning(std::vector<int> *idxArray,
                std::vector<int> *waveArray);
};

#endif /* PULSEDATA_HPP_ */

