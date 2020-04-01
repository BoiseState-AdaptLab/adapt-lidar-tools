#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <list>
#include <numeric>
#include <unordered_map>

#include "Peak.hpp"
#include "PeakProducts.hpp"

//See https://en.cppreference.com/w/cpp/algorithm/accumulate for details on std::accumulate
namespace PeakProducts{

//A mapping of property enums, used to extract a property from the peak.
const std::unordered_map<Property, std::function<double(const Peak&)>> propertyMap{
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

double minimum(std::list<Peak>::const_iterator begin, std::list<Peak>::const_iterator end, Property property){
    return propertyMap.at(property)(*std::min_element(begin, end,
            [&](const Peak& lhs, const Peak& rhs){
                return propertyMap.at(property)(lhs) < propertyMap.at(property)(rhs);
            }));
}

double maximum(std::list<Peak>::const_iterator begin, std::list<Peak>::const_iterator end, Property property){
    return propertyMap.at(property)(*std::max_element(begin, end,
            [&](const Peak& lhs, const Peak& rhs){
                return propertyMap.at(property)(lhs) < propertyMap.at(property)(rhs);
            }));
}

double average(std::list<Peak>::const_iterator begin, std::list<Peak>::const_iterator end, Property property){
    double sum = std::accumulate(begin, end, 0.0,
            [&](double sum, const Peak& peak){
                return sum + propertyMap.at(property)(peak);
            });
    return sum/std::distance(begin, end);
}

double standardDeviation(std::list<Peak>::const_iterator begin, std::list<Peak>::const_iterator end, Property property){
    double mean = average(begin, end, property);
    double value = std::accumulate(begin, end, 0.0,
            [&](double sum, const Peak& peak){
                return sum + std::pow(propertyMap.at(property)(peak)-mean, 2);
            });
    return std::sqrt(value/std::distance(begin, end));
}

//Taken from https://www.itl.nist.gov/div898/handbook/eda/section3/eda35b.htm
//power = 3 for skewness, 4 for kurtosis
double skewtosis(std::list<Peak>::const_iterator begin, std::list<Peak>::const_iterator end, Property property, int power){
    double mean = average(begin, end, property);
    double stddev = standardDeviation(begin, end, property);
    double val = std::accumulate(begin, end, 0.0,
            [&](double sum, const Peak& peak){
                return sum + std::pow(propertyMap.at(property)(peak) - mean, power);
            });
    return (val/std::distance(begin, end)) * std::pow(stddev, power);
}

double produceProduct(const std::list<Peak>& peaks, Product product){
    if(peaks.empty()) return 0.0; //@@TODO no data?

    //@@TODO: Check which functions need more than one data point
    
    Property property   = std::get<0>(product);
    Statistic statistic = std::get<1>(product);
    Subset subset       = std::get<2>(product);

    std::list<Peak>::const_iterator begin = peaks.begin();
    std::list<Peak>::const_iterator end = peaks.end();

    switch(subset){
        case Subset::All:
            break;
        case Subset::First:
            end = begin;
            ++end;
            break;
        case Subset::Last:
            begin = end;
            --begin;
            break;
        default:
            return 0;
            //@@TODO assert false
    }

    switch(statistic){
        case Statistic::Maximum:
            return maximum(begin, end, property);
        case Statistic::Minimum:
            return minimum(begin, end, property);
        case Statistic::Average:
            return average(begin, end, property);
        case Statistic::StandardDeviation:
            return standardDeviation(begin, end, property);
        case Statistic::Skewness:
            return skewtosis(begin, end, property, 3);
        case Statistic::Kurtosis:
            return skewtosis(begin, end, property, 4);
        default:
            //@@TODO assert(false);
            return 0;
    }
}

std::string productString(Product product){
    Property property   = std::get<0>(product);
    Statistic statistic = std::get<1>(product);
    Subset subset       = std::get<2>(product);
    return peakSubsetStringMap.at(subset) + "_" + productStringMap.at(statistic) +  "_" + propertyStringMap.at(property);
}

}   // namespace PeakProducts
