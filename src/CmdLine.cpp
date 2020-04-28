#include <array>
#include <cassert>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

#include <getopt.h>

#include "spdlog/spdlog.h"

#include "CmdLine.hpp"
#include "PeakProducts.hpp"

bool CmdLine::parseArguments(int argc, char* argv[]){
    //https://linux.die.net/man/3/getopt_long
    constexpr std::array<struct option, 17> optionsArray{{
        //{"Long Name", argType, return method, return val}
        {"file",                required_argument, nullptr, 'f'},
        {"help",                no_argument,       nullptr, 'h'},
        {"elevation",           required_argument, nullptr, 'e'},
        {"amplitude",           required_argument, nullptr, 'a'},
        {"width",               required_argument, nullptr, 'w'},
        {"rise-time",           required_argument, nullptr, 'r'},
        {"backscatter",         required_argument, nullptr, 'b'},
        {"all",                 no_argument,       nullptr, 'l'},
        {"calibration",         required_argument, nullptr, 'c'},
        {"noise-level",         required_argument, nullptr, 'n'},
        {"verbosity",           required_argument, nullptr, 'v'},
        {"number-threads",      required_argument, nullptr, 't'},
        {"waves-per-thread",    required_argument, nullptr, 'p'},
        {"min-peak-amp",        required_argument, nullptr, 'm'},
        {"no-smoothing",        no_argument,       nullptr, 's'},
        {"no-noise-reduction",  no_argument,       nullptr, 'o'},
        {0,                     0,                 nullptr, 0}
    }};

    //The first char '-' means @@TODO needed
    //The second char ':' tell getopt to not print an error message when it encounters a missing argument. It also returns ':' instead of '?' to indicate this. 
    //a letter followed by a ':' means an argument is required. A letter followed by '::' means the argument is optional.
    //Please keep this in the same order as the array.
    const std::string optionsString = ":f:he:a:w:r:b:ln:v:t:p:m:so";

    //Please keep the order of the lambdas below in the same order as the array above
    //Maps an option character to a function to execute. The function returns true if the argument was successfully parsed, false if an error occurred. 
    std::unordered_map<int, std::function<bool(std::string arg)>> argumentMap;

    //-----Begin Argument Functions-----

    //Set filename
    argumentMap['f'] = [&](std::string arg){
        return setInputFile(arg);
    };

    //We handle help separately since we stop when we encounter it, but it isn't an error.

    //Add elevation products
    argumentMap['e'] = [&](std::string arg){
        if(parseProducts(arg, PeakProducts::Property::Elevation)) return true;
        spdlog::critical("Error parsing Elevation products. Argument: \"{}\"", arg);
        return false;
    };

    //Add amplitude products
    argumentMap['a'] = [&](std::string arg){
        if(parseProducts(arg, PeakProducts::Property::Elevation)) return true;
        spdlog::critical("Error parsing Amplitude products. Argument: \"{}\"", arg);
        return false;
    };

    //Add pulsewidth products
    argumentMap['w'] = [&](std::string arg){
        if(parseProducts(arg, PeakProducts::Property::Elevation)) return true;
        spdlog::critical("Error parsing Width products. Argument: \"{}\"", arg);
        return false;
    };

    //Add RiseTime products
    argumentMap['r'] = [&](std::string arg){
        if(parseProducts(arg, PeakProducts::Property::Elevation)) return true;
        spdlog::critical("Error parsing Rise Time products. Argument: \"{}\"", arg);
        return false;
    };

    //Add backscatter products
    argumentMap['b'] = [&](std::string arg){
        if(parseProducts(arg, PeakProducts::Property::Elevation)) return true;
        spdlog::critical("Error parsing backscatter products. Argument: \"{}\"", arg);
        fitterOptions.calcBackscatter = true;
        return false;
    };

    //Add all products
    argumentMap['l'] = [&](std::string){
        for(const auto& iter : productMap){                             //Stat             Subset
            products.emplace_back(PeakProducts::Property::Amplitude,    iter.second.first, iter.second.second);
            products.emplace_back(PeakProducts::Property::Backscatter,  iter.second.first, iter.second.second);
            products.emplace_back(PeakProducts::Property::Elevation,    iter.second.first, iter.second.second);
            products.emplace_back(PeakProducts::Property::RiseTime,     iter.second.first, iter.second.second);
            products.emplace_back(PeakProducts::Property::Width,        iter.second.first, iter.second.second);
        }
        fitterOptions.calcBackscatter = true;

        return true;
    };

    //Set noise level
    argumentMap['n'] = [&](std::string arg){
        int noise = -1;
        if(!tryParseInteger(arg, noise, 0)){
            spdlog::critical("Error parsing argument \"{}\" as integer. Valid range is 0 to int_max");
            return false;
        }

        fitterOptions.noiseLevel = noise;
        return true;
    };

    //Set verbosity
    argumentMap['v'] = [&](std::string arg){
        if(setVerbosity(arg)) return true;
        spdlog::critical("Invalid verbosity \"{}\". Use \"--help\" to see valid options", arg);
        return true;
    };

    //Set number of threads
    argumentMap['t'] = [&](std::string arg){
        int numThreads = -1;
        if(!tryParseInteger(arg, numThreads, 1)){
            spdlog::critical("Error parsing argument \"{}\" as integer. Valid range is 1 to int_max");
            return false;
        }

        fitterOptions.numThreads = numThreads;
        return true;
    };

    //Set number of waves per thread
    argumentMap['p'] = [&](std::string arg){
        int wavesPerThread = -1;
        if(!tryParseInteger(arg, wavesPerThread, 1)){
            spdlog::critical("Error parsing argument \"{}\" as integer. Valid range is 1 to int_max");
            return false;
        }

        fitterOptions.wavesPerThread = wavesPerThread;
        return true;
    };

    //Set min peak amplitude
    argumentMap['m'] = [&](std::string arg){
        int minPeakAmp = -1;

        if(!tryParseInteger(arg, minPeakAmp, 1)){
            spdlog::critical("Error parsing argument \"{}\" as integer. Valid range is 1 to int_max");
            return false;
        }

        fitterOptions.minPeakAmp = minPeakAmp;
        return true;
    };

    //Disable smoothing
    argumentMap['s'] = [&](std::string){
        fitterOptions.smoothData = false;
        return true;
    };

    //Disable noise reduction
    argumentMap['o'] = [&](std::string){
        fitterOptions.reduceNoise = false;
        return true;
    };

    //-----End Argument Functions-----

    
    opterr = 0; //Don't print error messages
    int optionCode = -1;
    while((optionCode = getopt_long(argc, argv, optionsString.c_str(), optionsArray.data(), nullptr)) != -1){
        std::string arg;
        if(optarg){ //If we have an argument, put it into arg for use later.
            arg = std::string(optarg);
        }

        switch(optionCode){
            case 'h':   //Handle help separately, since it's not really an error but we need to stop parsing.
                spdlog::info(getUsageMessage());
                spdlog::info(getAdvancedUsageMessage());
                return false;
            case ':':   //Missing argument
                spdlog::critical("Missing argument for option -\'{}\'. Use --help for more information", static_cast<char>(optionCode));
                return false;
            case '?':   //Unknown option
                spdlog::critical("Invalid Option -\'{}\'. Use --help for more information", static_cast<char>(optionCode));
                return false;
            default:
                if(argumentMap.count(optionCode) == 0){
                    spdlog::critical("Invalid Option -\'{}\'. Use --help for more information", static_cast<char>(optionCode));
                    return false;
                }

                if(!argumentMap.at(optionCode)(arg)){   //Call the function associated with the option.
                    spdlog::critical("Parsing option -\'{}\' failed. Use --help to see usage.", static_cast<char>(optionCode));
                    return false;
                }

                //Argument parsed successfully, continue
                break;
        }
    }
    
    //Validate that we have everything we need
    
    if(fileName.empty()){   //No input file
        spdlog::critical("No input file specified. Use -f to specify a file. --help for more information");
        return false;
    }

    //If fitterOptions.calcBackscatter == true, then fitterOptions.calibrationConstant should've been set to something else.
    //Must be set when -b or -l is used. 
    if(fitterOptions.calcBackscatter && fitterOptions.calibrationConstant == std::numeric_limits<double>::min()){
        spdlog::critical("No backscatter coefficient specified. Specify it with -c <coeff>. Use --help for more details");
        return false;
    }

    if(products.empty()){
        spdlog::critical("No products specified. Use --help to see available products");
        return false;
    }

    if(fitterOptions.minPeakAmp <= fitterOptions.noiseLevel){
        spdlog::critical("The minimum amplitude ({}) must be higher than the noise level ({})", fitterOptions.minPeakAmp, fitterOptions.noiseLevel);
    }

    return true;
}

