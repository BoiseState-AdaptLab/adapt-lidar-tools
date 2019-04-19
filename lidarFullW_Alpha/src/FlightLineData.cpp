// File name: FlightLineData.cpp
// Created on: 09-November-2017
// Author: ravi

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
  
  geoascii_params = "";
  utm_str = "";
  geog_cs = "";
  utm = 0;

  next_pulse_exists = false;
}


// Stores the instrument information and initializes pReader
void FlightLineData::setFlightLineData(std::string fileName){

  pOpener.set_file_name(fileName.c_str());
  pReader = pOpener.open();
  if(pReader == NULL){
    throw "Unable to Open File" + fileName;
    exit (EXIT_FAILURE);
  }


  //bounding box x,y & z mins and maxes
  bb_x_min = pReader->header.min_x;
  bb_y_min = pReader->header.min_y;
  bb_z_min = pReader->header.min_z;
  bb_x_max = pReader->header.max_x;
  bb_y_max = pReader->header.max_y;
  bb_z_max = pReader->header.max_z;

  //TODO: Figure out how to use get_geoascii_params properly
  geoascii_params = pReader->header.geoascii_params;

  // Vector of string to save tokens
  std::vector <std::string> tokens; 
     
  // use stringstream to parse
  std::stringstream geo_stream(geoascii_params);

  tokenize_geoascii_params_to_vector(&geo_stream,&tokens);

  int utm_loc = locate_utm_field(&tokens);
  if(utm_loc < 0){
  	//no utm found, trouble!
  	std::cerr << "CRITICAL ERROR! No UTM value found in PLS header!\n"<<std::endl;
  	exit(EXIT_FAILURE);
  }
  utm_str = tokens[utm_loc];


	utm = parse_for_UTM_value(utm_str);
	int geog_cs_loc = locate_geog_cs_field(&tokens);
	if(geog_cs_loc < 0){
		//no geog_cs found, trouble!
		std::cerr << "CRITICAL ERROR! No geog_cs (NAD/WGS) value found in PLS header!\n"<<std::endl;
		exit(EXIT_FAILURE);
	}
	geog_cs = tokens.at(geog_cs_loc);


  //std::cout << "utm_str: " << utm_str << std::endl;
  //std::cout << "utm: " << utm << std::endl;
  //std::cout << "geog_cs: "<< geog_cs << std::endl;


  //fprintf(stderr,"TEST: min_x %lf max_y %lf\n",bb_x_min,bb_y_max);

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
    if(pReader->read_pulse()){
      if(pReader->read_waves()){
        next_pulse_exists = true;
      }
    }
    else{
      next_pulse_exists = false;
      throw -1;
    }
  }
  catch(int e){
        std::cout << "CRITICAL ERROR! No data!\n";
  // TODO: FIXME!!
        exit(EXIT_FAILURE);
  }
}

// Write instrument information to a CSV
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

//True if there exists a next pulse, else false
bool FlightLineData::hasNextPulse(){
  return next_pulse_exists;
}


