// File name: LidarVolume.cpp
// Created on: 08-November-2017
// Author: ravi

#include "LidarVolume.hpp"
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
  //peakData = NULL;
}

void LidarVolume::setBoundingBox(double ld_xMin, double ld_xMax,
                                 double ld_yMin, double ld_yMax,
                                 double ld_zMin, double ld_zMax){
  max_z = ld_zMax;
  min_z = ld_zMin;

  bb_x_min_padded = ld_xMin - 10;
  bb_y_min_padded = ld_yMin - 10;
  bb_z_min_padded = ld_zMin - 10;
  bb_x_max_padded = ld_xMax + 10;
  bb_y_max_padded = ld_yMax + 10;
  bb_z_max_padded = ld_zMax + 10;

  std::cout << "bb_z_min = "<< bb_z_min_padded << std::endl;
  std::cout << "bb_z_max = "<< bb_z_max_padded << std::endl;

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
  volume = (std::vector<Peak>**) calloc (sizeof(std::vector<Peak>*),
                                         i_extent * j_extent * k_extent );

  raster = (int*)calloc(sizeof(int),i_extent*j_extent);
}


void LidarVolume::deallocateMemory(){
  delete volume;
  volume = NULL; //pointing dangling pointer to null
}


//k is most contiguous
//i is the least contiguous
int LidarVolume::position(int i, int j, int k){
  return k + (j* k_extent) + (i*k_extent*j_extent);
}


void LidarVolume::insert_peak(Peak *peak){
  unsigned int i = gps_to_voxel_x(peak->x_activation);
  unsigned int j = gps_to_voxel_y(peak->y_activation);
  unsigned int k = gps_to_voxel_z(peak->z_activation);

  //No need to check for i, j, k < 0 because they are unsigned ints
  if((int) i>i_extent || (int)j> j_extent || (int)k > k_extent){
    std::cerr << "ERROR: Invalid peak ignored\n";
    return;
  }
  unsigned long int p = position(i,j,k);

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

int LidarVolume::gps_to_voxel_z(double z){
  int voxel_z = (z - bb_z_min_padded);
  return voxel_z;
}


void LidarVolume::rasterize(){

  int i,j,k;

  for(i=bb_i_min;i<bb_i_max;i++){
    for(j=bb_j_min;j<bb_j_max;j++){
      raster[i*j_extent+j] = 0;
      for(k=bb_k_max-1;k>=bb_k_min;k--){
        if(volume[position(i,j,k)] != NULL){
          raster[i*j_extent+j] = k;
          break;
        }
      }
    }
  }
}

void LidarVolume::display(){

  int i,j;
  for(i=bb_i_min;i<bb_i_max;i++){
    for(j=bb_j_min;j<bb_j_max;j++){
      printf("%3d ",raster[i*j_extent+j]);
    }
    printf("\n");
  }


  return;
}

// This function actually writes out the PNG image file. The string 'title' is
// also written into the image file
int LidarVolume::writeImage(const char* filename, const char* title){

  int code = 0;
  FILE *fp = NULL;
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  png_bytep row = NULL;

  // Open file for writing (binary mode)
  fp = fopen(filename, "wb");
  // TODO: fixme: this should not print or do a goto
  if (fp == NULL) {
      fprintf(stderr, "Could not open file %s for writing\n", filename);
      code = 1;
      goto finalise;
  }

  // Initialize write structure
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  // TODO: fixme: this should not print or do a goto
  if (png_ptr == NULL) {
      fprintf(stderr, "Could not allocate write struct\n");
      code = 1;
      goto finalise;
  }

  // Initialize info structure
  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL) {
      fprintf(stderr, "Could not allocate info struct\n");
      code = 1;
      goto finalise;
  }

  // Setup Exception handling
  if (setjmp(png_jmpbuf(png_ptr))) {
      fprintf(stderr, "Error during png creation\n");
      code = 1;
      goto finalise;
  }

  png_init_io(png_ptr, fp);

  // Write header (8 bit colour depth)
  png_set_IHDR(png_ptr, info_ptr, j_extent, i_extent,
            8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Set title
  if (title != NULL) {
      png_text title_text;
      title_text.compression = PNG_TEXT_COMPRESSION_NONE;
      title_text.key = "Title";
      char* temp = (char*)malloc (strlen(title));
      strcpy (temp, title);
      title_text.text = temp;
      png_set_text(png_ptr, info_ptr, &title_text, 1);
  }

  png_write_info(png_ptr, info_ptr);

  // Allocate memory for one row (3 bytes per pixel - RGB)
  row = (png_bytep) calloc(sizeof(png_byte),3 * j_extent);

  // Write image data
  int x, y;
  for (y=0 ; y<i_extent ; y++) {
    for (x=0 ; x<j_extent ; x++) {
      setRGB(&(row[x*3]), raster[y*j_extent + x]);
    }
    png_write_row(png_ptr, row);
  }

  // End write
  png_write_end(png_ptr, NULL);

  finalise:
  if (fp != NULL) fclose(fp);
  if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
  if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
  if (row != NULL) free(row);

  return code;

}

void LidarVolume::setRGB(png_byte *ptr, float val){

  ptr[0] = 255;
  ptr[1] = 255;
  ptr[2] = 255;

  //std::cout << "max z = " << max_z << std::endl;
  //std::cout << "min z = " << min_z << std::endl;
  std::cout << "float val = " << val << std::endl;
  double normalized_z = 2*((val - min_z) / (max_z - min_z)) - 1;
  //std::cout << "Normalized z = " << normalized_z << std::endl;
  double inverted_group=(1 - normalized_z)/0.25;       //invert and group
  //std::cout << "Inverted group = " << inverted_group << std::endl;
  int integer_part=floor(inverted_group); //this is the integer part
  //std::cout << "Integer part = " << integer_part << std::endl;
  //fractional_part part from 0 to 255
  int fractional_part=floor(255*(inverted_group - integer_part));
  //std::cout << "Fractional part  = " << fractional_part << std::endl;

  switch(integer_part)
  {
    case 0:
      ptr[0]=255;
      ptr[1]=fractional_part;
      ptr[2]=0;
      break;
    case 1:
      ptr[0]=255-fractional_part;
      ptr[1]=255;
      ptr[2]=0;
      break;
    case 2:
      ptr[0]=0;
      ptr[1]=255;
      ptr[2]=fractional_part;
      break;
    case 3:
      ptr[0]=0;
      ptr[1]=255-fractional_part;
      ptr[2]=255;
      break;
    case 4:
      ptr[0]=0;
      ptr[1]=0;
      ptr[2]=255;
      break;
  }
}

int LidarVolume::toPng(std::string filename){
  rasterize();
  writeImage(filename.c_str(), "This is a super fun test");
  return 0;
}
