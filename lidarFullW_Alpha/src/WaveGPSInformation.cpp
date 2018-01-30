// File name: WaveGPSInformation.cpp
// Created on: 20-June-2017
// Author: ravi

#include <iostream>
#include "WaveGPSInformation.hpp"

//Default constructor
WaveGPSInformation::WaveGPSInformation(){
  // enter default values
  x_anchor = 0;
  y_anchor = 0;
  z_anchor = 0;
  
  x_target = 0;
  y_target = 0;
  z_target = 0;
  
  dx = 0;
  dy = 0;
  dz = 0;
  
  x_first = 0;
  y_first = 0;
  z_first = 0;
  
  x_last = 0;
  y_last = 0;
  z_last = 0;
  
  edge = 0;
  facet = 0;
  scanDirection = 0;
  intensity = 0;
  
  xOffset = 0;
  yOffset = 0;
  zOffset = 0;
  xScale = 0;
  yScale = 0;
  zScale = 0;
  xReal_anchor = 0;
  yReal_anchor = 0;
  zReal_anchor = 0;
  xReal_target = 0;
  yReal_target = 0;
  zReal_target = 0;
  xReal_first = 0;
  yReal_first = 0;
  zReal_first = 0;
  xReal_last = 0;
  yReal_last = 0;
  zReal_last = 0;
  xRealMin = 0;
  yRealMin = 0;
  zRealMin = 0;
  xRealMax = 0;
  yRealMax = 0;
  zRealMax = 0;
  
  //xyzCoordinates.xCoordinate = 0;
  //xyzCoordinates.yCoordinate = 0;
  //xyzCoordinates.zCoordinate = 0;
}


 //Populate all the GPS data
void WaveGPSInformation::populateGPS(PULSEreader *pReader){
  
    gpsTime = pReader->pulse.get_t();
    
    //xScale = pReader->header.x_scale_factor;
    //yScale = pReader->header.x_scale_factor;
    //zScale = pReader->header.x_scale_factor;
    //xOffset = pReader->header.x_offset;
    //yOffset = pReader->header.x_offset;
    //zOffset = pReader->header.x_offset;
    
    pReader->pulse.compute_anchor_and_target_and_dir();
    x_anchor = pReader->pulse.get_anchor_x();
    y_anchor = pReader->pulse.get_anchor_y();
    z_anchor = pReader->pulse.get_anchor_z();
    x_target = pReader->pulse.get_target_x();
    y_target = pReader->pulse.get_target_y();
    z_target = pReader->pulse.get_target_z();

    dx = (x_target - x_anchor) / 1000;
    dy = (y_target - y_anchor) / 1000;
    dz = (z_target - z_anchor) / 1000;
    
    pReader->pulse.compute_first_and_last();
    x_first = pReader->pulse.get_first_x();
    y_first = pReader->pulse.get_first_y();
    z_first = pReader->pulse.get_first_z();
    x_last = pReader->pulse.get_last_x();
    y_last = pReader->pulse.get_last_y();
    z_last = pReader->pulse.get_last_z();

    edge = pReader->pulse.edge_of_scan_line;
    scanDirection = pReader->pulse.scan_direction;
    facet = pReader->pulse.mirror_facet,
    intensity = pReader->pulse.intensity;
}

/*
 * Displays all GPS data
 */
void WaveGPSInformation::displayGPSData(){
  std::cout << x_anchor << std::endl;
  std::cout << z_anchor << std::endl;
  std::cout << x_target << std::endl;
  std::cout << y_target << std::endl;
  std::cout << z_target << std::endl;
  std::cout << x_first << std::endl;
  std::cout << y_first << std::endl;
  std::cout << z_first << std::endl;
  std::cout << x_last << std::endl;
  std::cout << y_last << std::endl;
  std::cout << z_last << std::endl;
  std::cout << edge << std::endl;
  std::cout << facet << std::endl;
  std::cout << scanDirection << std::endl;
  std::cout << intensity << std::endl;
}


/*
//method of calculating real values of x, y, z discussed in 11/9 meeting
void WaveGPSInformation::calculateOurxyz() {
    our_x = x_anchor + (firstT + t) * xDeviation;
    our_y = y_anchor + (firstT + t) * yDeviation;
    our_z = z_anchor + (firstT + t) * zDeviation;
}

//method of calculating real x, y, z values as demonstrated in pdf from Nayani
//this method will calculate the real value of any xyz point passed with its respective scale and offset
void WaveGPSInformation::getRealPoint(pulseAxisPoint, axisScale, axisOffset) {
    realAxisPoint = (pulseAxisPoint * axisScale) + axisOffset;
    return realAxisPoint;
}

//method of calculating real x, y, z values as demonstrated in pdf from Nayani
//this method will calculate the real value of xyz _anchor, target, first, last, min and max values
void WaveGPSInformation::calculateRealValues() {
    real_target_x = (x_target * xScale) + xOffset;
    real_target_y = (y_target * yScale) + yOffset;
    real_target_z = (z_target * zScale) + zOffset;
    
    real_anchor_x = (x_anchor * xScale) + xOffset;
    real_anchor_y = (y_anchor * yScale) + yOffset;
    real_anchor_z = (z_anchor * zScale) + zOffset;
    
    realMin_x = (xMin * xScale) + xOffset;
    realMin_y = (yMin * yScale) + yOffset;
    realMin_z = (zMin * zScale) + zOffset;
    
    realMax_x = (xMax * xScale) + xOffset;
    realMax_y = (yMax * yScale) + yOffset;
    realMax_z = (zMax * zScale) + zOffset;
    
    real_first_x = (x_first * xScale) + xOffset;
    real_first_y = (y_first * yScale) + yOffset;
    real_first_z = (z_first * zScale) + zOffset;
    
    real_last_x = (x_last * xScale) + xOffset;
    real_last_y = (y_last * yScale) + yOffset;
    real_last_z = (z_last * zScale) + zOffset;
}

}

//Idea for method
void WaveGPSInformation::getRealPeakCoordinate(x_axis, y_axis, z_axis, PULSEreader *pReader){
    xyzCoordinates.xCoordinate = getRealPoint(x_axis,pReader.get_scale_x, pReader.get_offset_x);
    xyzCoordinates.yCoordinate = getRealPoint(y_axis,pReader.get_scale_y, pReader.get_offset_y);
    xyzCoordinates.zCoordinate = getRealPoint(z_axis,pReader.get_scale_z, pReader.get_offset_z);
} **/
