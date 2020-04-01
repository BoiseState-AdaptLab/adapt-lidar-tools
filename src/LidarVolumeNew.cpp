#include <cmath>
#include <utility>

#include "spdlog/spdlog.h"

#include "LidarVolumeNew.hpp"
#include "Peak.hpp"

LidarVolumeNew::LidarVolumeNew(double minX, double maxX, double minY, double maxY) :
    xMin(static_cast<int>(std::round(minX))),   //@@TODO
    xMax(static_cast<int>(std::round(maxX))),
    yMin(static_cast<int>(std::round(minY))),
    yMax(static_cast<int>(std::round(maxY))),
    xSize(xMax-xMin),
    ySize(yMax-yMin)
{
    assert(maxX > minX);
    assert(maxY > minY);
    assert(xSize > 0);
    assert(ySize > 0);

    spdlog::info("xMin {}", xMin);
    spdlog::info("xMax {}", xMax);
    spdlog::info("yMin {}", yMin);
    spdlog::info("yMax {}", yMax);
}

bool LidarVolumeNew::insertPeak(const Peak& peak){
    std::pair<int, int> index = getIndex(peak);
    if(!indexValid(index)){
        spdlog::error("Peak outside bounding box ignored. {}, {} | {}, {}", peak.x_activation, peak.y_activation, peak.triggering_location, peak.amp);
        return false;
    }

    volume_[index.first][index.second].push_back(peak);
    return true;
}

const std::list<Peak>* LidarVolumeNew::getPeaks(int x, int y) const{
    if(volume_.count(x) == 0) return nullptr;
    if(volume_.at(x).count(y) == 0) return nullptr;
    return &volume_.at(x).at(y);
}

std::pair<int, int> LidarVolumeNew::getIndex(const Peak& peak) const{
    //@@TODO current version truncs. If we change this, change constructor
    //@@TODO do we want to check for NaN && infinity?
    int x = static_cast<int>(std::round(peak.x_activation));
    int y = static_cast<int>(std::round(peak.y_activation));
    return {x, y};
}

bool LidarVolumeNew::indexValid(std::pair<int, int> index) const{
    if(index.first  < xMin) return false;
    if(index.first  > xMax) return false;
    if(index.second < yMin) return false;
    if(index.second > yMax) return false;
    return true;
}
