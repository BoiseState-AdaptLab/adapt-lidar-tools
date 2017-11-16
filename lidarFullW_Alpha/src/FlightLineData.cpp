/*
 * File name: FlightLineData.cpp
 * Created on: 09- November-2017
 * Author: ravi
 */

#include <iostream>
#include "FlightLineData.hpp"

//Default constructor
FlightLineData::FlightLineData(){
  // enter default values
  pulse_bb_x_min = 0;
  pulse_bb_y_min = 0;
  pulse_bb_z_min = 0;
  pulse_bb_x_max = 0;
  pulse_bb_y_max = 0;
  pulse_bb_z_max = 0;
  scannerId  = 0; 
  waveLength = 0;
  outgoingPulseWidth  = 0;
  scanPattern  = 0;
  numberOfMirrorFacets  = 0;
  scanFrequency  = 0;
  scanAngleMin  = 0;
  scanAngleMax  = 0;
  pulseFrequency  = 0;
  beamDiameterAtExitAperture  = 0;
  beamDivergence  = 0;
  minimalRange  = 0;
  maximalRange  = 0;
}


/*
 * Setter: Gets the scanner information and stores it
 */
void FlightLineData::setFlightLineData(std::string fileName){

  pOpener.set_file_name(fileName.c_str());
  pReader = pOpener.open();
  if(pReader == NULL){
    throw "Unable to Open File" + fileName;
  }
  //bounding box x,y & z mins and maxes
  pulse_bb_x_min = pReader->header.min_x;
  pulse_bb_y_min = pReader->header.min_y;
  pulse_bb_z_min = pReader->header.min_z;
  pulse_bb_x_max = pReader->header.max_x;
  pulse_bb_y_max = pReader->header.max_y;
  pulse_bb_z_max = pReader->header.max_z;
    
  int i = 1;
  while(pReader->header.get_scanner(&scanner, i)) {
    scannerId = i; 
    waveLength = scanner.wave_length;
    outgoingPulseWidth = scanner.outgoing_pulse_width;
    scanPattern = scanner.scan_pattern;
    numberOfMirrorFacets = scanner.number_of_mirror_facets;
    scanFrequency = scanner.scan_frequency;
    scanAngleMin = scanner.scan_angle_min;
    scanAngleMax = scanner.scan_angle_max;
    pulseFrequency = scanner.pulse_frequency;
    beamDiameterAtExitAperture = scanner.beam_diameter_at_exit_aperture;
    beamDivergence = scanner.beam_divergence;
    minimalRange = scanner.minimal_range;
    maximalRange = scanner.maximal_range;
    i++;
  }
}

/*
 * Getter: Create a CSV with the details of all the scanner information 
 * (may be more thatn one)
 */
void FlightLineData::FlightLineDataToCSV(){

  /* Provide the file name to the PULSEreadOpener
  c_str returns a const char* that points to a null-terminated string 
  (i.e. a C-style string). It is useful when you want to pass the "contents"
  of an std::string to a function that expects to work with a C-style string */

  FILE *scanout;
  scanout = fopen("scanner.csv", "w");
  fprintf(scanout,
            "Scanner Id,Wave Length,Outgoing Pulse Width,Scan Pattern, \
            Number Of Mirror Facets,Scan Frequency,Scan Angle Min, \
            Scan Angle Max,Pulse Frequency,Beam Diameter At Exit Aperture, \
            Beam Divergence,Minimal Range,Maximal Range\n");

  int i = 1;
  while(pReader->header.get_scanner(&scanner, i)) {

    scannerId = i; 
    waveLength = scanner.wave_length;
    outgoingPulseWidth = scanner.outgoing_pulse_width;
    scanPattern = scanner.scan_pattern;
    numberOfMirrorFacets = scanner.number_of_mirror_facets;
    scanFrequency = scanner.scan_frequency;
    scanAngleMin = scanner.scan_angle_min;
    scanAngleMax = scanner.scan_angle_max;
    pulseFrequency = scanner.pulse_frequency;
    beamDiameterAtExitAperture = scanner.beam_diameter_at_exit_aperture;
    beamDivergence = scanner.beam_divergence;
    minimalRange = scanner.minimal_range;
    maximalRange = scanner.maximal_range;

    fprintf(scanout, "%d,%lf,%lf,%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
            scannerId, waveLength, outgoingPulseWidth, 
            scanPattern, numberOfMirrorFacets, scanFrequency, 
            scanAngleMin, scanAngleMax, pulseFrequency, 
            beamDiameterAtExitAperture, beamDivergence, 
            minimalRange, maximalRange);
    i++;
  }
  fclose(scanout);
  pReader->close();
  delete pReader;
}
