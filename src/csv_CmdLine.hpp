/*
 * File name: csv_CmdLine.hpp
 * Created on: 16 July 2019
 * Author: Spencer Fleming
 */

#ifndef SCV_CMDLINE_HPP_
#define SCV_CMDLINE_HPP_

#include <iostream>
#include <fstream>
#include <getopt.h>
#include <sstream>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <stdlib.h>

class csv_CmdLine{

private:
    //possible parameters
    std::string plsFileName;
    std::string wvsFileName;

    // helpful stuff
    std::string usageMessage;
    std::string exeName;

    // If peakFlag == true, use first difference to find peaks
    // else, use smooth second difference 
    bool max_elev_flag;



public:
    // help parameters
    bool printUsageMessage;
    // use find_peaks parameter
    // True = gaussian fitting, False = first differencing
    bool useGaussianFitting;

    //True stifles all output statements
    bool quiet;

    //True means input file was a txt file
    bool is_txt;

    csv_CmdLine();


    bool parse_args(int argc, char *argv[]);
    void setUsageMessage();
    std::string getUsageMessage();
    void check_input_file_exists();
    void check_input_txt_exists();
    void setInputFileName(char *args);
    void setInputFileName(std::string filename);
    std::string getInputFileName(bool pls);
    std::string getTrimmedFileName(bool pls);
    std::string get_output_filename(int product_id);
    std::string get_product_desc(int product_id);
    std::vector<int> selected_products;
};

#endif /* CSV_CMDLINE_HPP_ */
