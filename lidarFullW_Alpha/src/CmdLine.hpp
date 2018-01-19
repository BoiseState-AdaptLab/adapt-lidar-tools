/*
 * File name: CmdLine.hpp
 * Created on: 17-May-2017
 * Author: ravi
 */

#ifndef CMDLINE_HPP_
#define CMDLINE_HPP_

#include <iostream>
#include <fstream>
#include <getopt.h>
#include <sstream>

class CmdLine{

private:
  //possible parameters
  std::string inputFileName;

  // helpful stuff
  std::string usageMessage;
  std::string exeName;

  /* If peakFlag == true, use first difference to find peaks
   * else, use smooth second difference
   */
  bool peakFlag;

public:
  // help parameter
  bool printUsageMessage;
  bool exceptionFlag;

  CmdLine();

  void parse(int argc, char *argv[]);

  void setUsageMessage();
  std::string getUsageMessage();

  void setInputFileName(char *args);
  void setInputFileName(std::string filename);
  std::string getInputFileName();
  bool getPeakFlag();
};

#endif /* CMDLINE_HPP_ */
