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
* Custom exceptions
*
****************************************************************************/

// Custom exceptions for no command line arguments
struct CmdLineException : public exception{
  const char * what() const throw(){
    return "No command line arguments exception caught\n";
  }
};


// Custom exception for missing argument
struct missingArgException : public exception{
  const char * what() const throw(){
    return "Missing argument option exception caught\n";
  }
};

// Custom exception for invalid option
struct invalidOptionException : public exception{
  const char * what() const throw(){
    return "Invalid option exception caught\n";
  }
};


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
  buffer << "       pathToExecutable [-option argument]+" << std::endl;
  buffer << "\nOption:  " << std::endl;
  buffer << "       -f  pathToFile.pls"
         << "  (first difference to find peaks)" << std::endl;
  buffer << "       -s  pathToFile.pls"
         << "  (smooth second difference to find peaks)" << std::endl;
  buffer << "       -h" << std::endl;
  buffer << "\nExample: " << std::endl;
  buffer << "       pathToExecutable -f ../src/140823_183115_1_clipped_test.pls\n" << std::endl;
  usageMessage.append(buffer.str());
}

// Function that prints correct usage of this program
std::string CmdLine::getUsageMessage(){
    return usageMessage;
}

// Default constructor
CmdLine::CmdLine(){
  // enter default values
  noise_level = 3;
  printUsageMessage = false;
  exceptionFlag = false;
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
    printUsageMessage = true;
    throw CmdLineException();
  }

  exeName.append(argv[0]);

  static struct option long_options[] =
  {
      {"first", required_argument, NULL, 'f'},
      {"smooth", required_argument, NULL, 's'},
      {"help", no_argument, NULL, 'h'},
      {0, 0, 0, 0}
  };


  // getopt_long stores the option index here. 
  int option_index = 0;

  /* Using getopt_long to get the arguments with an option.
   * ":hf:s:" indicate that option 'h' is without arguments while
   * option 'f' and 's' require arguments
   */
  while((optionChar = getopt_long (argc, argv, ":hf:s:",
         long_options, &option_index))!= -1){
    switch(optionChar){
      // Option 'h' shows the help information
      case 'f': //Find peaks using first difference
        fArg = optarg;
        setInputFileName(fArg);
        peakFlag = true;
        break;
      case 's': //Find peaks using smooth second difference
        fArg = optarg;
        setInputFileName(fArg);
        peakFlag = false;
        break;
      case 'h':
        printUsageMessage = true;
        break;
      case ':':
        // Missing option argument 
        exceptionFlag = true;
        throw missingArgException();
      default:
        // Invalid option 
        exceptionFlag = true;
        throw invalidOptionException();
    }
  }
  // For non option input
  if(optind < argc){
    printUsageMessage = true;
  }
}


// Return peakFlag value
bool CmdLine::getPeakFlag(){
  return peakFlag;
}