std::string CmdLine::getPLSFilename() const{
    if(fileName.empty()) return "";
    return fileName + ".pls";
}

std::string CmdLine::getWVSFilename() const{
    if(fileName.empty()) return "";
    return fileName + ".wvs";
}

std::string CmdLine::getTrimmedFileName() const{
    return fileName;
}


//Private members & functions

//Map string verbosities to spdlog enums
const std::unordered_map<std::string, spdlog::level::level_enum> CmdLine::verbosityMap{
    {"trace",   spdlog::level::trace},
    {"debug",   spdlog::level::debug},
    {"info",    spdlog::level::info},
    {"warn",    spdlog::level::warn},
    {"error",   spdlog::level::err},
    {"critical",spdlog::level::critical}
};

//Map product ids to actual product stats and subsets
const std::unordered_map<int, std::pair<PeakProducts::Statistic, PeakProducts::Subset>> CmdLine::productMap{
    { 1, {PeakProducts::Statistic::Maximum,              PeakProducts::Subset::First}},
    { 2, {PeakProducts::Statistic::Minimum,              PeakProducts::Subset::First}},
    { 3, {PeakProducts::Statistic::Average,              PeakProducts::Subset::First}},
    { 4, {PeakProducts::Statistic::StandardDeviation,    PeakProducts::Subset::First}},
    { 5, {PeakProducts::Statistic::Skewness,             PeakProducts::Subset::First}},
    { 6, {PeakProducts::Statistic::Kurtosis,             PeakProducts::Subset::First}},
    { 7, {PeakProducts::Statistic::Maximum,              PeakProducts::Subset::Last}},
    { 8, {PeakProducts::Statistic::Minimum,              PeakProducts::Subset::Last}},
    { 9, {PeakProducts::Statistic::Average,              PeakProducts::Subset::Last}},
    {10, {PeakProducts::Statistic::StandardDeviation,    PeakProducts::Subset::Last}},
    {11, {PeakProducts::Statistic::Skewness,             PeakProducts::Subset::Last}},
    {12, {PeakProducts::Statistic::Kurtosis,             PeakProducts::Subset::Last}},
    {13, {PeakProducts::Statistic::Maximum,              PeakProducts::Subset::All}},
    {14, {PeakProducts::Statistic::Minimum,              PeakProducts::Subset::All}},
    {15, {PeakProducts::Statistic::Average,              PeakProducts::Subset::All}},
    {16, {PeakProducts::Statistic::StandardDeviation,    PeakProducts::Subset::All}},
    {17, {PeakProducts::Statistic::Skewness,             PeakProducts::Subset::All}},
    {18, {PeakProducts::Statistic::Kurtosis,             PeakProducts::Subset::All}}
};


