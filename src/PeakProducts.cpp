#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <iterator>
#include <numeric>
#include <unordered_map>

#include "GDALWriter.hpp"
#include "Peak.hpp"
#include "PeakProducts.hpp"

//See https://en.cppreference.com/w/cpp/algorithm/accumulate for details on std::accumulate

//Add specializations to std::hash for our enum classes. This isn't needed as of C++14. See https://en.cppreference.com/w/cpp/utility/hash for more information.
namespace std{
    template <> struct hash<PeakProducts::Property>{
        std::size_t operator()(PeakProducts::Property& property) const{
            using UnderlyingType = std::underlying_type<PeakProducts::Property>::type;
            return std::hash<UnderlyingType>{}(static_cast<UnderlyingType>(property));
        }
    };

    template <> struct hash<PeakProducts::Statistic>{
        std::size_t operator()(PeakProducts::Statistic& stat) const{
            using UnderlyingType = std::underlying_type<PeakProducts::Statistic>::type;
            return std::hash<UnderlyingType>{}(static_cast<UnderlyingType>(stat));
        }
    };

    template <> struct hash<PeakProducts::Subset>{
        std::size_t operator()(PeakProducts::Subset& subset) const{
            using UnderlyingType = std::underlying_type<PeakProducts::Subset>::type;
            return std::hash<UnderlyingType>{}(static_cast<UnderlyingType>(subset));
        }
    };
}

namespace PeakProducts{

//A mapping of property enums, used to extract a property from the peak.
//C++11 doesn't provide a hashing function for enums so we provide one
const std::unordered_map<Property, std::function<double(const Peak&)>, std::hash<int>> propertyMap{
    {Property::Amplitude,   [](const Peak& peak){return peak.amp;}},
    {Property::Elevation,   [](const Peak& peak){return peak.z_activation;}},
    {Property::Width,       [](const Peak& peak){return peak.fwhm;}},
    {Property::RiseTime,    [](const Peak& peak){return peak.rise_time;}},
    {Property::Backscatter, [](const Peak& peak){return peak.backscatter_coefficient;}}
};

//@@TODO: Should these be accessible to users, and should there be spaces in the name?
const std::unordered_map<Subset, std::string> peakSubsetStringMap{
    {Subset::First,     "First"},
    {Subset::Last,      "Last"},
    {Subset::All,       "All"}
};

const std::unordered_map<Statistic, std::string> productStringMap{
    {Statistic::Maximum,      "Maximum"},
    {Statistic::Minimum,      "Minimum"},
    {Statistic::Average,      "Average"},
    {Statistic::StandardDeviation,"Standard Deviation"},
    {Statistic::Skewness,     "Skewness"},
    {Statistic::Kurtosis,     "Kurtosis"}
};

const std::unordered_map<Property, std::string> propertyStringMap{
    {Property::Amplitude,   "Amplitude"},
    {Property::Elevation,   "Elevation"},
    {Property::Width,       "Width"},
    {Property::RiseTime,    "Rise Time"},
    {Property::Backscatter, "Backscatter"}
};

/**Computes the minimum value of a range of doubles.
 * The result is undefined if std::distance(begin, end) == 0
 * @param begin     Inclusive start of the range
 * @param end       Exclusive end of the range
 * @return          Minimum value
 */
double minimum(std::vector<double>::const_iterator begin, std::vector<double>::const_iterator end){
    assert(std::distance(begin, end) != 0);
    return *std::min_element(begin, end);
}

/**Computes the maximum value of a range of doubles.
 * The result is undefined if std::distance(begin, end) == 0
 * @param begin     Inclusive start of the range
 * @param end       Exclusive end of the range
 * @return          Maximum value
 */
double maximum(std::vector<double>::const_iterator begin, std::vector<double>::const_iterator end){
    assert(std::distance(begin, end) != 0);
    return *std::max_element(begin, end);
}

/**Computes the average of a range of doubles.
 * The result is undefined if std::distance(begin, end) == 0
 * @param begin     Inclusive start of the range
 * @param end       Exclusive end of the range
 * @return          Average
 */
double average(std::vector<double>::const_iterator begin, std::vector<double>::const_iterator end){
    assert(std::distance(begin, end) != 0);
    double sum = std::accumulate(begin, end, 0.0);
    return sum/std::distance(begin, end);
}

/**Computes the standard deviation of a range of doubles.
 * The result is undefined if std::distance(begin, end) == 0
 * @param begin     Inclusive start of the range
 * @param end       Exclusive end of the range
 * @return          Standard Deviation
 */
double standardDeviation(std::vector<double>::const_iterator begin, std::vector<double>::const_iterator end){
    assert(std::distance(begin, end) != 0);
    double mean = average(begin, end);
    double value = std::accumulate(begin, end, 0.0,
            [&](double sum, double val){
                return sum + std::pow(val-mean, 2);
            });
    return std::sqrt(value/std::distance(begin, end));
}

/**Computes the skewness or kurtosis of a range of doubles.
 * The result is undefined if std::distance(begin, end) == 0
 * @param begin     Inclusive start of the range
 * @param end       Exclusive end of the range
 * @param power     3 for skewness, 4 for kurtosis
 * @return          Standard Deviation
 *
 * Formulas come from https://www.itl.nist.gov/div898/handbook/eda/section3/eda35b.htm
 * (This is also where the screenshot in the variable description file comes from)
 */
double skewtosis(std::vector<double>::const_iterator begin, std::vector<double>::const_iterator end, int power){
    assert(std::distance(begin, end) != 0);
    assert(power == 3 || power == 4);
    double mean = average(begin, end);
    double stddev = standardDeviation(begin, end);
    double val = std::accumulate(begin, end, 0.0,
            [&](double sum, double val){
                return sum + std::pow(val-mean, power);
            });
    return (val/std::distance(begin, end)) / std::pow(stddev, power);
}

double produceProduct(const std::list<Peak>& peaks, Product product){
    if(peaks.empty()) return GDALWriter::GDAL_NO_DATA;

    Property property   = std::get<0>(product);
    Statistic statistic = std::get<1>(product);
    Subset subset       = std::get<2>(product);

    std::vector<double> values;
    for(const Peak& peak : peaks){
        switch(subset){
            case Subset::All:
                break;
            case Subset::First:
                if(peak.position_in_wave != 1) continue;
                break;
            case Subset::Last:
                if(!peak.is_final_peak) continue;
                break;
            default:
                assert(false);
        }

        values.emplace_back(propertyMap.at(property)(peak));
    }

    if(values.empty()) return GDALWriter::GDAL_NO_DATA;

    switch(statistic){
        case Statistic::Maximum:
            return maximum(values.begin(), values.end());
        case Statistic::Minimum:
            return minimum(values.begin(), values.end());
        case Statistic::Average:
            return average(values.begin(), values.end());
        case Statistic::StandardDeviation:
            return standardDeviation(values.begin(), values.end());
        case Statistic::Skewness:
            return skewtosis(values.begin(), values.end(), 3);
        case Statistic::Kurtosis:
            return skewtosis(values.begin(), values.end(), 4);
        default:
            assert(false);
    }
}

std::string productString(Product product){
    Property property   = std::get<0>(product);
    Statistic statistic = std::get<1>(product);
    Subset subset       = std::get<2>(product);
    return peakSubsetStringMap.at(subset) + "_" + productStringMap.at(statistic) +  "_" + propertyStringMap.at(property);
}

}   // namespace PeakProducts
