#include <iostream>
#include <fstream>
#include <vector>
#include "pulsereader.hpp"
#include "pulsewriter.hpp"
#include "PulseData.hpp"

int main (int argc, char *argv[]) {

  PULSEreadOpener pOpener;
  PULSEreader *pReader;

  if(argc < 2){
    std::cerr << "Usage: " << argv[0] << " <fileName.pls>" <<std::endl;
  }

  std::string fileName  = argv[1];
  pOpener.set_file_name(fileName.c_str());
  pReader = pOpener.open();
  if(pReader == NULL){
    throw "Unable to Open File" + fileName;
    exit (EXIT_FAILURE);
  }

  std::string geoascii_params = pReader->header.geoascii_params;
  // Vector of string to save tokens 
  std::vector <std::string> tokens; 
     
  // use stringstream to parse
  std::stringstream geo_stream(geoascii_params); 
      
  std::string intermediate; 
      
  // Tokenizing w.r.t. '/' 
  while(getline(geo_stream, intermediate, '/')){ 
      tokens.push_back(intermediate); 
  } 
      
  utm_str = tokens[0];
  
  // Parsing UTM string to get an int 
  std::stringstream temp_s1(tokens[0]); 
  std:: string temp;  
  //Tokenizing w.r.t. ' '
  while(getline(temp_s1, intermediate, ' ')){ 
      temp = intermediate; 
  } 
  std::stringstream temp_s2(temp);
  temp_s2 >> utm;

  geog_cs = tokens[1];

  std::cout << "utm_str: " << utm_str << std::endl;
  std::cout << "utm: " << utm << std::endl;
  std::cout << "geog_cs: "<< geog_cs << std::endl;

}
