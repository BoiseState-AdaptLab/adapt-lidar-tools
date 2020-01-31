#ifndef ADAPTLIDAR_FITTER_HPP
#define ADAPTLIDAR_FITTER_HPP
#include <iostream>
#include <vector>

namespace Fitter{

    //@@TODO docs
    struct Gaussian{
        Gaussian(){};
        Gaussian(double a, double b, double c) : a(a), b(b), c(c) {};   //Allow for {1,2,3} construction
        double a=0;
        double b=0;
        double c=0;
    };

    //@@TODO docs
    std::vector<Gaussian> fitGaussians(const std::vector<double>& indexData, const std::vector<double>& amplitudeData, const std::vector<Gaussian>& guesses);
}
#endif  //ADAPTLIDAR_FITTER_HPP
