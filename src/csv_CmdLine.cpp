// File name: csv_CmdLine.cpp
// Created on: 16 July 2019
// Author: Spencer Fleming


#include "csv_CmdLine.hpp"

using namespace std;

//type is ((id - 1) % 6)
const static std::string peakvars[10] = {
    "Amplitude", "Location", "Width", "Is Final Peak", "Position in Wave",
    "Triggering Amp", "Triggering Location", "Peak Location",
    "Triggering Location", "Samples"};

/****************************************************************************
 *
 * Begin CmdLine functions
 *
 ****************************************************************************/

/**
 * Function that sets the command line arguments
 * @param args
 */
void csv_CmdLine::setInputFileName(char *args){
    //Check if the filename is a txt or a pls
    plsFileName = args;
    size_t begin = plsFileName.find_last_of(".");
    if (begin == string::npos) {
        std::cerr << "File not specified or has no extension" << std::endl;
        printUsageMessage = true;
        return;
    }

    size_t len = plsFileName.size() - begin;
    std::string file_ext = plsFileName.substr(begin, len);

    if (file_ext == ".pls") {
        check_input_file_exists();
    } else if (file_ext == ".txt") {
        is_txt = true;
        check_input_txt_exists();
    } else {
        std::cerr << "Not a supported filetype" << std::endl;
        printUsageMessage = true;
    }
}


/**
 * Function that prints(sets up) correct usage of this program
 */
void csv_CmdLine::setUsageMessage()
{
    std::stringstream buffer;
    buffer << "\nUsage: " << std::endl;
    buffer << "       path_to_executable -f <path to pls file> -p"
        << " <list of peak variables> [-d]"
        << std::endl;
    buffer << std::endl;
    buffer << "Options:  " << std::endl;
    buffer << "       -f  <path to pls/txt file>"
        << "  :Reads pls file for peak data" << std::endl;
    buffer << "       -d"
        << "  :Disables gaussian fitter, using first diff method instead" << std::endl;
    buffer << "       -h"
        << "  :Prints this help message" << std::endl;
    buffer << "       -p "
        << "  :Writes peak data to CSV" << std::endl;
    buffer << "       -l "
        << "  :Logs extra diagnostics information about peaks" << std::endl;
    buffer << std::endl;
    buffer << "Peak variable options and letters:" << std::endl << std::endl;
    buffer << "| Variable                | Number | Description " << std::endl;
    buffer << "|-------------------------|--------|-------------" << std::endl;
    buffer << "| Amplitude               | 1      | Amplitude of Peak" << std::endl;
    buffer << "| Location                | 2      | Time of Peak occurance" << std::endl;
    buffer << "| Width                   | 3      | Width at half-max amplitude" << std::endl;
    buffer << "| Is Final Peak           | 4      | Whether or not peak is last in wave" << std::endl;
    buffer << "| Position in Wave        | 5      | Number represening peak position" << std::endl;
    buffer << "| Triggering Amp          | 6      | Amplitude at inflection point" << std::endl;
    buffer << "| Triggering Location     | 7      | Time of inflection point occurance" << std::endl;
    buffer << "| Peak x, y, z            | 8      | x, y, z coordinate of peak" << std::endl;
    buffer << "| Triggering x, y, z      | 9      | x, y, z of inflection point" << std::endl;
    buffer << "| Samples                 | 10     | Number of Gaussian Fitter iterations used on peak" << std::endl;
    buffer << std::endl;
    buffer << "Valid ways to format the product list include:" << std::endl;
    buffer << "                   -p 1,2,3           (no white-space)" << std::endl;
    buffer << "                   -p 1 -e 3 -e 2     (broken into multiple arguments)" << std::endl;
    buffer << "                   -p \" 1 , 2 , 3 \"   (white-space allowed inside quotes)" << std::endl;
    buffer << "\nExample: " << std::endl;
    buffer << "       bin/csvDriver -f ../etc/140823_183115_1_clipped_test.pls -p 1,2,3"
        << std::endl;
    usageMessage.append(buffer.str());
}


/**
 * Function that prints correct usage of this program
 * @return usage message
 */
std::string csv_CmdLine::getUsageMessage(){
    return usageMessage;
}


/**
 * Default constructor
 */
csv_CmdLine::csv_CmdLine(){
    quiet = false;
    is_txt = false;
    printUsageMessage = false;
    useGaussianFitting = true;
    log_diagnostics = false;
    exeName = "";
    setUsageMessage();
}


/**
 * Function that returns the file name
 * @param pls true returns pls file name, false returns wvs file name
 * @return the name of the input file
 */
std::string csv_CmdLine::getInputFileName(bool pls){
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
bool csv_CmdLine::parse_args(int argc,char *argv[]){
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
        {"peaks", required_argument,NULL,'p'},
        {"log-diag", no_argument, NULL, 'l'},
        {0, 0, 0, 0}
    };

    // getopt_long stores the option index here.
    int option_index = 0;
    /* Using getopt_long to get the arguments with an option.
     * ":hf:s:" indicate that option 'h' is without arguments while
     * option 'f' and 's' require arguments
     */
    while((optionChar = getopt_long (argc, argv, "-:hdf:p:l",
                    long_options, &option_index))!= -1){
        if (optionChar == 'f') { //Set the filename to parse
            fArg = optarg;
            setInputFileName(fArg);
        } else if (optionChar == 'h') { //Show help information
            printUsageMessage = true;
        } else if (optionChar == 'd') { //Sets analysis method
            useGaussianFitting = false;
        } else if (optionChar == 'l') {//Sets log_diagnostics
            log_diagnostics = true;
        } else if (optionChar == 'p') {
            //Sets which pruducts to create and for which variable
            { // Without curly braces wrapping this case, there are compilation
              //errors
                e_arg = optarg;
                std::stringstream ss(e_arg);
                while(ss.good()) {
                    string substr;
                    getline(ss, substr, ',');
                    try {
                        int prod_num = stoi(substr.c_str());
                        if (prod_num > 10 || prod_num <= 0){
                            msgs.push_back(string("Invalid product code: ")
                                + substr);
                            printUsageMessage = true;
                        }
                        selected_products.push_back(prod_num);
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
            }
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
void csv_CmdLine::check_input_file_exists() {
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
 * check if the input file exists, print error message if not
 */
void csv_CmdLine::check_input_txt_exists() {
    if (!std::ifstream(plsFileName.c_str())) {
        if (!quiet) {
            std::cout << "\nFile " << plsFileName << " not found."
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
std::string csv_CmdLine::getTrimmedFileName(bool pls){
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
std::string csv_CmdLine::get_output_filename(int product_id) {
    std::string output_filename = getTrimmedFileName(true);
    //Name file base on method used
    std::string file_type = ".csv";
    std::string fit_type = useGaussianFitting ? "_gaussian" : "_firstDiff";
    std::string prod_desc = "_" + get_product_desc(product_id);
    return output_filename +  prod_desc + fit_type + file_type;
}


/**
 * get the description of the product being produced
 * @param product_id the product id
 * @return the short description of the product
 */
std::string csv_CmdLine::get_product_desc(int id){
    return peakvars[id-1];
}
