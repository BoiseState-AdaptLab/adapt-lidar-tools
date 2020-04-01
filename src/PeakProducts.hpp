#ifndef ADAPTLIDARTOOLS_PEAKPRODUCTS_HPP
#define ADAPTLIDARTOOLS_PEAKPRODUCTS_HPP

#include <list>
#include <string>
#include <tuple>

#include "Peak.hpp"

namespace PeakProducts{

    enum class Subset {First, Last, All};

    enum class Statistic {Maximum, Minimum, Average, StandardDeviation, Skewness, Kurtosis};

    enum class Property {Amplitude, Elevation, Width, RiseTime, Backscatter};

    using Product = std::tuple<Property, Statistic, Subset>;

    double produceProduct(const std::list<Peak>& peaks, Product product);

    std::string productString(Product product);

}   //namespace PeakProducts
#endif // ADAPTLIDARTOOLS_PEAKPRODUCTS_HPP
