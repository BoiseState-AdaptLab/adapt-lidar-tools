/*
 * File name: WaveGPSInformation.cpp
 * Created on: 20-June-2017
 * Author: ravi
  * Modified by: Nicholas Chapa
 */

#include <iostream>
#include "WaveGPSInformation.hpp"

//Default constructor
WaveGPSInformation::WaveGPSInformation(){
  // enter default values
  xAnchor = 0;
  yAnchor = 0;
  zAnchor = 0;
  xTarget = 0;
  yTarget = 0;
  zTarget = 0;
  xFirst = 0;
  yFirst = 0;
  zFirst = 0;
  xLast = 0;
  yLast = 0;
  zLast = 0;
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
  xRealAnchor = 0;
  yRealAnchor = 0;
  zRealAnchor = 0;
  xRealTarget = 0;
  yRealTarget = 0;
  zRealTarget = 0;
  xRealFirst = 0;
  yRealFirst = 0;
  zRealFirst = 0;
  xRealLast = 0;
  yRealLast = 0;
  zRealLast = 0;
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
    xAnchor = pReader->pulse.get_anchor_x();
    yAnchor = pReader->pulse.get_anchor_y();
    zAnchor = pReader->pulse.get_anchor_z();
    xTarget = pReader->pulse.get_target_x();
    yTarget = pReader->pulse.get_target_y();
    zTarget = pReader->pulse.get_target_z();
    
    pReader->pulse.compute_first_and_last();
    xFirst = pReader->pulse.get_first_x();
    yFirst = pReader->pulse.get_first_y();
    zFirst = pReader->pulse.get_first_z();
    xLast = pReader->pulse.get_last_x();
    yLast = pReader->pulse.get_last_y();
    zLast = pReader->pulse.get_last_z();

    edge = pReader->pulse.edge_of_scan_line;
    scanDirection = pReader->pulse.scan_direction;
    facet = pReader->pulse.mirror_facet,
    intensity = pReader->pulse.intensity;
}

/*
 * Displays all GPS data
 */
void WaveGPSInformation::displayGPSData(){
  std::cout << xAnchor << std::endl;
  std::cout << zAnchor << std::endl;
  std::cout << xTarget << std::endl;
  std::cout << yTarget << std::endl;
  std::cout << zTarget << std::endl;
  std::cout << xFirst << std::endl;
  std::cout << yFirst << std::endl;
  std::cout << zFirst << std::endl;
  std::cout << xLast << std::endl;
  std::cout << yLast << std::endl;
  std::cout << zLast << std::endl;
  std::cout << edge << std::endl;
  std::cout << facet << std::endl;
  std::cout << scanDirection << std::endl;
  std::cout << intensity << std::endl;
}


void WaveGPSInformation::calculateTargetDeviations() {
    xDeviation = (xTarget - xAnchor) / 1000;
    yDeviation = (yTarget - yAnchor) / 1000;
    zDeviation = (zTarget - zAnchor) / 1000;
}
/*
//method of calculating real values of x, y, z discussed in 11/9 meeting
void WaveGPSInformation::calculateOurxyz() {
    our_x = xAnchor + (firstT + t) * xDeviation;
    our_y = yAnchor + (firstT + t) * yDeviation;
    our_z = zAnchor + (firstT + t) * zDeviation;
}

//method of calculating real x, y, z values as demonstrated in pdf from Nayani
//this method will calculate the real value of any xyz point passed with its respective scale and offset
void WaveGPSInformation::getRealPoint(pulseAxisPoint, axisScale, axisOffset) {
    realAxisPoint = (pulseAxisPoint * axisScale) + axisOffset;
    return realAxisPoint;
}

//method of calculating real x, y, z values as demonstrated in pdf from Nayani
//this method will calculate the real value of xyz anchor, target, first, last, min and max values
void WaveGPSInformation::calculateRealValues() {
    realTarget_x = (xTarget * xScale) + xOffset;
    realTarget_y = (yTarget * yScale) + yOffset;
    realTarget_z = (zTarget * zScale) + zOffset;
    
    realAnchor_x = (xAnchor * xScale) + xOffset;
    realAnchor_y = (yAnchor * yScale) + yOffset;
    realAnchor_z = (zAnchor * zScale) + zOffset;
    
    realMin_x = (xMin * xScale) + xOffset;
    realMin_y = (yMin * yScale) + yOffset;
    realMin_z = (zMin * zScale) + zOffset;
    
    realMax_x = (xMax * xScale) + xOffset;
    realMax_y = (yMax * yScale) + yOffset;
    realMax_z = (zMax * zScale) + zOffset;
    
    realFirst_x = (xFirst * xScale) + xOffset;
    realFirst_y = (yFirst * yScale) + yOffset;
    realFirst_z = (zFirst * zScale) + zOffset;
    
    realLast_x = (xLast * xScale) + xOffset;
    realLast_y = (yLast * yScale) + yOffset;
    realLast_z = (zLast * zScale) + zOffset;
}

}

//Idea for method
void WaveGPSInformation::getRealPeakCoordinate(x_axis, y_axis, z_axis, PULSEreader *pReader){
    xyzCoordinates.xCoordinate = getRealPoint(x_axis,pReader.get_scale_x, pReader.get_offset_x);
    xyzCoordinates.yCoordinate = getRealPoint(y_axis,pReader.get_scale_y, pReader.get_offset_y);
    xyzCoordinates.zCoordinate = getRealPoint(z_axis,pReader.get_scale_z, pReader.get_offset_z);
} **/