//Read and populate a single pulse at a time
void FlightLineData::getNextPulse(PulseData *pd){

  if(!next_pulse_exists){
    std::cout << "CRITICAL ERROR! Cannot be here if there isn't a next pulse\n";
    exit(EXIT_FAILURE);
  }
  current_wave_gps_info.populateGPS(pReader);

  //Clear the vectors since we're storing a single pulse at a time
  pd->outgoingIdx.clear();
  pd->outgoingWave.clear();
  pd->returningIdx.clear();
  pd->returningWave.clear();

  double pulse_outgoing_start_time;
  double pulse_outgoing_segment_time;
  double pulse_returning_start_time;
  double pulse_returning_segment_time;

  int num_samplings = pReader->waves->get_number_of_samplings();
  int sampling_number = 0;  // can only be 0 or 1
  sampling = pReader->waves->get_sampling(sampling_number);

  //If the first sampling is not of type outgoing, there is some error
  if(sampling->get_type() != PULSEWAVES_OUTGOING){
    std::cout << "CRITICAL ERROR! \
                  The first sampling must be an outgoing wave!\n";
    exit(EXIT_FAILURE);
  }

  //FOR TESTING PURPOSES
  // std::cout << "Starting outgoing" << std::endl; 

  //Populate outgoing wave data
  for(int j = 0; j < sampling->get_number_of_segments(); j++ ){
    sampling->set_active_segment(j);
    //set the start time of the outgoing wave and keep track of the times
    if(j == 0){
      pulse_outgoing_start_time =
                              sampling->get_duration_from_anchor_for_segment();
      pulse_outgoing_segment_time =
                              sampling->get_duration_from_anchor_for_segment();
    }
    else{
      pulse_outgoing_segment_time =
                              sampling->get_duration_from_anchor_for_segment();
    }
    for(int k = 0; k < sampling->get_number_of_samples(); k++){
      pd->outgoingIdx.push_back(pulse_outgoing_segment_time -
                                pulse_outgoing_start_time);
      int temp_amp = sampling->get_sample(k);
      pd->outgoingWave.push_back(temp_amp);
      pulse_outgoing_segment_time++;
    }
  }

  //If there exists a returning wave
  if(++sampling_number < num_samplings){
    // FOR TESTING PURPOSES
    // std::cout << "Starting returning" << std::endl;  
    sampling = pReader->waves->get_sampling(sampling_number);
    if(sampling->get_type() != PULSEWAVES_RETURNING) {
      std::cout << "CRITICAL ERROR! \
                    The second sampling must be a returning wave!\n";
      exit(EXIT_FAILURE);
    }
    //Populate returing wave data
    for(int j = 0; j < sampling->get_number_of_segments(); j++ ){
      sampling->set_active_segment(j);
      //set the start time of the returning wave and keep track of the times
      if(j == 0){
        pulse_returning_start_time =
                              sampling->get_duration_from_anchor_for_segment();
        pulse_returning_segment_time =
                              sampling->get_duration_from_anchor_for_segment();
      }
      else{
        pulse_returning_segment_time =
                              sampling->get_duration_from_anchor_for_segment();
      }
      for(int k = 0; k < sampling->get_number_of_samples(); k++){
        pd->returningIdx.push_back(pulse_returning_segment_time -
                                   pulse_returning_start_time);
        pd->returningWave.push_back(sampling->get_sample(k));
        pulse_returning_segment_time++;
      }

      // FOR TESTING PURPOSES
      // std::cerr << std::endl << "DEBUG SAMPLES: ";
      // for(int i=0; i< (int)pd->returningWave.size(); i++){
      //   std::cerr<< pd->returningWave[i] << " ";
      // }
      // std::cerr << std::endl ;
    }
  }
  else{
    // FOR TESTING PURPOSES
    // std::cout << "No returning Wave" << std::endl;
  }

  //Check if there exists a next pulse
  if(pReader->read_pulse()){
    if(pReader->read_waves()){
      next_pulse_exists = true;
      return;
    }
  }
  next_pulse_exists = false;
  return;
}


/**
 * Calculate x, y and z activation
 * @param peaks pointer to the peaks to calculate activations for
 * @return the number of peaks left after calculation
 */
