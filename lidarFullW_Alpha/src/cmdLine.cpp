/*
 * File name: cmd_line.cpp
 * Created on: 17-May-2017
 * Author: ravi
 */

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "cmdLine.hpp"

void printUsage(char *s);
using namespace std;

void parseCmdLineArgs (int argc,char *argv[])
{
	char optionChar; 	/* Option character */
	char *fArg; 		/* Argument of the f(file) option character */
	char *oArg; 		/* Argument of the o(output prefix) option character */

	/*if the program is run without any command line arguments, display the correct
	 * program usage and quit.*/
	if(argc < 2){
		throw invalid_argument("No command line arguments present");
	}


	static struct option long_options[] =
	{
			/* These options don’t set a flag.
	             We distinguish them by their indices. */
			{"file",    		required_argument, 		NULL, 	'f'},
			{"help",  		no_argument,			NULL,	'h'},
			{0, 			0,				0,	0}
	};

	/* getopt_long stores the option index here. */
	int option_index = 0;

	/*use function getopt_long to get the arguments with the option.":hf:o:" indicate
	 * that option his without arguments while f and 0 are options with arguments*/
	while((optionChar = getopt_long (argc, argv, ":hf:", long_options, &option_index))!= -1)
	{
		switch(optionChar)
		{
		/*option h show the help information*/
		case 'f':
			fArg = optarg;
			cout<<"Your filename is "<<fArg<<endl;
			break;
		case 'h':
			printUsage(argv[0]);
			break;
		case ':':
			/* missing option argument */
			fprintf(stderr, "%s: option '-%c' requires an argument\n",
					argv[0], optopt);
			printUsage(argv[0]);
			break;
		case '?':
		default:
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
	cout<<"Usage:   "<<s<<" [-option argument]+\n"<<endl;
	cout<<"Option:  "<<"-f  fileName.pls"<<endl;
	cout<<"         "<<"-h  show help information\n"<<endl;
	cout<<"Example: "<<s<<" -f 140823_183115_1_clipped_test.pls"<<endl;
}
