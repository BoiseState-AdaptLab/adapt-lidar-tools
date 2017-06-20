/*
 * File name: ScannerInformation.hpp
 * Created on: 20-June-2017
 * Author: ravi
 */

#ifndef GPSINFORMATION_HPP_
#define GPSINFORMATION_HPP_

#include <iostream>
#include <fstream>
#include "pulsereader.hpp"
#include "pulsewriter.hpp"

class GPSInformation{

public:
  //possible parameters
  long long p = 0;
  double gpsTime;
  double xa, ya, za;
  double xt, yt, zt;
  double dx, dy, dz;
  double xf, yf, zf;
  double xl, yl, zl;
  unsigned char edge;
  unsigned char scan_dir;
  unsigned char intensity;

  PULSEreadOpener pOpener;
  PULSEreader *pReader;
  PULSEscanner scanner;

  GPSInformation();
  
  void setScannerInformation(std::string fileName);
  void getScannerInformation();
};

#endif /* GPSINFORMATION_HPP_ */
