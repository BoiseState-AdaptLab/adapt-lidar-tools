/*
 * File name: ScannerInformation.cpp
 * Created on: 20-June-2017
 * Author: ravi
 */

#include <iostream>
#include "GPSInformation.hpp"

//Default constructor
GPSInformation::ScannerInformation(){
  // enter default values
  xa = 0;
  ya = 0;
  za = 0;
  xt = 0;
  yt = 0;
  zt = 0;
  xf = 0;
  yf = 0;
  zf = 0;
  xl = 0;
  yl = 0;
  zl = 0;

  edge = 0;
  scan_dir = 0;
  intensity = 0;
}


/*
 * Setter: Gets the scanner information and stores it
 */
void GPSInformation::setGPSInformation(){
  pReader->seek(0);
  int outCount = 0;
  int outFound = 0;
  while(pReader->read_pulse()) {
    /* Write line to pulse file */
    gpsTime = pReader->pulse.get_t();
    pReader->pulse.compute_anchor_and_target_and_dir();
    xa = pReader->pulse.get_anchor_x();
    ya = pReader->pulse.get_anchor_y();
    za = pReader->pulse.get_anchor_z();
    xt = pReader->pulse.get_target_x();
    yt = pReader->pulse.get_target_y();
    zt = pReader->pulse.get_target_z();
    pReader->pulse.compute_first_and_last();
    xf = pReader->pulse.get_first_x();
    yf = pReader->pulse.get_first_y();
    zf = pReader->pulse.get_first_z();
    xl = pReader->pulse.get_last_x();
    yl = pReader->pulse.get_last_y();
    zl = pReader->pulse.get_last_z();

    edge = pReader->pulse.edge_of_scan_line;
    scan_dir = pReader->pulse.scan_direction;
    intensity = pReader->pulse.intensity;
    
    p++;
  }
}
