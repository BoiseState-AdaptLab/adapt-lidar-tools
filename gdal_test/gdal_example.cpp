#include "gdal.h"
#include "gdal_priv.h"
#include <iostream>

int main(){

  const char *input = "image.tif";
  const char *output = "test.tif";

  //GDAL uses drivers to format all data sets so this registers the drivers
  GDALAllRegister();  

  //Setup gdal datasets
  GDALDataset *newDS, *oldDS;

  //Stores these details from the existing tif dataset
  int nRows, nCols;
  double noData;

  //In a north up image, transform[1] is the pixel width, and transform[5] is 
  //the pixel height. The upper left corner of the upper left pixel is at 
  //position (transform[0],transform[3]).
  double transform[6];

  //Open the existing file you want to copy
  oldDS = (GDALDataset*) GDALOpen(input, GA_ReadOnly);
  
  //Get the no of rows and cols from the existing file
  nCols = oldDS->GetRasterBand(1)->GetXSize();
  nRows = oldDS->GetRasterBand(1)->GetYSize();
  noData = oldDS->GetRasterBand(1)->GetNoDataValue();

  //Get top left corner coords, cell width and height and any offset info
  //6 element array
  oldDS->GetGeoTransform(transform);

  //Print all data to check
  std::cout << "Cols: " <<nCols<< " Rows: "<<nRows<< std::endl;
  std::cout << "NoData : "<<noData<< std::endl;
  std::cout << "Width: " <<transform[1]<< " Height: "<<transform[5]<< std::endl;
  std::cout << "X: " <<transform[0]<< " Y: "<<transform[3]<< std::endl;

  //Represents the output file format. This is used only to write data sets
  GDALDriver *driverTiff;

  driverTiff = GetGDALDriverManager()->GetDriverByName("GTiff");



  //Create a copy of the old file - The driver comes into play here!
  //newDS = DriverTiff->CreateCopy(output, oldDS, FALSE, NULL, NULL, NULL);

  //To create a new dataset
  // Create (const char *pszFilename, //the name of the dataset to create
  //         int nXSize,              //width of created raster in pixels(cols)
  //         int nYSize,              //height of created raster in pixels(rows)
  //         int nBands,              //number of bands
  //         GDALDataType eType,      //type of raster
  //         char **   papszOptions   //driver specific control parameters
  //        )
  newDS = driverTiff->Create(output, nCols, nRows, 1, oldDS->GetRasterBand(1)->GetRasterDataType(), NULL);

  float *oldRow = (float*) CPLMalloc(sizeof(float)*nCols);
  float *newRow = (float*) CPLMalloc(sizeof(float)*nCols);
  
  
  for(int i =0; i<nRows; i++){
    //Read an entire row from the raster into memory and loop through each column
    CPLErr retval = oldDS->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, oldRow, nCols, 1, oldDS->GetRasterBand(1)->GetRasterDataType(), 0, 0);
    if(retval != CE_None){
      fprintf(stderr,"Error during reading\n");
      return 0;
    }
    for(int j =0; j<nCols; j++){

      if(oldRow[j] == noData){
        newRow[j] = noData;
      }
      else{
        newRow[j] = oldRow[j] + 10;
      }
    }
    retval = newDS->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, newRow, nCols, 1, oldDS->GetRasterBand(1)->GetRasterDataType(), 0, NULL);
    if(retval != CE_None){
      fprintf(stderr,"Error during reading\n");
      return 0;
    }
  }


  //GDALClose(oldsDS);
  GDALClose(newDS);
  GDALDestroyDriverManager();

  return 0;

}