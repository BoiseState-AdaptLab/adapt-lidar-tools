#ifndef ADAPTLIDARTOOLS_GEOTIFFCONSUMER_HPP
#define ADAPTLIDARTOOLS_GEOTIFFCONSUMER_HPP

#include <tuple>
#include <vector>

#include "LidarVolumeNew.hpp"
#include "Peak.hpp"
#include "PeakProducts.hpp"
#include "PulseData.hpp"

class GeoTIFFConsumer{
public:
    GeoTIFFConsumer(double minX, double maxX, double minY, double maxY, std::string fileNamePrefix, std::string coordSy, int utm, std::vector<PeakProducts::Product> products);
    void consumePeaks(const std::vector<Peak>& peaks, const PulseData&);
    void postProcess();

private:
    LidarVolumeNew volume_;
    std::string fileNamePrefix_;
    std::string coordSys_;
    int utm_=0;
    std::vector<PeakProducts::Product> products_;
};
#endif // ADAPTLIDARTOOLS_GEOTIFFCONSUMER_HPP
