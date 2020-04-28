#ifndef ADAPTLIDARTOOLS_CMDLINE_HPP
#define ADAPTLIDARTOOLS_CMDLINE_HPP

#include <limits>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "spdlog/spdlog.h"

#include "Common.hpp"
#include "PeakProducts.hpp"

class CmdLine{
public:
    //Contains the list of all specified products. Not guaranteed to be unique @@TODO maybe it should
    std::vector<PeakProducts::Product> products;

    //Options for the fitting and processing.
    Common::Options fitterOptions;


    /** Parse the command line arguments. 
     * @param argc      Argument Count
     * @param argv      c-array of c-strings of arguments
     * @return          true if program execution should continue, false otherwise. This could mean an error in option parsing or the --help option was called.
     */
    bool parseArguments(int argc, char* argv[]);

    /** Returns the full path to the .pls file
     * @return the path to the .pls file
     */
    std::string getPLSFilename() const;

    /** Returns the full path to the .wvs file.
     * @return the path to the .wvs file
     */
    std::string getWVSFilename() const;

    /** Get the filename without an extension, but with the leading path.
     * @return trimmed file name
     */
    std::string getTrimmedFileName() const;


private:
    //Map string verbosity to actual spdlog verbosity enum
    const static std::unordered_map<std::string, spdlog::level::level_enum> verbosityMap;

    //Map product id(from the product table) to corresponding statistic and subset pair
    const static std::unordered_map<int, std::pair<PeakProducts::Statistic, PeakProducts::Subset>> productMap;

    //Filename without extension, but including path
    std::string fileName;

    /** Returns the regular usage message.
     * @return The normal usage message.
     */
    static std::string getUsageMessage();

    /** Returns the advanced usage message. Note that this doesn't include the text returned by getUsageMessage(), so you probably want to print them both.
     * @return The advanced usage message
     */
    static std::string getAdvancedUsageMessage();

    /** Set the verbosity of spdlog, if valid.
     * @param str        One of "trace", "debug", "info", "warn", "error", or "critical
     * @return true if the verbosity was valid and updated.
     */
    bool static setVerbosity(const std::string& str);

    /** Check if fileName is a .pls file, and if it and a corresponding .wvs file exists. If so, set CmdLine::fileName to that, and return true.
     * @param fileName   path to .pls file
     * @return true iff fileName is a .pls file that exists, and a file with the same name but a .wvs extension exists.
     */
    bool setInputFile(const std::string& fileName);

    /** Attempt to parse the list of products in productList, and add the to CmdLine::products with the specified property.
     * @param productList   The list of products to parse. Allowed format is integers separated by whitespace and/or commas
     * @param property      The peak property to add the products with.
     * @return              true the if the productList was parsed and successfully added to CmdLine::products, false otherwise
     */
    bool parseProducts(std::string productList, PeakProducts::Property property);

    /** Attempts to parse the string integer into an int. Returns false if parsing failed. Also takes an optional set of boundaries. If the parsed int is not within the boundaries(inclusive), outInt is not modified and the function returns false.
     * @param integer       String containing the integer to be parsed.
     * @param outInt        If parsing was successful, write result into outInt
     * @param lowerBound    (Optional) Inclusive lower bound. Defaults to int min
     * @param upperBound    (Optional) Inclusive upper bound. Defaults to int max
     * @return              True if integer was successfully parsed into an int, and the result is in the range [lowerBound, upperBound]. False otherwise.
     */
    static bool tryParseInteger(const std::string& integer, int& outInt, int lowerBound = std::numeric_limits<int>::min(), int upperBound = std::numeric_limits<int>::max());
};

#endif //ADAPTLIDARTOOLS_CMDLINE_HPP
