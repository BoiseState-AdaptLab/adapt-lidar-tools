// File name: LidarVolume.cpp
// Created on: 08-November-2017
// Author: ravi

#include "LidarVolume.hpp"
#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <iostream>

//Default constructor
LidarVolume::LidarVolume(){
  //initialize to zero
  bb_x_min_padded = 0;
  bb_y_min_padded = 0;
  bb_z_min_padded = 0;
  bb_x_max_padded = 0;
  bb_y_max_padded = 0;
  bb_z_max_padded = 0;

  bb_i_min = 0;
  bb_j_min = 0;
  bb_k_min = 0;
  bb_i_max = 0;
  bb_j_max = 0;
  bb_k_max = 0;

  max_z = 0;
  min_z = 0;

  i_extent = 0;
  j_extent = 0;
  k_extent = 0;

  currentPeak = 0;
  numOfPeaks = 5;

  elev_high = -1;
  elev_low = 99999;

}

void LidarVolume::setBoundingBox(double ld_xMin, double ld_xMax,
                                 double ld_yMin, double ld_yMax,
                                 double ld_zMin, double ld_zMax){
  max_z = ld_zMax;
  min_z = ld_zMin;

  bb_x_min_padded = ld_xMin;
  bb_y_min_padded = ld_yMin;
  bb_z_min_padded = ld_zMin;
  bb_x_max_padded = ld_xMax;
  bb_y_max_padded = ld_yMax;
  bb_z_max_padded = ld_zMax;

  bb_i_min = 0;
  bb_j_min = 0;
  bb_k_min = 0;
  bb_i_max = (int) (ceil(bb_x_max_padded)) - (floor(bb_x_min_padded));
  bb_j_max = (int) (ceil(bb_y_max_padded)) - (floor(bb_y_min_padded));
  bb_k_max = (int) (ceil(bb_z_max_padded)) - (floor(bb_z_min_padded));

  i_extent = bb_i_max - bb_i_min + 1;
  j_extent = bb_j_max - bb_j_min + 1;
  k_extent = bb_k_max - bb_k_min + 1;
}


//allocate memory with a 3d array of the data
void LidarVolume::allocateMemory(){
  // we are going to allocate a 3D array of space that will hold peak
  // information (we don't know how many per volume)
  unsigned int size = i_extent*j_extent;  //To preven overflow during calloc
  volume = (std::vector<Peak>**) calloc (sizeof(std::vector<Peak>*), size);
  
  if(volume==NULL){
    perror("PERROR: ");
  }

  raster = (int*)calloc(sizeof(int),i_extent*j_extent);
}


void LidarVolume::deallocateMemory(){
  delete volume;
  volume = NULL; //pointing dangling pointer to null
}


//k is most contiguous
//i is the least contiguous
int LidarVolume::position(int i, int j){
  return j + (i* j_extent);
}


void LidarVolume::insert_peak(Peak *peak){
  unsigned int i = gps_to_voxel_x(peak->x_activation);
  unsigned int j = gps_to_voxel_y(peak->y_activation);

  //No need to check for i, j, k < 0 because they are unsigned ints
  if((int) i>i_extent || (int)j> j_extent){
    std::cerr << "ERROR: Invalid peak ignored\n";
    return;
  }
  unsigned long int p = position(i,j);

  if(volume[p] == NULL){
    volume[p] = new std::vector<Peak>();
  }
  volume[p]->push_back(*peak);  
}


//Convert peak x, y and z values to
//i, j and k which identifies the voxel space they belong to
int LidarVolume::gps_to_voxel_x(double x){
  int voxel_x = (x - bb_x_min_padded);
  return voxel_x;
}

int LidarVolume::gps_to_voxel_y(double y){
  int voxel_y = (y - bb_y_min_padded);
  return voxel_y;
}


//Rasterize for max elevation
void LidarVolume::rasterizeMaxElevation(){

  int i,j,k;

  for(i=bb_i_min;i<bb_i_max;i++){
    for(j=bb_j_min;j<bb_j_max;j++){
      raster[i*j_extent] = -1;
      if(volume[position(i,j)] != NULL){
        raster[i*j_extent] = j;

        //Save the max and mins of the max elevations
        if(raster[i*j_extent] > elev_high){
          elev_high = j;
        }
        if(raster[i*j_extent] < elev_low){
          elev_low = j;
        }
        std::cout << "Raster: " << raster[i*j_extent] <<std::endl;
        break;
      }
    }
  }
}


//Rasterize for min elevation
void LidarVolume::rasterizeMinElevation(){

  int i,j,k;

  for(i=bb_i_min;i<bb_i_max;i++){
    for(j=bb_j_min;j<bb_j_max;j++){
      raster[i*j_extent] = -1;
      if(volume[position(i,j)] != NULL){
        raster[i*j_extent] = j;
        //store the maz and mins of the min elevation
        if(raster[i*j_extent] > elev_high){
          elev_high = j;
        }
        if(raster[i*j_extent] < elev_low){
          elev_low = j;
        }
        std::cout << "Raster: " << raster[i*j_extent] <<std::endl;
        break;
      }
    }
  }
}


void LidarVolume::display(){

  int i,j;
  for(i=bb_i_min;i<bb_i_max;i++){
    for(j=bb_j_min;j<bb_j_max;j++){
      printf("%3d ",raster[i*j_extent]);
    }
    printf("\n");
  }


  return;
}


