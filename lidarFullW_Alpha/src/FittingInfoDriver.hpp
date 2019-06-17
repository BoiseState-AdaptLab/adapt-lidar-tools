/*
 * File Name: FittingInfoDriver.hpp
 * Created On: 14 June 2019
 * Author: Aaron Orenstein
*/

#ifndef FITTINGIFNODRIVER_HPP_
#define FITTINGINFODRIVER_HPP_

#include "FlightLineData.hpp"
#include "PulseData.hpp"
#include "GaussianFitter.hpp"
#include "Peak.hpp"
#include <fstream>
#include <math.h>
#include <getopt.h>

class FittingInfoDriver {
    public:
        bool printUsageMessage;
        int lowerBound;
        int upperBound;

        FittingInfoDriver();
        void writeData(FlightLineData &data, std::string out_file_name);
        std::string parse_args(int argc, char *argv[]);
        std::string getTrimmedFileName(std::string name);

    private:
        std::string getUsageMessage();
        void check_input_file_exists(std::string name,
            std::vector<std::string> msgs);
};

#endif /* FITTINGINFODIRVER_HPP_ */
