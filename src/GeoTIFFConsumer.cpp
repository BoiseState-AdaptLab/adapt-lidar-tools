#include <list>
#include <string>
#include <vector>

#include <gdal.h>

#include "spdlog/spdlog.h"

#include "GeoTIFFConsumer.hpp"
#include "GDALData.hpp"
#include "PeakProducts.hpp"

GeoTIFFConsumer::GeoTIFFConsumer(double minX, double maxX, double minY, double maxY, std::string fileNamePrefix, std::string coordSys, int utm, std::vector<PeakProducts::Product> products):
        volume_(minX, maxX, minY, maxY), fileNamePrefix_(fileNamePrefix), coordSys_(coordSys), utm_(utm), products_(products)
{}

void GeoTIFFConsumer::postProcess(){
    //Product a file for each product
    GDALAllRegister();
    GDALDriver* tiffDriver = GetGDALDriverManager()->GetDriverByName("GTiff");

    if(!tiffDriver){
        spdlog::critical("[GeoTIFFConsumer] Failed to create GDAL GTiff driver");
        return;
    }

    //@@TODO: I can make a version of this that calculates all products in one pass, but it would be significantly more complex and the code would be fairly complicated.

    std::vector<float> columnBuffer;
    const int bufferSize = volume_.ySize;
    columnBuffer.reserve(bufferSize);

    for(const PeakProducts::Product& product : products_){
        std::string fileName = fileNamePrefix_ + "_" + PeakProducts::productString(product) + ".tif";
        std::string fileDesc = "desc";  //@@TODO get from somewhere

        GDALData data(*tiffDriver, fileName, fileDesc, volume_.xSize, volume_.ySize);
        data.orient(coordSys_, utm_, volume_.xMin, volume_.yMax);

        //Loop through all peaks
        for(int x = 0; x < volume_.xSize; ++x){
            //Clear previous values, and fill with no data value
            columnBuffer.clear();
            columnBuffer.resize(bufferSize, GDAL_NO_DATA);

            for(int y = 0; y < volume_.ySize; ++y){
                const std::list<Peak>* peaks = volume_.getPeaks(x, y);
                if(peaks){  //Only update the buffer if we have peaks there
                    columnBuffer[volume_.ySize-1-y] = PeakProducts::produceProduct(*peaks, product);    //The volume_.ySize-1 is because it is inverted otherwise
                }
            }

            bool success = data.writeColumn(columnBuffer, x);
            if(!success){
                spdlog::error("[GeoTIFFConsumer] Failed to write column with offset of {} to file", x);
            }
        }
    }
}

void GeoTIFFConsumer::consumePeaks(const std::vector<Peak>& peaks, const PulseData&){
    for(const Peak& peak : peaks){
        volume_.insertPeak(peak);   //@@TODO: What to do if peak rejected?
    }
}
