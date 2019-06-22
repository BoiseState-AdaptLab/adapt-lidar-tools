/*
 * File Name: FittingInfoDriver.cpp
 * Created On: 14 June 2019
 * Author: Aaron Orenstein
*/

#include "FittingInfoDriver.hpp"

FittingInfoDriver::FittingInfoDriver(){
    printUsageMessage = false;
    lowerBound = 0;
    upperBound = INFINITY;
}

void FittingInfoDriver::writeData(FlightLineData &data, std::string out_name_1,
                                 std::string out_name_2){
    //Initialize necessary tools
    PulseData pd;
    GaussianFitter fitter;
    std::vector<Peak*> peaks;

    //Open output file
    std::ofstream rawFile(out_name_1);
    std::ofstream statsFile(out_name_2);

    //Holds number waveforms that had idx # of peaks
    std::vector<int> num_waves;

    //Holds the number of iterations required for waveforms with idx # of peaks
    std::vector<std::vector<size_t>> num_iters;

    //Keep track of the number waveforms
    int num_waveforms = 0;

    //Cycle through every pulse
    while(data.hasNextPulse()){
        //Clear our peaks vector just to be sure
        peaks.clear();

        //Get the next pulse/waveform
        data.getNextPulse(&pd);

        //If there is no data, skip the waveform
        if (pd.returningIdx.empty()) continue;

        try {
            //Smooth the data before performing fitting
            fitter.smoothing_expt(&pd.returningWave);

            //Reset fail counts
            fitter.fail = 0;

            //Test the upper bound.
            //If it fails, the waveform is not within our range of iterations
            fitter.find_peaks(&peaks, pd.returningWave, pd.returningIdx,
                             upperBound);
            if (fitter.fail != 0) continue;

            //Reset pass counts
            fitter.pass = 0;

            /*Increase the maximum number of iterations for gaussian fittings
             *until the wave is fit. start one below the lower bound so we
             *don't have to check all the iterations before that */
            size_t i;
            size_t peak_count;
            for (i = std::max(lowerBound - 1, 0); fitter.pass == 0; i++){
                peak_count = fitter.find_peaks(&peaks, pd.returningWave,
                                              pd.returningIdx, i);
            }

            //Check that the number of iterations is within our range
            if ((int) i < lowerBound || (int) i > upperBound) continue;


            //If we haven't had a waveform with this number of peaks befors,
            //add a new int for num peaks and vector for num iterations
            while (peak_count > num_waves.size()) {
                num_waves.push_back(0);
            }
            while (peak_count > num_iters.size()) {
                num_iters.push_back(std::vector<size_t>(0));
            }

            //Record peak_count and number of iterations for the waveform
            num_waves.at(peak_count - 1) ++;
            num_iters.at(peak_count - 1).push_back(i);

            num_waveforms ++;

            //Print waveform number and collumn labels
            rawFile << "Waveform " << num_waveforms  << std::endl;
            rawFile << "Time," << "Amplitude";

            for (size_t j = 1; j <= peak_count; j++){
                rawFile << ",Peak " << j;
            }

            rawFile << ",Number of Iterations: " << i << std::endl;

            //Write the data for each row
            for (size_t k = 0; k < pd.returningIdx.size(); k++){
                //Time index and amplitude from the data
                rawFile << pd.returningIdx.at(k) << "," <<
                    pd.returningWave.at(k);
                for (auto it = peaks.begin(); it != peaks.end(); ++it){
                    //Calculated amplitude for each peak
                    double val = (*it)->amp * exp(-0.5 * pow((
                        pd.returningIdx.at(k) - (*it)->location) /
                        ((*it)->fwhm / 2), 2));
                    rawFile << "," << std::to_string(val);
                }
                rawFile << std::endl;
            }
        } catch (const char* msg){
            std::cout << msg << std::endl;
        }

        rawFile << std::endl << std::endl;
    }
    //Close output file
    rawFile.close();

    //Write to stats file
    statsFile << "# Peaks,# Waveforms,Avg. Iterations" << std::endl;
    for (size_t i = 0; i < num_waves.size() && i < num_iters.size(); i ++){
        int avg = accumulate(num_iters.at(i).begin(), num_iters.at(i).end(),
            0)/num_iters.at(i).size();
        statsFile << i << "," << num_waves.at(i) << "," << avg << std::endl;
    }
    statsFile.close();
}

