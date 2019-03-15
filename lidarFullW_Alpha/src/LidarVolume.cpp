// File name: LidarVolume.cpp
// Created on: 08-November-2017
// Author: ravi

#include "LidarVolume.hpp"


//Default constructor
LidarVolume::LidarVolume(){
  //initialize to zero
  bb_x_min = 0;
  bb_y_min = 0;
  bb_z_min = 0;
  bb_x_max = 0;
  bb_y_max = 0;
  bb_z_max = 0;

  bb_x_idx_min = 0;
  bb_y_idx_min = 0;
  bb_z_idx_min = 0;
  bb_x_idx_max = 0;
  bb_y_idx_max = 0;
  bb_z_idx_max = 0;

  max_z = 0;
  min_z = 0;

  x_idx_extent = 0;
  y_idx_extent = 0;

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

  bb_x_min = ld_xMin;
  bb_y_min = ld_yMin;
  bb_z_min = ld_zMin;
  bb_x_max = ld_xMax;
  bb_y_max = ld_yMax;
  bb_z_max = ld_zMax;

  bb_x_idx_min = 0;
  bb_y_idx_min = 0;
  bb_z_idx_min = 0;
  bb_x_idx_max = (int) (ceil(bb_x_max)) - (floor(bb_x_min));
  bb_y_idx_max = (int) (ceil(bb_y_max)) - (floor(bb_y_min));
  bb_z_idx_max = (int) (ceil(bb_z_max)) - (floor(bb_z_min));

  x_idx_extent = bb_x_idx_max - bb_x_idx_min + 1;
  y_idx_extent = bb_y_idx_max - bb_y_idx_min + 1;
}


//allocate memory with a 2d array of the data
void LidarVolume::allocateMemory(){
  // we are going to allocate a 2D array of space that will hold peak
  // information (we don't know how many per volume)
  unsigned int size = x_idx_extent*y_idx_extent;  //To prevent overflow during calloc
  int x_idx,y_idx;
  volume = (std::vector<Peak>**) malloc (sizeof(std::vector<Peak>*)*size);
  if(volume==NULL){
    perror("ERROR ATTEMPTING TO ALLOCATE LidarVolume Data: ");
  }
  // set all values to NULL (remember NULL is not guaranteed to be all 0
  // bits and so we can't just use calloc
  for(y_idx=0;y_idx<y_idx_extent;y_idx++){
    for(x_idx=0;x_idx<x_idx_extent;x_idx++){
      volume[position(y_idx,x_idx)] = NULL;
    }
  }
}


void LidarVolume::deallocateMemory(){
  delete volume;
  volume = NULL; //pointing dangling pointer to null
}


//j is most contiguous
//i is the least contiguous
int LidarVolume::position(int i, int j){
  return j + (i* x_idx_extent);
}

/* insert_peak
 * Note that this is an unordered list
 */
void LidarVolume::insert_peak(Peak *peak){
  unsigned int x_idx = gps_to_voxel_x(peak->x_activation);
  unsigned int y_idx = gps_to_voxel_y(peak->y_activation);

  // make sure we are in our bounding box
  if((int)x_idx > x_idx_extent || (int) y_idx > y_idx_extent){
    std::cerr << "ERROR: Invalid peak ignored" << std::endl;
    return;
  }
  unsigned long int p = position(y_idx,x_idx);

  if(volume[p] == NULL){
    volume[p] = new std::vector<Peak>();
  }
  volume[p]->push_back(*peak);
}


//Convert peak x, y and z values to
//i, j and k which identifies the voxel space they belong to
int LidarVolume::gps_to_voxel_x(double x){
  int voxel_x = (int)(x - bb_x_min);
  return voxel_x;
}

int LidarVolume::gps_to_voxel_y(double y){
  int voxel_y = (int)(y - bb_y_min);
  return voxel_y;
}


//Rasterize for max elevation
void LidarVolume::rasterizeMaxElevation(){

  std::cerr << "This function is not implemented" << std::endl;
  return;

  int i,j;

  for(i=bb_x_idx_min;i<bb_x_idx_max;i++){
    for(j=bb_y_idx_min;j<bb_y_idx_max;j++){
      raster[i*y_idx_extent] = -1;
      if(volume[position(j,i)] != NULL){
        raster[i*y_idx_extent] = j;

        //Save the max and mins of the max elevations
        if(raster[i*y_idx_extent] > elev_high){
          elev_high = j;
        }
        if(raster[i*y_idx_extent] < elev_low){
          elev_low = j;
        }
        std::cout << "Raster: " << raster[i*y_idx_extent] <<std::endl;
        break;
      }
    }
  }
}


//Rasterize for min elevation
void LidarVolume::rasterizeMinElevation(){

  std::cerr << "This function is not implemented" << std::endl;
  return;

  int i,j;

  for(i=bb_x_idx_min;i<bb_x_idx_max;i++){
    for(j=bb_y_idx_min;j<bb_y_idx_max;j++){
      raster[i*y_idx_extent] = -1;
      if(volume[position(j,i)] != NULL){
        raster[i*y_idx_extent] = j;
        //store the maz and mins of the min elevation
        if(raster[i*y_idx_extent] > elev_high){
          elev_high = j;
        }
        if(raster[i*y_idx_extent] < elev_low){
          elev_low = j;
        }
        std::cout << "Raster: " << raster[i*y_idx_extent] <<std::endl;
        break;
      }
    }
  }
}


