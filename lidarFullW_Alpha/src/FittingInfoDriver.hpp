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

class FitterInfoDriver {
    public:
        void write_data(FlightLineData &data, ofstream outFile);
        std::string parse_args(int argc, char *argv[]);
        std::string getTrimmedFileName();
};

#endif /* FITTINGINFODIRVER_HPP_ */