// This function actually writes out the PNG image file. The string 'title' is
// also written into the image file
void LidarVolume::writeImage(const char* filename, const char* title){

  //GDAL uses drivers to format all data sets so this registers the drivers
  GDALAllRegister();

  //Setup gdal datasets
  GDALDataset *newDS;

  //From raster
  //-1 because of zero indexing
  int nCols = j_extent;
  int nRows = i_extent;

  //FOR TESTING PURPOSES
  // std::cout << "nCols = i_extent = " << nCols << std::endl;
  // std::cout << "nRows = j_extent = " << nRows << std::endl;

  //Represents the output file format. This is used only to write data sets
  GDALDriver *driverTiff;

  driverTiff = GetGDALDriverManager()->GetDriverByName("GTiff");

  //To create a new dataset
  // Create(
  //      const char *pszFilename, //the name of the dataset to create
  //      int nXSize,              //width of created raster in pixels(cols)
  //      int nYSize,              //height of created raster in pixels(rows)
  //      int nBands,              //number of bands
  //      GDALDataType eType,      //type of raster
  //      char **   papszOptions   //driver specific control parameters
  //      )
  newDS = driverTiff->Create(filename, nCols, nRows, 1,GDT_UInt16 , NULL);

  double noData = -99999.9;

  //Used in transform
  double min_x = bb_x_min_padded;
  double max_y = bb_y_max_padded;

  //In a north up image, transform[1] is the pixel width, and transform[5] is
  //the pixel height. The upper left corner of the upper left pixel is at
  //position (transform[0],transform[3]).
  //  adfGeoTransform[0] /* top left x */
  //  adfGeoTransform[1] /* w-e pixel resolution */
  //  adfGeoTransform[2] /* 0 */
  //  adfGeoTransform[3] /* top left y */
  //  adfGeoTransform[4] /* 0 */
  //  adfGeoTransform[5] /* n-s pixel resolution (negative value) */

  double transform[6];
  transform[0] = min_x;
  transform[1] = 1;
  transform[2] = 0;
  transform[3] = max_y;
  transform[4] = 0;
  transform[5] = 1;

  OGRSpatialReference oSRS;
  char *pszSRS_WKT = NULL;
  newDS->SetGeoTransform(transform);
  oSRS.SetUTM(11, TRUE);
  oSRS.SetWellKnownGeogCS("NAD83");
  oSRS.exportToWkt(&pszSRS_WKT);
  newDS->SetProjection(pszSRS_WKT);
  CPLFree(pszSRS_WKT);


  unsigned char *r_row = (unsigned char*)calloc(sizeof(unsigned char),j_extent);
  unsigned char *g_row = (unsigned char*)calloc(sizeof(unsigned char),j_extent);
  unsigned char *b_row = (unsigned char*)calloc(sizeof(unsigned char),j_extent);
  //int* heights = (int*)calloc(sizeof(int),j_extent);
  GUInt16 *heights = (GUInt16*)calloc(sizeof(GUInt16),j_extent);


  CPLErr retval;

  // Write image data
  int x, y;
  for (y=0 ; y<i_extent ; y++) {
    for (x=0 ; x<j_extent ; x++) {
      // unsigned char r,g,b;
      // setRGB(&r,&g,&b,raster[y*j_extent + x]);
      heights[x] = raster[y*j_extent + x];
      // r_row[x] = r;
      // g_row[x] = g;
      // b_row[x] = b;
    }

    // Refer to http://www.gdal.org/classGDALRasterBand.html
    retval = newDS->GetRasterBand(1)->RasterIO(GF_Write, 0, y, nCols, 1,
                                       heights, nCols, 1, GDT_UInt16, 0, NULL);
    
    if(retval != CE_None){
        fprintf(stderr,"Error during writing band: 1\n");
        fprintf(stderr,"%d cols %d ncols %d rows %d nRows\n",j_extent,nCols,
                                                             i_extent,nRows);
    }
  }

  GDALClose((GDALDatasetH)newDS);
  GDALDestroyDriverManager();
}


void LidarVolume::setRGB(unsigned char* r,unsigned char* g, unsigned char* b, float val){

  *r = 255;
  *g = 255;
  *b = 255;

  if(val < 0 ){
    // use a special color
    *r=0;
    *g=0;
    *b=0;
    return;
  }
  double normalized_z = (val - bb_k_min) / (bb_k_max - bb_k_min);

  //invert and group
  double inverted_group=(1 - normalized_z)/0.25;

  //this is the integer part
  int integer_part=floor(inverted_group);

  //fractional_part part from 0 to 255
  int fractional_part=floor(255*(inverted_group - integer_part));

  // FOR TESTING PURPOSES
  // std::cout << "max k = " << bb_k_max << std::endl;
  // std::cout << "min k = " << bb_k_min << std::endl;
  // std::cout << "int val = " << val << std::endl;
  // std::cout << "Normalized z = " << normalized_z << std::endl;
  // std::cout << "Inverted group = " << inverted_group << std::endl;
  // std::cout << "Integer part = " << integer_part << std::endl;
  // std::cout << "Fractional part  = " << fractional_part << std::endl;

  switch(integer_part){
    case 0:
      *r=255;
      *g=fractional_part;
      *b=0;
      break;
    case 1:
      *r=255-fractional_part;
      *g=255;
      *b=0;
      break;
    case 2:
      *r=0;
      *g=255;
      *b=fractional_part;
      break;
    case 3:
      *r=0;
      *g=255-fractional_part;
      *b=255;
      break;
    case 4:
      *r=fractional_part;
      *g=0;
      *b=255;
      break;
    case 5:
      *r=255;
      *g=0;
      *b=255;
      break;
  }
}


int LidarVolume::toTif(std::string filename){
  writeImage(filename.c_str(), "This is a super fun test");
  return 0;
}
