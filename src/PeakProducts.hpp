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

//Add specializations to std::hash for our enum classes. This isn't needed as of C++14. See https://en.cppreference.com/w/cpp/utility/hash for more information.
namespace std{
    template <> struct hash<PeakProducts::Property>{
        std::size_t operator()(PeakProducts::Property property) const{
            using UnderlyingType = std::underlying_type<PeakProducts::Property>::type;
            return std::hash<UnderlyingType>{}(static_cast<UnderlyingType>(property));
        }
    };

    template <> struct hash<PeakProducts::Statistic>{
        std::size_t operator()(PeakProducts::Statistic stat) const{
            using UnderlyingType = std::underlying_type<PeakProducts::Statistic>::type;
            return std::hash<UnderlyingType>{}(static_cast<UnderlyingType>(stat));
        }
    };

    template <> struct hash<PeakProducts::Subset>{
        std::size_t operator()(PeakProducts::Subset subset) const{
            using UnderlyingType = std::underlying_type<PeakProducts::Subset>::type;
            return std::hash<UnderlyingType>{}(static_cast<UnderlyingType>(subset));
        }
    };
}
#endif // ADAPTLIDARTOOLS_PEAKPRODUCTS_HPP
