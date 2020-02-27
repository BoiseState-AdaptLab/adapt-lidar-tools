#ifndef ADAPTLIDARTOOLS_GEOTIFFCONSUMER_HPP
#define ADAPTLIDARTOOLS_GEOTIFFCONSUMER_HPP

#include <vector>

#include "Peak.hpp"
#include "PulseData.hpp"

class GeoTIFFConsumer{
public:
    void consumePeaks(const std::vector<Peak>& peaks, const PulseData& data);
    void postProcess();

private:
};
#endif // ADAPTLIDARTOOLS_GEOTIFFCONSUMER_HPP
