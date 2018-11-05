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
      {0, 0, 0, 0}
  };


  // getopt_long stores the option index here.
  int option_index = 0;

  /* Using getopt_long to get the arguments with an option.
   * ":hf:s:" indicate that option 'h' is without arguments while
   * option 'f' and 's' require arguments
   */
  while((optionChar = getopt_long (argc, argv, ":hf:",
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
