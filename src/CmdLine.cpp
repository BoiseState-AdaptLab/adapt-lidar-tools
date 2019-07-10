// File name: CmdLine.cpp
// Created on: 17-May-2017
// Author: ravi


#include "CmdLine.hpp"

using namespace std;

//type is ((id - 1) % 6)
const static std::string product_type[6] = {"max", "min", "mean", "stdev", "skew",
    "kurt"};
//data used is floor(((id -1) % 18) / 6)
const static std::string product_data[3] = {"first", "last", "all"};
//variable is ((id - 1) / 18)
const static std::string product_variable[5] = {"elev", "amp", "width",
    "riseTime", "backscatter"};

/****************************************************************************
 *
 * Begin CmdLine functions
 *
 ****************************************************************************/

/**
 * Function that sets the command line arguments
 * @param args
 */
void CmdLine::setInputFileName(char *args){
    plsFileName = args;
    check_input_file_exists();
}


/**
 * Function that prints(sets up) correct usage of this program
 */
void CmdLine::setUsageMessage()
{
    std::stringstream buffer;
    buffer << "\nUsage: " << std::endl;
    buffer << "       path_to_executable -f <path to pls file> -product_type"
        << " <list of products> [-product_type <list_of_products>]* [-d]"
        << std::endl;
    buffer << std::endl;
    buffer << "Options:  " << std::endl;
    buffer << "       -f  <path to pls file>"
        << "  :Generates a Geotif file" << std::endl;
    buffer << "       -d"
        << "  :Disables gaussian fitter, using first diff method instead" << std::endl;
    buffer << "       -h"
        << "  :Prints this help message" << std::endl;
     buffer << std::endl;
    buffer << "Product Type Options:" << std::endl;
    buffer << "       -e  <list of products>"
        << "  :Generates Elevation products" << std::endl;
    buffer << "       -a  <list of products>"
        << "  :Generates Amplitude products" << std::endl;
    buffer << "       -w  <list of products>"
        << "  :Generates Width products" << std::endl;
    buffer << "       -r  <list of products>"
        << "  :Generates Rise Time products" << std::endl;
    buffer << "       -b  <list of products> <calibration constant>"
        << "  :Generates Backscatter Coefficient products with the given"
        << " calibration constant" << std::endl;
    buffer << "           Scientific notation allowed for calibration constant"
        << " (e.g. 0.78 = 7.8e-1 = 7.8E-1)"
        << std::endl;
    buffer << std::endl;
    buffer << "Product Options and Numbers:" << std::endl << std::endl;
    buffer << "| Calculation | Peaks Used | Product Number |" << std::endl;
    buffer << "|-------------|------------|----------------|" << std::endl;
    buffer << "| Max         | First      | 1              |" << std::endl;
    buffer << "| Min         | First      | 2              |" << std::endl;
    buffer << "| Mean        | First      | 3              |" << std::endl;
    buffer << "| Std.Dev     | First      | 4              |" << std::endl;
    buffer << "| Skewness    | First      | 5              |" << std::endl;
    buffer << "| Kurtosis    | First      | 6              |" << std::endl;
    buffer << "| Max         | Last       | 7              |" << std::endl;
    buffer << "| Min         | Last       | 8              |" << std::endl;
    buffer << "| Mean        | Last       | 9              |" << std::endl;
    buffer << "| Std.Dev     | Last       | 10             |" << std::endl;
    buffer << "| Skewness    | Last       | 11             |" << std::endl;
    buffer << "| Kurtosis    | Last       | 12             |" << std::endl;
    buffer << "| Max         | All        | 13             |" << std::endl;
    buffer << "| Min         | All        | 14             |" << std::endl;
    buffer << "| Mean        | All        | 15             |" << std::endl;
    buffer << "| Std.Dev     | All        | 16             |" << std::endl;
    buffer << "| Skewness    | All        | 17             |" << std::endl;
    buffer << "| Kurtosis    | All        | 18             |" << std::endl;
    buffer << std::endl;
    buffer << "Valid ways to format the product list include:" << std::endl;
    buffer << "                   -e 1,2,3           (no white-space)" << std::endl;
    buffer << "                   -e 1 -e 3 -e 2     (broken into multiple arguments)" << std::endl;
    buffer << "                   -e \" 1 , 2 , 3 \"   (white-space allowed inside quotes)" << std::endl;
    buffer << "\nExample: " << std::endl;
    buffer << "       bin/geotiff-driver -f ../etc/140823_183115_1_clipped_test.pls -e 1,2 -a 3,4,5 -w 14,9 -b 13,4 .768"
        << std::endl;
    usageMessage.append(buffer.str());
}