std::string FittingInfoDriver::parse_args(int argc, char *argv[]){
    std::vector<std::string> msgs;
    std::string file_name;

    char optChar;

    /* If the program is run without any command line arguments, display
     * the correct program usage and quit.*/
    if(argc < 2){
        msgs.push_back("No command line arguments");
        printUsageMessage = true;
    }

    /* Using getopt_long to get the arguments with an option.
     * ":hf:s:" indicate that option 'h' is without arguments while
     * option 'f' and 's' require arguments
   */
    while((optChar = getopt(argc, argv, ":hf:l:u:")) != -1){
        if (optChar == 'f'){
            file_name = optarg;
            check_input_file_exists(file_name, msgs);
        } else if (optChar == 'h'){
            printUsageMessage = true;
        } else if (optChar == 'l'){
            try {
                lowerBound = std::stoi(optarg);
            } catch (std::invalid_argument e){
                msgs.push_back("Failed to convert lower bound to an integer");
                printUsageMessage = true;
            }
        } else if (optChar == 'u'){
            try {
                upperBound = std::stoi(optarg);
            } catch (std::invalid_argument e){
                msgs.push_back("Failed to convert upper bound to an integer");
                printUsageMessage = true;
            }
        } else if (optChar == ':'){
            msgs.push_back("Missing arguments");
            printUsageMessage = true;
        } else {
            msgs.push_back(std::string("Invalid Argument: ") + optChar);
            printUsageMessage = true;
        }
    }

    if (upperBound < lowerBound){
        msgs.push_back("Upper bound must be greater than lower bound");
        printUsageMessage = true;
    }

    for (auto it = msgs.begin(); it != msgs.end(); ++it){
        std::string line(it->length(), '-');
        std::cout << "\n" << *it << "\n" << line << std::endl;
    }

    if (printUsageMessage){
        std::cout << getUsageMessage() << std::endl;
        return "";
    }
 
    return file_name;
}

/**
 * check if the input file exists, print error message if not
 */
void FittingInfoDriver::check_input_file_exists(std::string file_name,
                                                std::vector<std::string> msgs){
    std::string plsFileName = file_name;
    if (!std::ifstream(plsFileName.c_str())) {
        msgs.push_back(std::string("File ") + plsFileName +
            std::string(" not found."));
        printUsageMessage = true;
    }
    std::size_t idx = plsFileName.rfind(".pls");
    std::string wvsFileName = plsFileName.substr(0,idx) + ".wvs";
    if (!std::ifstream(wvsFileName.c_str())) {
        msgs.push_back(std::string("File ") + wvsFileName +
           std::string(" not found."));
        printUsageMessage = true;
    }
}

/**
 * get the input file name, stripped of leading path info and trailing
 * extension info
 * @param pls True returns pls file name, false returns wvs file name
 * @return input file name stripped of path or extension information
 */
std::string FittingInfoDriver::getTrimmedFileName(std::string name){
    size_t start = name.find_last_of("/");
    if(start==std::string::npos){
        start = 0;
    }else{
        start++;
    }
    size_t end = name.find_last_of(".");
    int len = end - start;
    return name.substr(start,len);
}

std::string FittingInfoDriver::getUsageMessage(){
    std::stringstream buffer;
    buffer << "\nUsage: " << std::endl;
    buffer << "       path_to_executable -f <path to pls file> "
        << "[-l lower-bound] [-u upper-bound]" << std::endl;
    buffer << "\nOptions: " << std::endl;
    buffer << "       -h: Prints this usage message" << std::endl;
    buffer << "       -f <path to pls file>: Sets the file to be used"
        << std::endl;
    buffer << "       -l <lower-bound>: Waveforms requiring a number of "
        << "iterations to be fit that is less than the lower bound will "
        << "not be reported" << std::endl;
    buffer << "       -u <upper-bound>: Waveforms requiring a number of "
        << "iterations to be fit that is greater than the upper bound "
        << "will not be reported" << std::endl;
    return buffer.str();
}
