#ifndef ADAPTLIDARTOOLS_GDALWRITER_HPP
#define ADAPTLIDARTOOLS_GDALWRITER_HPP

#include "gdal.h"
#include "gdal_priv.h"

class GDALWriter{
public:
    GDALWriter();
    ~GDALWriter();
    bool writeData(const std::string& fileName, const std::string& fileDesc, const std::vector<std::vector<float>>& data, const std::string& geogCoordSys, int utm, double xMin, double yMax);

private:
    void orientGDAL(GDALDataset& dataset, const std::string& coordSys, int utm, double xMin, double yMax);
    GDALDriver* driver = nullptr;
};
#endif // ADAPTLIDARTOOLS_GDALWRITER_HPP
