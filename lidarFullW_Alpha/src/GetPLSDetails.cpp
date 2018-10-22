#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "pulsereader.hpp"
#include "pulsewriter.hpp"
#include "PulseData.hpp"

int main (int argc, char *argv[]) {

  PULSEreadOpener pOpener;
  PULSEreader *pReader;

  if(argc < 2){
    std::cerr << "Usage: " << argv[0] << " <path to .pls file>" <<std::endl;
    exit(EXIT_FAILURE);
  }

  std::string fileName  = argv[1];
  pOpener.set_file_name(fileName.c_str());
  pReader = pOpener.open();
  if(pReader == NULL){
    throw "Unable to Open File" + fileName;
    exit (EXIT_FAILURE);
  }

  /* This record is simply an array of ASCII data. It contains one or 
  * many strings separated by null or space characters which are referenced 
  * by position from tags in the GeoKeyDirectory */
  std::string geoascii_params = pReader->header.geoascii_params;

  /* The min and max fields describe the bounding box that includes the 
  * first and the last points of the sampled parts of the returning 
  * waveforms of all pulses.*/
  double  bb_x_min = pReader->header.min_x;
  double  bb_y_max = pReader->header.max_y;
  
  std::vector <std::string> tokens; /* vector of strings to store tokens */
  std::stringstream geo_stream(geoascii_params); /* Use stringStream to parse */
      
  std::string intermediate; /* Store temporary strings */ 
      
  // Tokenizing w.r.t. '/' 
  while(getline(geo_stream, intermediate, '/')){ 
      tokens.push_back(intermediate); 
  } 
      
  std::string utm_str = tokens[0];
  std::string geog_cs = tokens[1];

  std::cout << "\nUTM: " << utm_str << std::endl;
  std::cout << "Geog_cs: "<< geog_cs << std::endl;

  std::cout << "\nMin x: "<< bb_x_min << std::endl;
  std::cout << "Max y: "<< bb_y_max << std::endl;
  std::cout << std::endl;

}
