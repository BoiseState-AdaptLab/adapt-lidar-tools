/*
 * File name: testDriver.cpp
 * Created on: 18-May-2017
 * Author: ravi
 */
#include <iostream>
#include <vector>
#include "cmdLine.hpp"
using namespace std;


int main (){
	int noOfArgs;
	std :: vector <char> someArgs[10];

	//Test for only 1 command line argument
	noOfArgs = 1;
	parseCmdLineArgs(noOfArgs,NULL);
	
	//Test for 2 command line arguments
	noOfArgs = 2;
	parseCmdLineArgs(noOfArgs,someArgs);

	return 0;
}
