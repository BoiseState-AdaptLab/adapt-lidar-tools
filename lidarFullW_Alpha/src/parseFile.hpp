/*
 * File name: cmd_line.hpp
 * Created on: 19-May-2017
 * Author: hravi
 */

#ifndef PARSEFILE_HPP_
#define PARSEFILE_HPP_

#include <iostream>
#include <fstream>
#include <sstream>

class ParseFile{

private:
  //possible parameters


public:
  // help parameter
  bool printFileMessage;

    ParseFile();
    void parse(int argc, char *argv[]);
    std::string getUsageMessage();
    void setUsageMessage();
    void setInputFileName(char *args);
    void setInputFileName(std::string filename);
    std::string getInputFileName();

  void parseFile(std::string filename);
};



#endif /* PARSEFILE_HPP_ */
