#ifndef ADAPTLIDARTOOLS_PULSEWAVESPRODUCER_HPP
#define ADAPTLIDARTOOLS_PULSEWAVESPRODUCER_HPP

#include <string>
#include <vector>

#include "Peak.hpp"
#include "PulseData.hpp"

#include "FlightLineData.hpp"

class PulseWavesProducer{
public:
    /** Constructs a new pulsewaves producer.
     * @param fileName      The .pls file to open.
     */
    PulseWavesProducer(const std::string& fileName);

    /** Destructor, closes FlightLineData.
     */
    ~PulseWavesProducer();
    
    /** Get a read-only reference to the FlightLineData object.
     * @return  A read only ref to the underlying FlightLineData object.
     */
    const FlightLineData& getFlightLineData() const;




    //PulseProducer requirements, see Common.hpp for docs
    void producePulse(PulseData& data);
    bool done() const;
private:
    FlightLineData flightData;
};

#endif // ADAPTLIDARTOOLS_PULSEWAVESPRODUCER_HPP
