#ifndef ADAPTLIDARTOOLS_GDALDATA_HPP
#define ADAPTLIDARTOOLS_GDALDATA_HPP

#include <memory>
#include <string>
#include <vector>

#include <gdal.h>
#include <gdal_priv.h>

constexpr double GDAL_NO_DATA = -99999;

class GDALData{
public:
    GDALData(GDALDriver& driver, const std::string& fileName, const std::string& fileDesc, int xSize, int ySize);
    void orient(const std::string& coordSys, int utm, double xMin, double yMax);

    //@@TODO: The old code in LidarDriver.cpp +372 claimed it was writing columns, but it looked like it was writing rows. This code actually writes columns
    bool writeColumn(const std::vector<float>& column, int columnOffset);

private:
    std::unique_ptr<GDALDataset> data_;
    const int xSize_=0;
    const int ySize_=0;
};
#endif // ADAPTLIDARTOOLS_GDALDATA_HPP
