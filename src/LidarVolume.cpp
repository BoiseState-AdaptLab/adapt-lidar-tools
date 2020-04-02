#include <cmath>
#include <list>
#include <utility>
#include <vector>

#include "spdlog/spdlog.h"

#include "LidarVolume.hpp"
#include "Peak.hpp"

LidarVolume::LidarVolume(double minX, double maxX, double minY, double maxY) :
    xMin(static_cast<int>(std::round(minX))),   //@@TODO is rounding correct?
    xMax(static_cast<int>(std::round(maxX))),
    yMin(static_cast<int>(std::round(minY))),
    yMax(static_cast<int>(std::round(maxY))),
    xSize(1+xMax-xMin),
    ySize(1+yMax-yMin)
{
    assert(maxX > minX);
    assert(maxY > minY);
    assert(xSize > 0);
    assert(ySize > 0);

    spdlog::info("[LidarVolume] xMin {}", xMin);
    spdlog::info("[LidarVolume] xMax {}", xMax);
    spdlog::info("[LidarVolume] yMin {}", yMin);
    spdlog::info("[LidarVolume] yMax {}", yMax);
    spdlog::info("[LidarVolume] xSize {}", xSize);
    spdlog::info("[LidarVolume] ySize {}", ySize);
    spdlog::info("[LidarVolume] Total table size {}", xSize*ySize);

    volume_.resize(xSize*ySize);
}

bool LidarVolume::insertPeak(const Peak& peak){
    std::pair<int, int> index = getIndex(peak);
    if(!indexValid(index)){
        spdlog::error("[LidarVolume] Peak outside bounding box ignored. {}, {} | {}, {}", peak.x_activation, peak.y_activation, peak.triggering_location, peak.amp);
        return false;
    }

    int vecIndex = resolveIndex(index);
    if(!volume_.at(vecIndex)){
        volume_.at(vecIndex) = std::unique_ptr<std::list<Peak>>(new std::list<Peak>());
    }

    volume_.at(vecIndex)->push_back(peak);

    return true;
}

const std::list<Peak>* LidarVolume::getPeaks(int x, int y) const{
    if(!indexValid({x, y})) return nullptr;

    int vecIndex = resolveIndex({x, y});

    if(!volume_.at(vecIndex)) return nullptr;
    return volume_.at(vecIndex).get();
}

std::pair<int, int> LidarVolume::getIndex(const Peak& peak) const{
    //@@TODO current version truncs. If we change this, change constructor
    //@@TODO do we want to check for NaN && infinity?
    int x = static_cast<int>(std::round(peak.x_activation)) - xMin;
    int y = static_cast<int>(std::round(peak.y_activation)) - yMin;
    return {x, y};
}

bool LidarVolume::indexValid(std::pair<int, int> index) const{
    if(index.first  < 0) return false;
    if(index.first  > xMax-xMin) return false;
    if(index.second < 0) return false;
    if(index.second > yMax-yMin) return false;
    assert(resolveIndex(index) < static_cast<int>(volume_.size()));
    return true;
}

int LidarVolume::resolveIndex(std::pair<int, int> index) const{
    return index.first*ySize+index.second;
}