/**
 * Function that prints correct usage of this program
 * @return usage message
 */
std::string CmdLine::getUsageMessage(){
    return usageMessage;
}


/**
 * Default constructor
 */
CmdLine::CmdLine(){
    quiet = false;
    calibration_constant = 0;
    printUsageMessage = false;
    useGaussianFitting = true;
    calcBackscatter = false;
    exeName = "";
    setUsageMessage();
}


/**
 * Function that returns the file name
 * @param pls true returns pls file name, false returns wvs file name
 * @return the name of the input file
 */
std::string CmdLine::getInputFileName(bool pls){
    return pls ? plsFileName : wvsFileName;
}



/**
 * Function that parses the command line arguments
 * @param argc count of arguments
 * @param argv array of arguments
 * @param quiet true prints all messages, false prints no messages
 *        used in cmd line unittests
 * @return true if all arguments were valid, false otherwise
 */
bool CmdLine::parse_args(int argc,char *argv[]){
    std::vector<std::string> msgs;

    char optionChar;  //Option character
    char *fArg;       //Argument of the option character f
    char *e_arg;      //Argument of the option character e

    /* If the program is run without any command line arguments, display
     * the correct program usage and quit.*/
    if(argc < 2){
        msgs.push_back("No command line arguments");
        printUsageMessage = true;
    }

    exeName.append(argv[0]);

    static struct option long_options[] =
    {
        {"file", required_argument, NULL, 'f'},
        {"help", no_argument, NULL, 'h'},
        {"firstdiff", no_argument, NULL, 'd'},
        {"elevation", required_argument,NULL,'e'},
        {"amplitude", required_argument,NULL,'a'},
        {"width", required_argument,NULL,'w'},
        {"risetime", required_argument,NULL,'r'},
        {"backscatter", required_argument,NULL,'b'},
        {0, 0, 0, 0}
    };

    // getopt_long stores the option index here.
    int option_index = 0;
    //Tacks the last option used
    char lastOpt = ' ';
    /* Using getopt_long to get the arguments with an option.
     * ":hf:s:" indicate that option 'h' is without arguments while
     * option 'f' and 's' require arguments
     */
    while((optionChar = getopt_long (argc, argv, "-:hdf:e:a:w:r:b:",
                    long_options, &option_index))!= -1){
        if (optionChar == 'f') { //Set the filename to parse
            fArg = optarg;
            setInputFileName(fArg);
        } else if (optionChar == 'h') { //Show help information
            printUsageMessage = true;
        } else if (optionChar == 'd') { //Sets analysis method
            useGaussianFitting = false;
        } else if (optionChar == 'e' || optionChar == 'a' || optionChar == 'w'
            || optionChar == 'r' || optionChar == 'b'){
            //Sets which pruducts to create and for which variable
            { // Without curly braces wrapping this case, there are compilation
              //errors
                e_arg = optarg;
                std::stringstream ss(e_arg);
                int prod_offset = optionChar == 'a' ? 18 : optionChar == 'w' ?
                   36 : optionChar == 'r' ? 54 : optionChar == 'b' ? 72 : 0;
                while(ss.good()) {
                    string substr;
                    getline(ss, substr, ',');
                    try {
                        //If the inputted product number is above 18, stop now
                        int prod_num = stoi(substr.c_str());
                        if (prod_num > 18 || prod_num <= 0){
                            msgs.push_back(string("Invalid product code: ")
                                + substr);
                            printUsageMessage = true;
                        }
                        selected_products.push_back(prod_offset + prod_num);
                    } catch (std::invalid_argument e) {
                        msgs.push_back(string("Product list could not be") + 
                            string(" converted into integers"));
                        printUsageMessage = true;
                    } catch (std::out_of_range e) {
                        msgs.push_back(string("Invalid product code: ")
                            + substr);
                        printUsageMessage = true;
                    }
                }
                calcBackscatter = optionChar == 'b' ? true : calcBackscatter;
            }
        } else if (optionChar == 1 && lastOpt == 'b'
                  && calibration_constant == 0){ //Set calibration coefficient
            calibration_constant = std::atof(optarg);
            msgs.push_back("Calibration constant for backscatter coefficient = "
                + std::to_string(calibration_constant));
        } else if (optionChar == ':'){
            // Missing option argument
            msgs.push_back("Missing arguments");
            printUsageMessage = true;
        } else if (optionChar == '?'){
            //Invalid option
            msgs.push_back("Invalid option");
            printUsageMessage = true;
        } else {
            // Invalid option
            msgs.push_back(string("Invalid argument: ") + optarg);
            printUsageMessage = true;
        }
        lastOpt = optionChar;
    }
   
    //Backscatter coefficient requires a calibration constant
    if (calcBackscatter && calibration_constant == 0){
        msgs.push_back("Missing Calibration Constant");
        printUsageMessage = true;
    }

    // For non option input
    if(optind < argc){
        msgs.push_back("Invalid option");
        printUsageMessage = true;
    }

    //No file inputted
    if (getInputFileName(true) == ""){
        msgs.push_back("No file inputted");
        printUsageMessage = true;
    }

    // Make sure at least one product was selected
    if (selected_products.size() < 1){
        msgs.push_back("Select at least one product");
        printUsageMessage = true;
    }

    //Print messages
    if (!quiet){
        for (auto it = msgs.begin(); it != msgs.end(); ++it){
            std::string line(it->length(), '-');
            std::cout << "\n" << *it << "\n" << line << std::endl;
        }
    }

    // Check if an error occured
    if (printUsageMessage){
        //Make sure broken data is not used by clearing requested products
        selected_products.clear();
        if (!quiet) std::cout << getUsageMessage() << std::endl;
        return false;
    }
    
    return true;
}

