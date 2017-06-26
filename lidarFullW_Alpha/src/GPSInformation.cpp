/*
 * File name: ScannerInformation.cpp
 * Created on: 20-June-2017
 * Author: ravi
 */

#include <iostream>
#include "GPSInformation.hpp"

//Default constructor
GPSInformation::GPSInformation(){
  // enter default values
  noOfPulses = 0;
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
}


void GPSInformation::setGPSInformation(){

  gpsTime = pReader->pulse.get_t();
  // Compute anchor, target and direction
  pReader->pulse.compute_anchor_and_target_and_dir();
  xAnchor = pReader->pulse.get_anchor_x();
  yAnchor = pReader->pulse.get_anchor_y();
  zAnchor = pReader->pulse.get_anchor_z();
  xTarget = pReader->pulse.get_target_x();
  yTarget = pReader->pulse.get_target_y();
  zTarget = pReader->pulse.get_target_z();
  // Compute first and last returning Values
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
 * Writes all GPS information to a csv file
 */
void GPSInformation::writeToFileGPSInformation(){
  long long pulseIndex = 0;
  FILE *scanout;
  scanout = fopen("gps.csv", "w");
  fprintf(scanout, "Pulse Index, GPS Time, X Anchor, Y Anchor,  Z Anchor, \
                    X Target, Y Target, Z Target, X First, \
                    Y First, Z First, X Last, Y Last, Z Last, \
                    edge, Scan Direction, facet, intensity\n");
  
  pReader->seek(0);
  while(pReader->read_pulse()) {
    gpsTime = pReader->pulse.get_t();
    
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
    
    fprintf(scanout, "%lld,%.8lf,   \
                      %lf,%lf,%lf,  \
                      %lf,%lf,%lf,  \
                      %lf,%lf,%lf,  \
                      %lf,%lf, %lf, \
                      %d,%d,%d,%d,\n", 
            pulseIndex, gpsTime, 
            xAnchor, yAnchor, zAnchor, 
            xTarget, yTarget, zTarget,
            xFirst, yFirst, zFirst,
            xLast, yLast, zLast, 
            edge, scanDirection, facet, intensity) ;
    pulseIndex++;
  }
}
