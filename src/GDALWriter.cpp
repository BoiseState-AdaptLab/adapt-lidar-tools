#include <array>
#include <string>
#include <vector>

#include <gdal.h>
#include <gdal_priv.h>

#include "spdlog/spdlog.h"

#include "GDALWriter.hpp"

GDALWriter::GDALWriter(GDALDriver& driver, const std::string& fileName, const std::string& fileDesc, int xSize, int ySize):
        data_{driver.Create(fileName.c_str(), xSize, ySize, 1, GDT_Float32, nullptr)}, xSize_{xSize}, ySize_{ySize}
{
    data_->GetRasterBand(1)->SetNoDataValue(GDAL_NO_DATA);
    data_->GetRasterBand(1)->SetDescription(fileDesc.c_str());
}

void GDALWriter::orient(const std::string& coordSys, int utm, double xMin, double yMax){
    std::array<double, 6> transform{xMin, 1, 0, yMax, 0, -1};
    data_->SetGeoTransform(transform.data());

    OGRSpatialReference refSys;
    refSys.SetUTM(utm);
    refSys.SetWellKnownGeogCS(coordSys.c_str());

    char* wktStr = nullptr;
    refSys.exportToWkt(&wktStr);
    data_->SetProjection(wktStr);
    CPLFree(wktStr);
}

bool GDALWriter::writeColumn(const std::vector<float>& column, int columnOffset){
    if(static_cast<int>(column.size()) != ySize_){
        spdlog::critical("[GDAL] Tried to write column with invalid size");
        return false;
    }

    if(columnOffset >= xSize_ || columnOffset < 0){
        spdlog::critical("[GDAL] Tried to write column with invalid offset");
        return false;
    }

    //@@TODO https://gdal.org/api/gdalrasterband_cpp.html#_CPPv4N14GDALRasterBand8RasterIOE10GDALRWFlagiiiiPvii12GDALDataType8GSpacing8GSpacingP20GDALRasterIOExtraArg
    bool success = CPLErr::CE_None == data_->GetRasterBand(1)->RasterIO(
            GDALRWFlag::GF_Write,               //Write data
            columnOffset,                       //X offset (start at top left)
            0,                                  //Y offset (start at top)   @@TODO: Check if problems
            1,                                  //X Size
            column.size(),                      //Y Size
            const_cast<float*>(column.data()),  //Data
            1,                                  //X Buffer size
            column.size(),                      //Y Buffer size
            GDT_Float32,                        //Data type (float)
            0,                                  //Byte offset (default)
            0,                                  //Byte offset (default)
            nullptr);                           //Additional args (default)
    return success;
}