void LidarVolume::display(){

  std::cerr << "This function is not implemented" << std::endl;
  return;
  
  int i,j;
  for(i=bb_x_idx_min;i<bb_x_idx_max;i++){
    for(j=bb_y_idx_min;j<bb_y_idx_max;j++){
      printf("%3d ",raster[i*y_idx_extent]);
    }
    printf("\n");
  }


  return;
}


// This function actually writes out the GEOTIF file.
void LidarVolume::writeImage(std::string outputFilename, bool maxElevationFlag, std::string geog_cs, int utm){

  //GDAL uses drivers to format all data sets so this registers the drivers
  GDALAllRegister();

  //Setup gdal datasets
  GDALDataset *newDS;

  //From raster
  //-1 because of zero indexing
  int nCols = x_idx_extent;
  int nRows = y_idx_extent;

  //FOR TESTING PURPOSES
  #ifdef DEBUG
    std::cerr << "nCols = x_idx_extent = " << nCols << std::endl;
    std::cerr << "nRows = y_idx_extent = " << nRows << std::endl;
  #endif

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

  newDS = driverTiff->Create(outputFilename.c_str(), x_idx_extent, y_idx_extent, 1,
                              GDT_Float32 , NULL);


  float noData = -99999.99;
  const float maxFloat = std::numeric_limits<float>::max();

  //encode the noData value as metadata in this band
  newDS->GetRasterBand(1)->SetNoDataValue(noData);

  //add a description to the band
  newDS  ->GetRasterBand(1)->SetDescription(maxElevationFlag ? "Max Elevation" : "Min Elevation");

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
  transform[0] = bb_x_min;
  transform[1] = 1;
  transform[2] = 0;
  transform[3] = bb_y_max;
  transform[4] = 0;
  transform[5] = -1;	//Always -1

  OGRSpatialReference oSRS;
  char *pszSRS_WKT = NULL;
  newDS->SetGeoTransform(transform);
  oSRS.SetUTM(utm, TRUE);
  oSRS.SetWellKnownGeogCS(geog_cs.c_str());
  oSRS.exportToWkt(&pszSRS_WKT);
  newDS->SetProjection(pszSRS_WKT);
  CPLFree(pszSRS_WKT);

  //float *heights = (float*)calloc(sizeof(float),y_idx_extent);
  float *heights = (float*)calloc(x_idx_extent, sizeof(float));

  #ifdef DEBUG
    std::cerr << "Mallocd heights. In "<< __FILE__ << ":" << __LINE__ << std::endl;
  #endif
  CPLErr retval;

  // Write image data
  int x, y;

  #ifdef DEBUG
    std::cerr << "Entering write image loop. In "<< __FILE__ << ":" << __LINE__ << std::endl;
  #endif

  bool max = maxElevationFlag;
  for (y=y_idx_extent-1; y>=0 ; y--) {
    for (x=0 ; x<x_idx_extent ; x++) {
        float maxZ = noData;
        float minZ = maxFloat;
      std::vector<Peak> *myPoints = volume[position(y, x)];
      if (myPoints != NULL) {
        for (std::vector<Peak>::iterator it = myPoints->begin();
             it != myPoints->end(); ++it) {
          //check if max or min we want
          if (max) {
            if (it->z_activation > maxZ) {
              maxZ = (float) it->z_activation;
            }
          } else {
            if (it->z_activation < minZ) {
              minZ = (float) it->z_activation;
            }
          }
          //std::cout << "maxZ " << maxZ << std::endl;
        }
      }
      heights[x] = max ? maxZ : (minZ == maxFloat ? noData:minZ );
      //std::cout<< "In x loop: Height[" << x <<"]= maxZ = " << maxZ << std::endl;
    }
    #ifdef DEBUG
      std::cerr << "In writeImage loop. Writing band: "<< x << "," << y << ". In " << __FILE__ << ":" << __LINE__ << std::endl;
    #endif

    // Refer to http://www.gdal.org/classGDALRasterBand.html
    retval = newDS->GetRasterBand(1)->RasterIO(GF_Write, 0, y_idx_extent-y-1, x_idx_extent,1,
                                                heights, x_idx_extent, 1, 
                                                GDT_Float32, 0, 0, NULL);
    #ifdef DEBUG
      std::cerr << "In writeImage loop. Writing band: "<< x << "," << y << ". In " << __FILE__ << ":" << __LINE__ << std::endl;
    #endif
    
    if(retval != CE_None){
        std::cerr << "Error during writing band: 1 "<< std::endl;
        std::cerr << x_idx_extent << " cols " << nCols << " ncols " << y_idx_extent << " rows " << nRows << " nRows" << std::endl;
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
  double normalized_z = (val - bb_z_idx_min) / (bb_z_idx_max - bb_z_idx_min);

  //invert and group
  double inverted_group=(1 - normalized_z)/0.25;

  //this is the integer part
  int integer_part=floor(inverted_group);

  //fractional_part part from 0 to 255
  int fractional_part=floor(255*(inverted_group - integer_part));

  // FOR TESTING PURPOSES
  // std::cout << "max k = " << bb_z_idx_max << std::endl;
  // std::cout << "min k = " << bb_z_idx_min << std::endl;
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


int LidarVolume::toTif(std::string outputFilename, bool maxElevationFlag, std::string geog_cs, int utm){
  writeImage(outputFilename, maxElevationFlag, geog_cs, utm);
  return 0;
}
