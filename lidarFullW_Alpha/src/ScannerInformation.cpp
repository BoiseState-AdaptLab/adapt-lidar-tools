/*
 * File name: ScannerInformation.cpp
 * Created on: 15-June-2017
 * Author: ravi
 */

#include <iostream>
#include "ScannerInformation.hpp"

//Default constructor
ScannerInformation::ScannerInformation(){
  // enter default values
  scannerId [10] = {0}; 
  waveLength [10 ]= {0};
  outgoingPulseWidth [10] = {0};
  scanPattern [10] = {0};
  numberOfMirrorFacets [10] = {0};
  scanFrequency [10] = {0};
  scanAngleMin [10] = {0};
  scanAngleMax [10] = {0};
  pulseFrequency [10] = {0};
  beamDiameterAtExitAperture [10] = {0};
  beamDivergence [10] = {0};
  minimalRange [10] = {0};
  maximalRange [10] = {0};
}


/*
 *Read the file and set he scanner information
 */
void ScannerInformation::setScannerInformation(/* File name goes here */){
  pOpener.set_file_name(/* File name goes here. const CHAR* file_name*/);
  pReader = pOpener.open();
  
  int i =1;
  while(pReader->header.get_scanner(&scanner, i)) {
    scannerId[i] = i; 
    waveLength[i]= scanner.wave_length;
    outgoingPulseWidth[i] = scanner.outgoing_pulse_width;
    scanPattern[i] = scanner.scan_pattern;
    numberOfMirrorFacets[i] = scanner.number_of_mirror_facets;
    scanFrequency[i] = scanner.scan_frequency;
    scanAngleMin[i] = scanner.scan_angle_min;
    scanAngleMax[i] = scanner.scan_angle_max;
    pulseFrequency[i] = scanner.pulse_frequency;
    beamDiameterAtExitAperture[i] = scanner.beam_diameter_at_exit_aperture;
    beamDivergence[i] = scanner.beam_divergence;
    minimalRange[i] = scanner.minimal_range;
    maximalRange[i] = scanner.maximal_range;
    i++;
  }
  
  pReader->close();
  delete pReader;
}

void ScannerInformation::getScannerInformation(){

  FILE *scanout;
  scanout = fopen("Scanner.csv", "w");
  fprintf(scanout,
            "Scanner Id,Wave Length,Outgoing Pulse Width,Scan Pattern,"
            "Number Of Mirror Facets,Scan Frequency,Scan Angle Min,"
            "Scan Angle Max,Pulse Frequency,Beam Diameter At Exit Aperture,"
            "Beam Divergence,Minimal Range,Maximal Range,Maximal_range\n");

    int i = 1;
    while(pReader->header.get_scanner(&scanner, i)) {   
      fprintf(scanout, "%d,%lf,%lf,%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
              scannerId[i], waveLength[i], outgoingPulseWidth[i], 
              scanPattern[i], numberOfMirrorFacets[i], scanFrequency[i], 
              scanAngleMin[i], scanAngleMax[i], pulseFrequency[i], 
              beamDiameterAtExitAperture[i], beamDivergence[i], 
              minimalRange[i], maximalRange[i]);
      i++;
    }
    fclose(scanout);
}