/**
 * check if the input file exists, print error message if not
 */
void CmdLine::check_input_file_exists() {
    plsFileName = getInputFileName(true);
    if (!std::ifstream(plsFileName.c_str())) {
        if (!quiet) {
            std::cout << "\nFile " << plsFileName << " not found."
                << std::endl;
        }
        printUsageMessage = true;
    }
    std::size_t idx = plsFileName.rfind(".pls");
    wvsFileName = plsFileName.substr(0,idx) + ".wvs";
    if (!std::ifstream(wvsFileName.c_str())) {
        if (!quiet) {
            std::cout << "\nFile " << wvsFileName << " not found."
                << std::endl;
        }
        printUsageMessage = true;
    }
}

/**
 * get the input file name, stripped of leading path info and trailing
 * extension info
 * @param pls True returns pls file name, false returns wvs file name
 * @return input file name stripped of path or extension information
 */
std::string CmdLine::getTrimmedFileName(bool pls){
    size_t start = getInputFileName(pls).find_last_of("/");
    if(start==string::npos){
        start = 0;
    }else{
        start++;
    }
    size_t end = getInputFileName(pls).find_last_of(".");
    int len = end - start;
    return getInputFileName(pls).substr(start,len);
}

/**
 * get the output filename based on the command line arguments and input
 * filename
 * @param product_id the id of the product to produce
 * @return the output filename
 */
std::string CmdLine::get_output_filename(int product_id) {
    std::string output_filename = getTrimmedFileName(true);
    //Name file base on method used
    std::string file_type = ".tif";
    std::string fit_type = useGaussianFitting ? "_gaussian" : "_firstDiff";
    std::string prod_desc = "_" + get_product_desc(product_id);
    return output_filename +  prod_desc + fit_type + file_type;
}

/**
 * get the description of the product being produced
 * @param product_id the product id
 * @return the short description of the product
 */
std::string CmdLine::get_product_desc(int id){
    return product_type[(id - 1) % 6] + "_" + product_data[((id - 1) % 18) / 6]
        + "_" + product_variable[(id - 1) / 18];
}
