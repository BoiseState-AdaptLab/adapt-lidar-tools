/*
 * File name: cmd_line.cpp
 * Created on: 17-May-2017
 * Author: ravi
 */

#include <iostream>
#include <fstream>
#include <getopt.h>
#include "cmdLine.hpp"

using namespace std;

void printUsage(char *s);

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
    return "Missing argument option caught\n";
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

/**
 * Set the command line arguments
 */
void CmdLineArgs::setfileName(char *args){
  fileName = args;
}

/* Function that prints correct usage of this program*/
void printUsage(char *s)
{
  std::cout <<"Usage:   " << s <<" [-option argument]+" << std::endl;
  std::cout <<"Option:  " << "-f  ../src/fileName.pls" << std::endl;
  std::cout <<"Help:    " << "-h" << std::endl;
  std::cout <<"Example: " << s
            << " -f ../src/140823_183115_1_clipped_test.pls\n"
            <<std::endl;
}

CmdLineArgs parseCmdLineArgs (int argc,char *argv[])
{
  char optionChar;  /* Option character */
  char *fArg;     /* Argument of the f(file) option character */

  /*if the program is run without any command line arguments, display
   * the correct program usage and quit.*/
  if(argc < 2){
    printUsage(argv[0]);
    throw cmdLineException();
  }


  static struct option long_options[] =
  {
      {"file", required_argument, NULL, 'f'},
      {"help", no_argument, NULL, 'h'},
      {0, 0, 0, 0}
  };

  /* getopt_long stores the option index here. */
  int option_index = 0;

  CmdLineArgs cla; // Initialize a CmdLineArgs type

  /*use function getopt_long to get the arguments with the option.
   * ":hf:o:" indicate that option h is without arguments while
   * f and 0 are options with arguments
   */
  while((optionChar = getopt_long (argc, argv, ":hf:",
      long_options, &option_index))!= -1)
  {
    
    switch(optionChar)
    {
    /*option h show the help information*/
    case 'f':
      fArg = optarg;
      cla.setfileName(fArg);
      break;
    case 'h':
      printUsage(argv[0]);
      break;
    case ':':
      /* missing option argument */
      throw missingArgException();
    default:
      /* invalid option */
      throw invalidOptionException();
    }
  }
  return cla;
  
}
