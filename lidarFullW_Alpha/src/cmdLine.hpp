/*
 * File name: cmd_line.hpp
 * Created on: 17-May-2017
 * Author: hravi
 */

#ifndef CMDLINE_HPP_
#define CMDLINE_HPP_

class CmdLineArgs{
  char *fileName;
  public:
    void setfileName(char *args);
    char *inputFile(){
      return fileName;
    }
};
CmdLineArgs parseCmdLineArgs(int argc,char *argv[]);

#endif /* CMDLINE_HPP_ */
