#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>
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
  long double  bb_x_min = pReader->header.min_x;
  long double  bb_x_max = pReader->header.max_x;
  long double  bb_y_min = pReader->header.min_y;
  long double  bb_y_max = pReader->header.max_y;

  int number_of_pulses =  pReader->header.number_of_pulses;

  pReader->close();
  delete pReader;

  double bb_x_idx_min = 0;
  double bb_y_idx_min = 0;
  int bb_x_idx_max = (int) (ceil(bb_x_max)) - (floor(bb_x_min));
  int bb_y_idx_max = (int) (ceil(bb_y_max)) - (floor(bb_y_min));

  /* extent of x and yas calculated from the pulse data (max - min) */
  double i_extent = bb_x_idx_max - bb_x_idx_min + 1;
  double j_extent = bb_y_idx_max - bb_y_idx_min + 1;
  
  std::vector <std::string> tokens; /* vector of strings to store tokens */
  std::stringstream geo_stream(geoascii_params); /* Use stringStream to parse */
      
  std::string intermediate; /* Store temporary strings */ 
      
  // Tokenizing w.r.t. '/' 
  while(getline(geo_stream, intermediate, '/')){ 
      tokens.push_back(intermediate); 
  } 
      
  std::string utm_str = tokens[0];
  std::string geog_cs = tokens[1];

  std::cout << "\nReading information from the header of: " <<  argv[1]
            << std::endl;
  std::cout << "----------------------------------------";
  for(std::size_t i = 0; i < strlen(argv[1]); i++){
    std::cout << "-";
  }
  std::cout << std::endl;
  std::cout << "No of pulses: " << number_of_pulses << std::endl;

  std::cout << "\nUTM: " << utm_str << std::endl;
  std::cout << "Geog_cs: "<< geog_cs << std::endl;

  std::cout << "\nMin x: "<< bb_x_min << std::endl;
  std::cout << "Max x: "<< bb_x_max << std::endl;
  std::cout << "Min y: "<< bb_y_min << std::endl;
  std::cout << "Max y: "<< bb_y_max << std::endl;

  fprintf(stdout,"\nMin x: %Lf\nMax y: %Lf\n",bb_x_min,bb_y_max);

  std::cout << "\nCalculating i, j max, mins and extents:"<< std::endl;
  std::cout << "---------------------------------------"<< std::endl;
  std::cout << "\nBounding box 'x_idx' min: "<< bb_x_idx_min << std::endl;
  std::cout << "Bounding box 'x_idx' max: "<< bb_x_idx_max << std::endl;
  std::cout << "Bounding box 'y_idx' min: "<< bb_y_idx_min << std::endl;
  std::cout << "Bounding box 'y_idx' max: "<< bb_y_idx_max << std::endl;

  std::cout << "\nBounding box 'x_idx' extent: "<< i_extent << std::endl;
  std::cout << "Bounding box 'y_idx' extent: "<< j_extent << std::endl;
  std::cout << std::endl;

}
