// File name: PlsToCsvHelper.cpp
// Created on: February 2020
// Nicholas Prussen, Jared White, Floriana Ciaglia

#include "PlsToCsvHeader.hpp"
#include "spdlog/spdlog.h"

/**
 * Fits the raw data using either gaussian or first difference fitting,
 * and sends each wave of peaks to be written to a CSV file.
 * @param raw_data reference to FlightLineData object that holds raw data
 * @param strings a place where peak to_string calls will be stored
 * @param csv_CmdLine object that knows what data we want from peaks
 */
std::vector<Peak*> PlsToCsvHelper::fit_data_csv(FlightLineData &raw_data, csv_CmdLine &cmdLine) 
{
    PulseData pulseData;
    GaussianFitter fitter;
    fitter.noise_level = cmdLine.noise_level;
    std::vector<Peak*> peaks;
    std::vector<Peak*> results;

    //parse each pulse
    while (raw_data.hasNextPulse()) {
        peaks.clear();

        raw_data.getNextPulse(&pulseData);

        //Skip all the empty returning waveforms
        if (pulseData.returningIdx.empty()){
            continue;
        }

        try {
            // Smooth the data and test result
            fitter.smoothing_expt(&pulseData.returningWave);

            // Check parameter for using gaussian fitting or first differencing
            if (cmdLine.useGaussianFitting) {
                fitter.find_peaks(&peaks, pulseData.returningWave,
                        pulseData.returningIdx, 200);
            } else {
                fitter.guess_peaks(&peaks, pulseData.returningWave,
                        pulseData.returningIdx);
            }

            // for each peak - find the activation point
            //               - calculate x,y,z
            raw_data.calc_xyz_activation(&peaks);

            // for each peak we will call to_string and append them together
            results.insert(results.end(), peaks.begin(), peaks.end());
        } catch (const std::exception& e) {
            spdlog::error("Error processing data: {}", e.what());
        }
    }
  return results; 
}

std::string PlsToCsvHelper::getPeaksProperty(const std::vector<Peak*>& peaks, int productID){
    std::string results;
    for(const Peak* peak: peaks){
        std::string temp;
        peak->to_string(temp,{productID});
        results+=temp;
        results+=",";           
    }
    
    //this statement deletes the extra comma
    //at the end of the file
    if(!results.empty()){
        results.erase(results.end() -1);
    }

    return results;
}

bool PlsToCsvHelper::writeLinesToFile(std::string filename, std::vector<std::string> lines) {
    std::ofstream file (filename);
    if (file.is_open()) {
        for(std::string line: lines){
            file << line;
            file << '\n';
        }
    } else {
        //File could not be opened
        return false;
    }
    return true;
}


