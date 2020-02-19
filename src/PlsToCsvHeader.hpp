// File name: PlsToCsvHelper.hpp
// Created on: February 2020
// Author: Nicholas Prussen

#ifndef PLSTOCSVHEADER_HPP_
#define PLSTOCSVHEADER_HPP_

#include "CmdLine.hpp"
#include "FlightLineData.hpp"
#include "PulseData.hpp"
#include "Peak.hpp"
#include "GaussianFitter.hpp"
#include "csv_CmdLine.hpp"

class PlsToCsvHelper {
    public:

        std::vector<Peak*> fit_data_csv(FlightLineData &raw_data,
                          csv_CmdLine &cmdLine);

        std::string getPeaksProperty(const std::vector<Peak*>& peaks, int productID);

        bool writeLinesToFile(std::string filename, std::vector<std::string> lines);
};

#endif
