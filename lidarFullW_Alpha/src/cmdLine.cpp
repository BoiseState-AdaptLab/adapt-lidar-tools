/*
 * File name: cmd_line.cpp
 * Created on: 17-May-2017
 * Author: ravi
 */


#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "cmdLine.hpp"
#include "parseFile.hpp"

using namespace std;

void printUsage(char *s);

/**
* Custom exception for no command line arguments
*/
struct cmdLineException : public exception{
  const char * what() const throw(){
    return "No command line arguments";
  }
};

void parseCmdLineArgs (int argc,char *argv[])
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

  /*use function getopt_long to get the arguments with the option.
   * ":hf:o:" indicate that option h is without arguments while
   * f and 0 are options with arguments*/
  while((optionChar = getopt_long (argc, argv, ":hf:",
      long_options, &option_index))!= -1)
  {
    switch(optionChar)
    {
    /*option h show the help information*/
    case 'f':
      fArg = optarg;
      parseFile(fArg);
      break;
    case 'h':
      printUsage(argv[0]);
      break;
    case ':':
      /* missing option argument */
      std::cerr << argv[0] << ": option '-" << optopt <<
        "' requires an argument\n" <<endl;
      printUsage(argv[0]);
      break;
    default: /* '?' */
      /* invalid option */
      fprintf(stderr, "%s: option '-%c' is invalid\n",
          argv[0], optopt);
      printUsage(argv[0]);
      break;
    }

  }
}


/* Function that shows correct usage of this program*/
void printUsage(char *s)
{
  cout<<"\nUsage:   "<<s<<" [-option argument]+\n"<<endl;
  cout<<"Option:  "<<"-f  ../src/fileName.pls"<<endl;
  cout<<"Help:    "<<"-h\n"<<endl;
  cout<<"Example: "<<s<<" -f ../src/140823_183115_1_clipped_test.pls\n"<<endl;
}

