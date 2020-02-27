#include <memory>

#include "GDALWriter.hpp"

GDALWriter::GDALWriter(){
    GDALAllRegister();
    driver = GetGDALDriverManager()->GetDriverByName("GTiff");
}

GDALWriter::~GDALWriter(){
    GDALDestroyDriverManager();
}

bool GDALWriter::writeData(const std::string& fileName, const std::string& fileDesc, const std::vector<std::vector<float>>& data, const std::string& geogCoordSys, int utm, double xMin, double yMax){
    //@@TODO verify data
    std::unique_ptr<GDALDataset, decltype(&GDALClose)> dataset
        (driver->Create(fileName.c_str(), data.size(), data.first().size(), 1, GDT_Float32, nullptr), &GDALClose);

    dataset->GetRasterBand(1)->SetNoDataValue(/*@@TODO*/9);
    dataset->GetRasterBand(1)->SetDescription(fileDesc.c_str());
    orientGDAL(*dataset, geogCoordSys, utm, xMin, yMax);
    for(std::size_t y = data.size()
}
