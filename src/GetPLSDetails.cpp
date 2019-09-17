#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>
#include "pulsereader.hpp"
#include "pulsewriter.hpp"
#include "PulseData.hpp"

// Activity level must be defined before spdlog is included.
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
int main (int argc, char *argv[]) {

    // Setting up logger
    spdlog::set_pattern("[%^%=8l%$] %v");
    // Sets new pattern for timestamp
  
    auto logger = spdlog::create_async<spdlog::sinks::stdout_color_sink_mt>(
                 "logger");    

    PULSEreadOpener pOpener;
    PULSEreader *pReader;

    if(argc < 2){
        spdlog::error("Usage: {} <path to .pls file>", argv[0]);
        return 1;
    }

    std::string fileName    = argv[1];
    pOpener.set_file_name(fileName.c_str());
    pReader = pOpener.open();
    if(pReader == NULL){
        spdlog::critical("Unable to Open File {}", fileName);
        return 1;
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
    double x_idx_extent = bb_x_idx_max - bb_x_idx_min + 1;
    double y_idx_extent = bb_y_idx_max - bb_y_idx_min + 1;

    std::vector <std::string> tokens; /* vector of strings to store tokens */
    std::stringstream geo_stream(geoascii_params); /* Use stringStream to
                                                      parse */

    std::string intermediate; /* Store temporary strings */ 

    // Tokenizing w.r.t. '/' 
    while(getline(geo_stream, intermediate, '/')){ 
        tokens.push_back(intermediate); 
    } 

    std::string utm_str = tokens[0];
    std::string geog_cs = tokens[1];

    spdlog::trace("\nReading information from the header of: {}", argv[1]);
       
    spdlog::trace("----------------------------------------")
    for(std::size_t i = 0; i < strlen(argv[1]); i++){
        spdlog::trace("-");
    }
    spdlog::trace("No of pulses: {}", number_of_pulses);

    spdlog::trace("\nUTM: {}", utm_str);
    spdlog::trace("Geog_cs: {}", geog_cs);

    spdlog::trace("\nMin x: {}", bb_x_min);
    spdlog::trace("Max x: {}", bb_x_max);
    spdlog::trace("Min y: {}", bb_y_min);
    spdlog::trace("Max y: {}", bb_y_max);

    fprintf(stdout,"\nMin x: %Lf\nMax y: %Lf\n",bb_x_min,bb_y_max);

    spdlog::trace("\nCalculating i, j max, mins and extents:");
    spdlog::trace("---------------------------------------");
    spdlog::trace("\nBounding box 'x_idx' min: {}", bb_x_idx_min);
    spdlog::trace("Bounding box 'x_idx' max: {}", bb_x_idx_max);
    spdlog::trace("Bounding box 'y_idx' min: {}", bb_y_idx_min);
    spdlog::trace("Bounding box 'y_idx' max: {}", bb_y_idx_max);

    spdlog::trace("\nBounding box 'x_idx' extent: {}", x_idx_extent);
    spdlog::trace("Bounding box 'y_idx' extent: {}", y_idx_extent);

}