int FlightLineData::calc_xyz_activation(std::vector<Peak> *peaks){

  std::vector<Peak>::iterator it;
  // for each of the incoming peaks
  for(it = peaks->begin(); it != peaks->end();){
    // if the amplitude of the peak is too small just ignore the whole
    // thing
    if((*it).amp <= (*it).triggering_amp){
      it = peaks->erase(it);
      continue;
    }
    // check to see that each of the gps locations is within our
    // bounding box -- this is for x y and z
    (*it).x_activation =
                  (*it).triggering_location * current_wave_gps_info.dx +
                                  current_wave_gps_info.x_first;
    if((*it).x_activation < bb_x_min || (*it).x_activation > bb_x_max){
      std::cerr << "\nx activation: "<< (*it).x_activation
                << " not in range: " << bb_x_min << " - " << bb_x_max <<
                std::endl;
      exit (EXIT_FAILURE);
    }

    (*it).y_activation =
                  (*it).triggering_location * current_wave_gps_info.dy +
                                  current_wave_gps_info.y_first;
    if((*it).y_activation < bb_y_min || (*it).y_activation > bb_y_max){
      std::cerr << "\ny activation: "<< (*it).y_activation
                << " not in range: " << bb_y_min << " - " << bb_y_max <<
                std::endl;
      exit (EXIT_FAILURE);
    }

    (*it).z_activation =
                  (*it).triggering_location * current_wave_gps_info.dz +
                                  current_wave_gps_info.z_first;
    if((*it).z_activation < bb_z_min || (*it).z_activation > bb_z_max){
      std::cerr << "\nz activation: "<< (*it).z_activation
                << " not in range: " << bb_z_min << " - " << bb_z_max <<
                std::endl;
      exit (EXIT_FAILURE);
    }
    it++;
  }
  return peaks->size();
}

void FlightLineData::closeFlightLineData(){
	pReader->close(true);
	delete pReader;
}


/**
 * search for UTM in a string, extract the integer value following it
 * @param input a string containing the UTM
 * @return the integer value following the UTM characters
 */
int FlightLineData::parse_for_UTM_value(std::string input){
	std::stringstream stream;
	std::string temp;
	int utm = 0;
	bool UTM_found=false;
	stream << input;

	while(!stream.eof() ){
		stream >> temp;
		if(UTM_found){
			if(std::stringstream(temp)>>utm){
				return utm;
			}
		}
		if(temp=="UTM"){
			UTM_found=true;
		}
	}
	return utm;
}

/**
 * tokenize the geoascii_parameters string from the PLS header
 * @param geo_stream pointer to the geoascii_params stream from the PLS header
 * @param tokens pointer to the vector to store results
 */

void FlightLineData::tokenize_geoascii_params_to_vector(std::stringstream *geo_stream, std::vector<std::string>
        *tokens){
	//TODO: This needs to be tested
	std::string intermediate;
	std::string final;


	// Tokenizing w.k.t. on '/'
	while(getline(*geo_stream, intermediate, '/')){
		std::stringstream ss_int(intermediate);
		while(getline(ss_int,final,'|')){
			//remove whitespace at begin and end (trim) before pushing
			size_t first = final.find_first_not_of(" \t");
			size_t last = final.find_last_not_of(" \t");
			final = final.substr(first,(last-first+1));
			tokens->push_back(final);
		}

	}
}

/**
 * find the UTM field in the tokenized geoascii parameters string
 * @param tokens pointer to the tokenized representation of the geoascii parameters field
 * @return the index of the UTM field, or -1 if no UTM field located
 */
int FlightLineData::locate_utm_field(std::vector<std::string> *tokens){
	//TODO: This needs to be tested
	for (int i =0; i< (int)tokens->size();i++){
		if(tokens->at(i).find("UTM")!=std::string::npos){
			return i;
		}
	}
	//no utm value found
	return -1;
}

/**
 * find the geog_cs field in the tokenized geoascii parameters string
 * @param tokens pointer to the tokenized representation of the geoascii parameters field
 * @return the index of the geog_cs field, or -1 if no geog_cs field located
 */
int FlightLineData::locate_geog_cs_field(std::vector<std::string> *tokens){
	for (int i = 0; i<(int)tokens->size();i++){
		if(tokens->at(i).find("WGS")!=std::string::npos||tokens->at(i).find("NAD")!=std::string::npos){
			return i;
		}
	}
	//no geog_cs value found
	return -1;
}