std::string CmdLine::getUsageMessage(){
return R"DELIM(
Usage:
    geotiff-driver -f <path to pls file> -product_type <list of products> [-product_type <list_of_products>]* [-d]

Example:
    bin/geotiff-driver -f etc/Nayani_clipped_test.pls -e 1,2,3 -w 14,9 -b 13,4 -c 0.768

Options:
    -f  <path to pls file>
        Path to the pls file. Will determine the output name.
    -h
        Prints this help message.

Product Type Options:
    -e, --elevation  <list of products>
        Generates Elevation products;
    -a, --amplitude  <list of products>
        Generates Amplitude products
    -w, --width  <list of products>
        Generates Width products 
    -r, --rise-time  <list of products>
        Generates Rise Time products
    -b, --backscatter  <list of products>
        Generates Backscatter Coefficient products. Must specify the calibration constant with -c
    -l, --all
        Generates all products for every variable. Must also specify the calibration constant with -c. 
        Calibration constant is used for backscatter coefficient calculations
    -c, --calibration <calibration constant>
        Calibration constant to use when calculating backscatter. Required when using -b/--backscatter or -l/--all
        Note: Scientific notation is allowed (e.g. 0.78 = 7.8e-1 = 7.8E-1)

Product Options and Numbers:

-----------------------------------------
| Calculation | Peaks Used  | Product # |
|-------------|------------ |-----------|
| All Options | All Options |  0        |
| Max         | First       |  1        |
| Min         | First       |  2        |
| Mean        | First       |  3        |
| Std.Dev     | First       |  4        |
| Skewness    | First       |  5        |
| Kurtosis    | First       |  6        |
| Max         | Last        |  7        |
| Min         | Last        |  8        |
| Mean        | Last        |  9        |
| Std.Dev     | Last        | 10        |
| Skewness    | Last        | 11        |
| Kurtosis    | Last        | 12        |
| Max         | All         | 13        |
| Min         | All         | 14        |
| Mean        | All         | 15        |
| Std.Dev     | All         | 16        |
| Skewness    | All         | 17        |
| Kurtosis    | All         | 18        |
-----------------------------------------

Valid ways to format the product list include:
    -e 1,2,3           (no white-space)
    -e 1 -e 3 -e 2     (broken into multiple arguments)
    -e " 1 , 2 , 3 "   (white-space allowed inside quotes)

)DELIM";
}

