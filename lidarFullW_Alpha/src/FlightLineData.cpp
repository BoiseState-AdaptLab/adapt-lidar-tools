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
  bb_x_min = 0;
  bb_y_min = 0;
  bb_z_min = 0;
  bb_x_max = 0;
  bb_y_max = 0;
  bb_z_max = 0;
  scanner_id  = 0; 
  wave_length = 0;
  outgoing_pulse_width  = 0;
  scan_pattern  = 0;
  number_of_mirror_facets  = 0;
  scan_frequency  = 0;
  scan_angle_min  = 0;
  scan_angle_max  = 0;
  pulse_frequency  = 0;
  beam_diameter_at_exit_aperture  = 0;
  beam_divergence  = 0;
  minimal_range  = 0;
  maximal_range  = 0;
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
  bb_x_min = pReader->header.min_x;
  bb_y_min = pReader->header.min_y;
  bb_z_min = pReader->header.min_z;
  bb_x_max = pReader->header.max_x;
  bb_y_max = pReader->header.max_y;
  bb_z_max = pReader->header.max_z;
    
  int i = 1;
  while(pReader->header.get_scanner(&scanner, i)) {
    scanner_id = i; 
    wave_length = scanner.wave_length;
    outgoing_pulse_width = scanner.outgoing_pulse_width;
    scan_pattern = scanner.scan_pattern;
    number_of_mirror_facets = scanner.number_of_mirror_facets;
    scan_frequency = scanner.scan_frequency;
    scan_angle_min = scanner.scan_angle_min;
    scan_angle_max = scanner.scan_angle_max;
    pulse_frequency = scanner.pulse_frequency;
    beam_diameter_at_exit_aperture = scanner.beam_diameter_at_exit_aperture;
    beam_divergence = scanner.beam_divergence;
    minimal_range = scanner.minimal_range;
    maximal_range = scanner.maximal_range;
    i++;
  }
  
  //Initialize the pReader to read the pulse and the wave
  //If no data, throw an exception and exit
  try{
    if(pReader->read_waves()){
      pReader->read_pulse();
    }
    else{
      throw -1;
    }
  }
  catch(int e){
        std::cout << "CRITICAL ERROR! No data!\n";
        exit(EXIT_FAILURE);
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

    scanner_id = i; 
    wave_length = scanner.wave_length;
    outgoing_pulse_width = scanner.outgoing_pulse_width;
    scan_pattern = scanner.scan_pattern;
    number_of_mirror_facets = scanner.number_of_mirror_facets;
    scan_frequency = scanner.scan_frequency;
    scan_angle_min = scanner.scan_angle_min;
    scan_angle_max = scanner.scan_angle_max;
    pulse_frequency = scanner.pulse_frequency;
    beam_diameter_at_exit_aperture = scanner.beam_diameter_at_exit_aperture;
    beam_divergence = scanner.beam_divergence;
    minimal_range = scanner.minimal_range;
    maximal_range = scanner.maximal_range;

    fprintf(scanout, "%d,%lf,%lf,%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
            scanner_id, wave_length, outgoing_pulse_width, 
            scan_pattern, number_of_mirror_facets, scan_frequency, 
            scan_angle_min, scan_angle_max, pulse_frequency, 
            beam_diameter_at_exit_aperture, beam_divergence, 
            minimal_range, maximal_range);
    i++;
  }
  fclose(scanout);
  pReader->close();
  delete pReader;
}

void FlightLineData::getNextPulse(PulseData *pd){
  outgoing_time.clear();
  outgoing_wave.clear();
  returning_time.clear();
  returning_wave.clear();

  double pulse_outgoing_start_time;
  double pulse_returning_start_time;
  double segment_time;

  for(int i = 0; i < pReader->waves->get_number_of_samplings(); i++){
    sampling = pReader->waves->get_sampling(i);

    //If the first sampling is not outgoing, irrecoverable error, exit. 
    try{
      if(sampling->get_type() != PULSEWAVES_OUTGOING) {
        throw -1;
      }
    }
    catch(int e){
      std::cout << "CRITICAL ERROR! Must be an outgoing wave!\n";
      exit(EXIT_FAILURE);
    }
  }

    //Populate outgoing wave dat
    printf("Outgoing\n");
    for(int j = 0; j < sampling->get_number_of_segments(); j++ ){
        sampling->set_active_segment(j);            
        //set the start time of the outgoing pulse and keep track of the times
        if(j == 0){            
          pulse_outgoing_start_time = sampling->get_duration_from_anchor_for_segment();
          segment_time = sampling->get_duration_from_anchor_for_segment();
        }
        else{
          segment_time = sampling->get_duration_from_anchor_for_segment();
        }
        for(int k = 0; k < sampling->get_number_of_samples(); k++){
          outgoing_time.push_back(segment_time);
          outgoing_wave.push_back(sampling->get_sample(k));
          segment_time++;
        }
    } 
    // only continue if this is an incoming pulse
    if(pReader->read_pulse()) {

      if(sampling->get_type() != PULSEWAVES_RETURNING) {
        return;
      }
      //pd->populateReturning(sampling, maxCount, pulseIndex);
      //returningWave.push_back(pulseIndex);
      printf("Returning\n");
      for(int j = 0; j < sampling->get_number_of_segments(); j++ ){
        sampling->set_active_segment(j);
        //set the start time of the returning pulse and keep track of the times
        if(j == 0){            
          pulse_returning_start_time = sampling->get_duration_from_anchor_for_segment();
          segment_time = sampling->get_duration_from_anchor_for_segment();
        }
        else{
          segment_time = sampling->get_duration_from_anchor_for_segment();
        }
        for(int k = 0; k < sampling->get_number_of_samples(); k++){
          returning_time.push_back(segment_time);
          returning_wave.push_back(sampling->get_sample(k));
        }
      } 
    }
    pReader->read_pulse();
}
