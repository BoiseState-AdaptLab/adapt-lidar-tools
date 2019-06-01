// File name: CmdLine.cpp
// Created on: 17-May-2017
// Author: ravi


#include "CmdLine.hpp"

using namespace std;


//type is ((id - 1) % 6)
const static std::string product_type[6] = {"max", "min", "mean", "sd", "skew",
    "kurt"};
//data used is floor(((id -1) % 18) / 6)
const static std::string product_data[3] = {"first", "last", "all"};
//variable is ((id - 1) / 18)
const static std::string product_variable[4] = {"elev", "amp", "width",
    "backscatter"};

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
    inputFileName = args;
}


/**
 * Function that prints(sets up) correct usage of this program
 */
void CmdLine::setUsageMessage()
{
    std::stringstream buffer;
    buffer << "\nUsage: " << std::endl;
    buffer << "       path_to_executable -f <path to pls file> [-option argument] <list of products>" << std::endl;
    buffer << "Option:  " << std::endl;
    buffer << "       -f  <path to pls file>"
        << "  :Generate a Geotif file" << std::endl;
    buffer << "Option:  " << std::endl;
    buffer << "       -e  <list of products>"
        << "  :Generate elevation products. Choose products to generate from the following table;" << std::endl;
    buffer << std::endl;
    buffer << "| Function | Peak Type | Product Number |" << std::endl;
    buffer << "|----------|-----------|----------------|" << std::endl;
    buffer << "| Max      | First     | 1              |" << std::endl;
    buffer << "| Min      | First     | 2              |" << std::endl;
    buffer << "| Mean     | First     | 3              |" << std::endl;
    buffer << "| Std.Dev  | First     | 4              |" << std::endl;
    buffer << "| Skew     | First     | 5              |" << std::endl;
    buffer << "| Kurtosis | First     | 6              |" << std::endl;
    buffer << "| Max      | Last      | 7              |" << std::endl;
    buffer << "| Min      | Last      | 8              |" << std::endl;
    buffer << "| Mean     | Last      | 9              |" << std::endl;
    buffer << "| Std.Dev  | Last      | 10             |" << std::endl;
    buffer << "| Skew     | Last      | 11             |" << std::endl;
    buffer << "| Kurtosis | Last      | 12             |" << std::endl;
    buffer << "| Max      | All       | 13             |" << std::endl;
    buffer << "| Min      | All       | 14             |" << std::endl;
    buffer << "| Mean     | All       | 15             |" << std::endl;
    buffer << "| Std.Dev  | All       | 16             |" << std::endl;
    buffer << "| Skew     | All       | 17             |" << std::endl;
    buffer << "| Kurtosis | All       | 18             |" << std::endl;
    buffer << std::endl;
    buffer << "Valid ways to format the list include:" << std::endl;
    buffer << "                   -e 1,2,3           (no white-space)" << std::endl;
    buffer << "                   -e 1 -e 3 -e 2     (broken into multiple arguments)" << std::endl;
    buffer << "                   -e \" 1 , 2 , 3 \"   (white-space allowed inside quotes)" << std::endl;
    buffer << "Option:  " << std::endl;
    buffer << "       -a  <list of products>"
        << "  :Generate Amplitude products" << std::endl;
    buffer << "Option:  " << std::endl;
    buffer << "       -w  <list of products>"
        << "  :Generate Width products" << std::endl;
    buffer << "Option:  " <<std::endl;
    buffer << "       -b  <list of products> -c <calibration constant>"
        << "  :Generate Backscatter Coefficient products with the given"
        << " calibration constant" << std::endl;
    buffer << "Option:  " << std::endl;
    buffer << "       -d"
        << "  :Disables gaussian fitter, using first diff method instead" << std::endl;
    buffer << "       -h"
        << "  :Print this help message" << std::endl;
    buffer << "\nExample: " << std::endl;
    buffer << "       bin/geotiff-driver -f ../etc/140823_183115_1_clipped_test.pls -e 1,2 -a 3,4,5 -w 14,9"
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
    calibration_constant = 0;
    printUsageMessage = false;
    useGaussianFitting = true;
    exeName = "";
    setUsageMessage();
}


/**
 * Function that returns the file name
 * @return the name of the input file
 */
std::string CmdLine::getInputFileName(){
    return inputFileName;
}



/**
 * Function that parses the command line arguments
 * @param argc count of arguments
 * @param argv array of arguments
 */
