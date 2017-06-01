/*
 * File name: cmd_line.hpp
 * Created on: 17-May-2017
 * Author: hravi
 */

#ifndef CMDLINE_HPP_
#define CMDLINE_HPP_

#include <iostream>
#include <fstream>
#include <getopt.h>
#include <sstream>

class CmdLineArgs{

private:
  //possible parameters
  std::string inputFileName;
  // helpful stuff
  std::string usageMessage;
  std::string exeName;

public:
  // help parameter
  bool printUsageMessage;

    CmdLineArgs();
    void parse(int argc, char *argv[]);
    std::string getUsageMessage();
    void setUsageMessage();
    void setInputFileName(char *args);
    void setInputFileName(std::string filename);
    std::string getInputFileName();
};

#endif /* CMDLINE_HPP_ */
