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


/**
 * allocate memory with a 2d array of the data
 */
void LidarVolume::allocateMemory(){
    // we are going to allocate a 2D array of space that will hold peak
    // information (we don't know how many per volume)
    unsigned int size = x_idx_extent*y_idx_extent;  //To prevent overflow during
                                                    //calloc
    int x_idx,y_idx;
    volume = (std::vector<Peak*>**) malloc (sizeof(std::vector<Peak*>*)*size);
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

/**
 * clean up and deallocate resources
 */
void LidarVolume::deallocateMemory(){
    int x_idx,y_idx;
    for(y_idx=0;y_idx<y_idx_extent;y_idx++){
        for(x_idx=0;x_idx<x_idx_extent;x_idx++){
            if(volume[position(y_idx,x_idx)] != NULL){
                volume[position(y_idx,x_idx)]->clear();
                delete(volume[position(y_idx,x_idx)]);
            }
        }
    }
    free(volume);
    volume = NULL; //pointing dangling pointer to null
}


/**
 *
 * @param i most contiguous
 * @param j least contiguous
 * @return
 */
int LidarVolume::position(int i, int j){
    return j + (i* x_idx_extent);
}

/**
 * insert_peak Note that this is an unordered list
 * @param peak
 */
void LidarVolume::insert_peak(Peak* peak){
    unsigned int x_idx = gps_to_voxel_x(peak->x_activation);
    unsigned int y_idx = gps_to_voxel_y(peak->y_activation);

    // make sure we are in our bounding box
    if((long int)x_idx > x_idx_extent || (long int)y_idx > y_idx_extent){
        std::cerr << "ERROR: Invalid peak ignored" << std::endl;
        return;
    }
    unsigned long int p = position(y_idx,x_idx);

    if(volume[p] == NULL){
        volume[p] = new std::vector<Peak*>();
    }
    volume[p]->push_back(peak); 
}


/**
 * Convert peak x, y and z values to
 * i, j and k which identifies the voxel space they belong to
 * @param x
 * @return
 */
//TODO: Why ints? do we risk overflow/underflow?
int LidarVolume::gps_to_voxel_x(double x){
    int voxel_x = (int)(x - bb_x_min);
    return voxel_x;
}


/**
 * Convert peak x, y and z values to
 * i, j and k which identifies the voxel space they belong to
 * @param y
 * @return
 */
//TODO: Why ints? do we risk overflow/underflow?
int LidarVolume::gps_to_voxel_y(double y){
    int voxel_y = (int)(y - bb_y_min);
    return voxel_y;
}

/**
 * setRGB for heatmap colorization (unused)
 * @param r
 * @param g
 * @param b
 * @param val
 */
void LidarVolume::setRGB(unsigned char* r,unsigned char* g, unsigned char* b,
                        float val)
{
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
    // std::cout << "Fractional part    = " << fractional_part << std::endl;

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