void CmdLine::parse(int argc,char *argv[]){
    char optionChar;  //Option character
    char *fArg;       //Argument of the option character f
    char *e_arg;      //Argument of the option character e

    /* If the program is run without any command line arguments, display
     * the correct program usage and quit.*/
    if(argc < 2){
        std::cout << "\nNo command line arguments" <<std::endl;
        std::cout << "-------------------------" <<std::endl;
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
        {"backscatter", required_argument,NULL,'b'},
        {0, 0, 0, 0}
    };

    // getopt_long stores the option index here.
    int option_index = 0;
    //Keeps track of if backscatter coefficient is requested
    bool need_backscatter = false;
    /* Using getopt_long to get the arguments with an option.
     * ":hf:s:" indicate that option 'h' is without arguments while
     * option 'f' and 's' require arguments
     */
    while((optionChar = getopt_long (argc, argv, ":hdf:e:a:w:b:c:",
                    long_options, &option_index))!= -1){
        if (optionChar == 'f') { //Set the filename to parse
            fArg = optarg;
            setInputFileName(fArg);
        } else if (optionChar == 'h') { //Show help information
            printUsageMessage = true;
        } else if (optionChar == 'd') { //Sets analysis method
            useGaussianFitting = false;
        } else if (optionChar == 'e' || optionChar == 'a' || optionChar == 'w'
            || optionChar == 'b'){
            //Sets which pruducts to create and for which variable
            { // Without curly braces wrapping this case, there are compilation
              //errors
                e_arg = optarg;
                std::stringstream ss(e_arg);
                while(ss.good()) {
                    string substr;
                    getline(ss, substr, ',');
                    int arg;
                    try {
                        arg = (optionChar == 'a' ? 18 : optionChar == 'w' ? 36 :
                            optionChar == 'b' ? 54 : 0) + atoi(substr.c_str());
                    } catch (std::invalid_argument e) {
                        std::cout << "\nProduct list could not be converted into integers" <<std::endl;
                        std::cout << "-------------------------" <<std::endl;
                        printUsageMessage = true;
                    }

                    //Just making sure it doesn't try pushing broken data to 
                    //selected_products
                    if (printUsageMessage) {
                        break;
                    }
                    selected_products.push_back(arg);
                }
                if (optionChar == 'b'){
                    need_backscatter = true;
                }
            }
        } else if (optionChar == 'c'){ //Set calibration coefficient
            calibration_constant = std::atof(optarg);
        } else if (optionChar == ':'){
            // Missing option argument
            std::cout << "\nMissing arguments" <<std::endl;
            std::cout << "------------------" <<std::endl;
            printUsageMessage = true;
        } else {
            // Invalid option
            std::cout << "\nInvalid option" <<std::endl;
            std::cout << "---------------" <<std::endl;
            printUsageMessage = true;
        }
    }
    
    //Make sure that if the backscatter coefficient was requested,
    //the calibration coefficient was inputted
    if (need_backscatter && calibration_constant == 0){
        std::cout << "\nMissing Calibration Coefficient" << std::endl;
        std::cout << "---------------------------------" << std::endl;
        printUsageMessage = true;
    }

    // For non option input
    if(optind < argc){
        printUsageMessage = true;
    }
}

/**
 * check if the input file exists, print error message if not
 */
void CmdLine::check_input_file_exists() {
    if (!std::ifstream(getInputFileName().c_str())) {
        std::cout << "File " << getInputFileName() << " not found."
            << std::endl;
        printUsageMessage = true;
    }
}


/**
 * parse the command line arguments and validate them, return
 * 1 if all arguments parsed, else 0
 * @param argc the count of arguments in argv
 * @param argv the command line arguments
 * @return 1 (true) if all input passed validation, 0 (false) otherwise
 */
int CmdLine::parse_args(int argc, char *argv[]) {
    int rtn = 1;
    parse(argc, argv);
    //if arguments were invalid or did not parse correctly
    if (printUsageMessage) {
        std::cout << getUsageMessage() << std::endl;
        rtn = 0;
    }else{
        check_input_file_exists();
        if (printUsageMessage) {
            std::cout << getUsageMessage() << std::endl;
            rtn = 0;
        }
    }
    if(!printUsageMessage){
        //make sure some product was selected
        if(selected_products.size()<1){
            printUsageMessage = true;
            std::cout << "No output products selected." << std::endl;
            std::cout << getUsageMessage() << std::endl;
            rtn = 0;
        }
    }
    return rtn;
}

/**
 * get the input file name, stripped of leading path info and trailing
 * extension info
 * @return input file name stripped of path or extension information
 */
std::string CmdLine::getTrimmedFileName(){
    size_t start = inputFileName.find_last_of("/");
    if(start==string::npos){
        start = 0;
    }else{
        start++;
    }
    size_t end = inputFileName.find_last_of(".");
    int len = end - start;
    return inputFileName.substr(start,len);
}

/**
 * get the output filename based on the command line arguments and input
 * filename
 * @param product_id the id of the product to produce
 * @return the output filename
 */
std::string CmdLine::get_output_filename(int product_id) {
    std::string output_filename = getTrimmedFileName();
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