std::string CmdLine::getAdvancedUsageMessage(){
return R"DELIM(
Advanced Options:
    -n, --noise-level <noise level>
        Set the noise level. The default value is 6. Integer.
    -v, --verbosity <logging level>
        Set the logging level. Valid levels are: "trace", "debug", "info", "warn", "error", and "critical"
    -t, --number-threads <number of threads>
        Set the number of threads to be used. Defaults to 8. Integer.
    -p, --waves-per-thread <waves per thread>
        Sets the number of waves per thread. Defaults to 50. Integer.
    -m, --min-peak-amp <min peak amplitude (int)>
        Sets the minimum allowed height for a peak. Peaks under this height will not be identified. Must be higher than the noise level. Defaults to 9. Integer.
    -s, --no-smoothing
        Disable the 3-point scaled moving average smoother.
    -o, --no-noise-reduction
        Disable the scaled noise reduction.
)DELIM";
}

bool CmdLine::setVerbosity(const std::string& str){
    if(verbosityMap.count(str) == 0) return false;
    spdlog::set_level(verbosityMap.at(str));
    return true;
}


bool CmdLine::setInputFile(const std::string& fileName){
    //Check if file exists
    if(!std::ifstream(fileName).good()){
        spdlog::critical("File {} does not exist", fileName);
        return false;
    }

    //Check if file has correct extension
    if(fileName.rfind(".pls") != fileName.size()-4){
        spdlog::critical("File must end in .pls");
        return false;
    }

    this->fileName = fileName.substr(0, fileName.size()-4);

    //Check for matching .wvs file
    if(!std::ifstream(getWVSFilename()).good()){
        spdlog::critical("Missing matching .wvs file named \"{}\"", getWVSFilename());
        this->fileName = "";
        return false;
    }

    return true;
}

bool CmdLine::parseProducts(std::string productList, PeakProducts::Property property){
    std::vector<PeakProducts::Product> newProducts;

    std::stringstream ss(productList);  //Allow us to easily split by a delimiter (',')
    while(ss.good()){
        std::string str;
        std::getline(ss, str, ',');

        int productId = -1;
        if(!tryParseInteger(str, productId, 1, productMap.size())){
            spdlog::critical("Unable to parse argument \'{}\' as integer in range [1, {}]", str, productMap.size());
            return false;
        }

        assert(productMap.count(productId) != 0);

        newProducts.emplace_back(property, productMap.at(productId).first, productMap.at(productId).second);
    }

    products.insert(products.begin(), newProducts.begin(), newProducts.end());
    return true;
}


bool CmdLine::tryParseInteger(const std::string& integer, int& outInt, int lowerBound, int upperBound){
    try{
        int val = std::stoi(integer);
        if(lowerBound <= val && val <= upperBound){
            outInt = val;
            return true;
        }else{
            spdlog::critical("int with value {} out of bound range: [{}, {}]", val, lowerBound, upperBound);
        }
    }catch(const std::invalid_argument& e){
        spdlog::critical("Unable to convert {} to type int", integer);
    }catch(const std::out_of_range& e){
        spdlog::critical("Number {} exceeded the capacity of type int", integer);
    }

    return false;
}
