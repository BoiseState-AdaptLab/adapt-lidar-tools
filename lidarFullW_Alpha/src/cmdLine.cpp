/*
 * File name: cmd_line.cpp
 * Created on: 17-May-2017
 * Author: ravi
 */

#include <iostream>
#include <fstream>
#include <getopt.h>
#include <sstream>
#include "cmdLine.hpp"

using namespace std;


/****************************************************************************
* 
* Custom exceptions
* 
****************************************************************************/

/**
* Custom exceptions for no command line arguments
*/
struct cmdLineException : public exception{
  const char * what() const throw(){
    return "No command line arguments exception caught\n";
  }
};

/**
* Custom exception for missing argument
*/
struct missingArgException : public exception{
  const char * what() const throw(){
    return "Missing argument option exception caught\n";
  }
};

/**
* Custom exception for invalid option
*/
struct invalidOptionException : public exception{
  const char * what() const throw(){
    return "Invalid option exception caught\n";
  }
};

/****************************************************************************
* 
* Begin CmdLineArgs functions
* 
****************************************************************************/

/*Function that sets the command line arguments*/
void CmdLineArgs::setInputFileName(char *args){
  inputFileName = args;
}

/* Function that prints(sets up) correct usage of this program*/
void CmdLineArgs::setUsageMessage()
{
  std::stringstream buffer;
  buffer <<"Usage:   " << exeName <<" [-option argument]+" << std::endl;
  buffer <<"Option:  " << "-f  ../src/fileName.pls" << std::endl;
  buffer <<"Help:    " << "-h" << std::endl;
  buffer <<"Example: " << exeName
            << " -f ../src/140823_183115_1_clipped_test.pls\n"
            <<std::endl;
  usageMessage.append(buffer.str());
}

/* Function that prints correct usage of this program*/
std::string CmdLineArgs::getUsageMessage(){
    return usageMessage;
}

/* Default constructor*/
CmdLineArgs::CmdLineArgs(){
  // enter default values
  printUsageMessage = false;
  setUsageMessage();
}

/* Function that returns the file name*/
std::string CmdLineArgs::getInputFileName(){
  return inputFileName;
}

/* Function that parses the command line arguments*/
void CmdLineArgs::parse(int argc,char *argv[])
{
  char optionChar;  /* Option character */
  char *fArg;     /* Argument of the f(file) option character */

  /*if the program is run without any command line arguments, display
   * the correct program usage and quit.*/
  if(argc < 2){
    throw cmdLineException();
  }

  exeName.append(argv[0]);
  static struct option long_options[] =
  {
      {"file", required_argument, NULL, 'f'},
      {"help", no_argument, NULL, 'h'},
      {0, 0, 0, 0}
  };


  /* getopt_long stores the option index here. */
  int option_index = 0;

  /*use function getopt_long to get the arguments with the option.
   * ":hf:o:" indicate that option h is without arguments while
   * f and o are options with arguments
   */
  while((optionChar = getopt_long (argc, argv, ":hf:",
      long_options, &option_index))!= -1)
  {
    
    switch(optionChar)
    {
    /*option h show the help information*/
    case 'f':
      fArg = optarg;
      setInputFileName(fArg);
      break;
    case 'h':
      printUsageMessage = true;
      break;
    case ':':
      /* missing option argument */
      throw missingArgException();
    default:
      /* invalid option */
      throw invalidOptionException();
    }
  }
  
}
