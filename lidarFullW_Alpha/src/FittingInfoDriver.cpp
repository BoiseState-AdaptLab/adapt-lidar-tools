/*
 * File Name: FittingInfoDriver.cpp
 * Created On: 14 June 2019
 * Author: Aaron Orenstein
*/

#include "FittingInfoDriver.hpp"

void FittingInfoDriver::writeData(FlightLineData &data, std::string out_file_name){
    PulseData pd;
    GaussianFitter fitter;
    std::vector<Peak*> peaks;

    //Open output file
    std::ofstream outFile(out_file_name);

    while(data.hasNextPulse()){
        peaks.clear();

        data.getNextPulse(&pd);

        if (pd.returningIdx.empty()) continue;

        try {
            fitter.pass = 0;

            fitter.smoothing_expt(&pd.returningWave);

            size_t i;
            int peak_count;
            for (i = 0; fitter.pass == 0; i++){
                peak_count = fitter.find_peaks(&peaks, pd.returningWave,
                                  pd.returningIdx, i);
            }

            outFile << "Time," << "Amplitude";

            for (int j = 1; j <= peak_count; j++){
                outFile << ",Peak " << j;
            }
            outFile << ",Number of Iterations: " << i << std::endl;

            int num_points = pd.returningIdx.size();
            for (int k = 0; k < num_points; k++){
                outFile << pd.returningIdx.at(k) << "," <<
                    pd.returningWave.at(k);
                for (auto it = peaks.begin(); it != peaks.end(); ++it){
                    double val = (*it)->amp * exp(-0.5 * pow((
                        pd.returningIdx.at(k) - (*it)->location) /
                        ((*it)->fwhm / 2), 2));
                    outFile << "," << std::to_string(val);
                }
                outFile << std::endl;
            }
        } catch (const char* msg){
            std::cout << msg << std::endl;
        }

        outFile << std::endl << std::endl;
    }
    //Close output file
    outFile.close();
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
    while((optChar = getopt(argc, argv, ":f:")) != -1){
        if (optChar == 'f'){
            file_name = optarg;
            check_input_file_exists(file_name, msgs);
        } else if (optChar == ':'){
            msgs.push_back("Missing arguments");
            printUsageMessage = true;
        } else {
            msgs.push_back(std::string("Invalid Argument: ") + optChar);
            printUsageMessage = true;
        }
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
    buffer << "       path_to_executable -f <path to pls file>"
        << std::endl;
    return buffer.str();
}
