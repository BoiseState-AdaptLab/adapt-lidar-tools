// File name: CmdLine.cpp
// Created on: 17-May-2017
// Author: ravi

#include <iostream>
#include <fstream>
#include <getopt.h>
#include <sstream>
#include "CmdLine.hpp"

using namespace std;


/****************************************************************************
*
* Begin CmdLine functions
*
****************************************************************************/

// Function that sets the command line arguments
void CmdLine::setInputFileName(char *args){
  inputFileName = args;
}

// Function that prints(sets up) correct usage of this program
void CmdLine::setUsageMessage()
{
  std::stringstream buffer;
  buffer << "\nUsage: " << std::endl;
  buffer << "       path_to_executable [-option argument]+" << std::endl;
  buffer << "Option:  " << std::endl;
  buffer << "       -f  <path to pls file>"
         << "  :Generate a Geotif file with max elevations" << std::endl;
  buffer << "Option:  " << std::endl;
  buffer << "       -d"
         << "  :Disables gaussian fitter, using first diff method instead" << std::endl;
  buffer << "       -h" << std::endl;
  buffer << "\nExample: " << std::endl;
  buffer << "       bin/lidarDriver -f ../src/140823_183115_1_clipped_test.pls" << std::endl;
  usageMessage.append(buffer.str());
}

// Function that prints correct usage of this program
std::string CmdLine::getUsageMessage(){
    return usageMessage;
}

// Default constructor
CmdLine::CmdLine(){
  printUsageMessage = false;
  useGaussianFitting = true;
  exeName = "";
  setUsageMessage();
}

// Function that returns the file name
std::string CmdLine::getInputFileName(){
  return inputFileName;
}


// Function that parses the command line arguments
void CmdLine::parse(int argc,char *argv[]){
  char optionChar;  //Option character
  char *fArg;       //Argument of the option character

  /* If the program is run without any command line arguments, display
   * the correct program usage and quit.*/
  if(argc < 2){
    std::cout << "\nNo command line arguments" <<std::endl;
    std::cout << "-------------------------" <<std::endl;
    printUsageMessage = true;
  }

  exeName.append(argv[0]);

  static struct option long_options[] =
  {
      {"file", required_argument, NULL, 'f'},
      {"help", no_argument, NULL, 'h'},
      {"firstdiff", no_argument, NULL, 'd'},
      {0, 0, 0, 0}
  };


  // getopt_long stores the option index here.
  int option_index = 0;

  /* Using getopt_long to get the arguments with an option.
   * ":hf:s:" indicate that option 'h' is without arguments while
   * option 'f' and 's' require arguments
   */
  while((optionChar = getopt_long (argc, argv, ":hdf:",
         long_options, &option_index))!= -1){
    switch(optionChar){
      // Option 'h' shows the help information
      case 'f': //Generate a geotif file of max elevations
        fArg = optarg;
        setInputFileName(fArg);
        break;
      case 'h':
        printUsageMessage = true;
        break;
      case 'd':
	useGaussianFitting = false;
	break;
      case ':':
        // Missing option argument
	std::cout << "\nMissing arguments" <<std::endl;
    	std::cout << "------------------" <<std::endl;
        printUsageMessage = true;
	break;
      default:
        // Invalid option
	std::cout << "\nInvalid option" <<std::endl;
    	std::cout << "---------------" <<std::endl;
        printUsageMessage = true;
	break;
    }
  }
  // For non option input
  if(optind < argc){
    printUsageMessage = true;
  }
}

int CmdLine::parse_args(int argc, char *argv[], std::string &fileName) {
  parse(argc, argv);
  //if arguments were invalid or did not parse correctly
  if (printUsageMessage == true) {
    std::cout << getUsageMessage() << std::endl;
    return 1;
  }
  //try to get the fileName
  fileName = getInputFileName();
  //if file not found
  if (!std::ifstream(fileName.c_str())) {
    std::cout << "File " << fileName << " not found." << std::endl;
    std::cout << getUsageMessage() << std::endl;
    return 1;
  }
  return 0;
}


std::string CmdLine::getTrimmedFileName(){
   size_t start = inputFileName.find_last_of("/");
   if(start==string::npos){
     start = 0;
   }else{
     start++;
   }
   size_t end = inputFileName.find_last_of(".");
   int len = end - start;
   return inputFileName.substr(start,len);
}