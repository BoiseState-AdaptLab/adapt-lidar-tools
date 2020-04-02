#ifndef ADAPTLIDARTOOLS_LIDARVOLUMENEW
#define ADAPTLIDARTOOLS_LIDARVOLUMENEW

#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "Peak.hpp"

class LidarVolumeNew{
public:
    LidarVolumeNew(double minX, double maxX, double minY, double maxY);

    bool insertPeak(const Peak& peak);

    const std::list<Peak>* getPeaks(int x, int y) const;

    const int xMin=0;
    const int xMax=0;
    const int yMin=0;
    const int yMax=0;

    const int xSize=0;
    const int ySize=0;
private:
    std::pair<int, int> getIndex(const Peak& peak) const;

    bool indexValid(std::pair<int, int> index) const;

    int resolveIndex(std::pair<int, int> index) const;

    //[x][y] = (x*ySize + y)
    std::vector<std::unique_ptr<std::list<Peak>>> volume_;
};
#endif // ADAPTLIDARTOOLS_LIDARVOLUMENEW
