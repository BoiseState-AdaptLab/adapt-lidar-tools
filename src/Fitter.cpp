#include <gsl/gsl_blas.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_multifit_nlinear.h>
#include <gsl/gsl_vector.h>

#include <algorithm>
#include <cassert>
#include <memory>
#include <numeric>
#include <vector>

#include "spdlog/spdlog.h"
#include "Fitter.hpp"

namespace Fitter{
//@@TODO docs

struct Pulse{
    Pulse(const std::vector<double>& idxData, const std::vector<double>& ampData) : indexData(idxData), amplitudeData(ampData){};
    const std::vector<double>& indexData;
    const std::vector<double>& amplitudeData;
};

//https://en.wikipedia.org/wiki/Gaussian_function
double gaussianFunc(double a, double b, double c, double t){
    double z = (t-b)/c;
    return a * std::exp(-0.5 * z * z);
}

std::string gaussianToString(const gsl_vector& x){
    std::string result;
    for(std::size_t i = 0; i < x.size/3; ++i){
        result += fmt::format("{{{}, {}, {}}} ",   //@@TODO specify # of decimals
                gsl_vector_get(&x, i*3),
                gsl_vector_get(&x, i*3+1),
                gsl_vector_get(&x, i*3+2));
    }
    return result;
}
    
//@@TODO name
int func_f(const gsl_vector* x, void* params, gsl_vector* f){
    assert(x);
    assert(params);
    assert(f);

    const Pulse& data = *reinterpret_cast<const Pulse*>(params);

    for(std::size_t i = 0; i < data.indexData.size(); ++i){
        double t = data.indexData[i];
        double y = data.amplitudeData[i];

        for(std::size_t j = 0; j < x->size/3; ++j){
            double a = gsl_vector_get(x, j*3);
            double b = gsl_vector_get(x, j*3+1);
            double c = gsl_vector_get(x, j*3+2);

            y-=gaussianFunc(a,b,c,t);
        }

        gsl_vector_set(f, i, y);
    }

    return GSL_SUCCESS;
}


//@@TODO name
int func_df(const gsl_vector* x, void* params, gsl_matrix* J){
    assert(x);
    assert(params);
    assert(J);

    const Pulse& data = *reinterpret_cast<const Pulse*>(params);

    for(std::size_t i = 0; i < data.indexData.size(); ++i){
        double t = data.indexData[i];

        for(std::size_t j = 0; j < x->size/3; ++j){
            double a = gsl_vector_get(x, j*3);
            double b = gsl_vector_get(x, j*3+1);
            double c = gsl_vector_get(x, j*3+2);

            double zi = (t-b)/c;
            double ei = std::exp(-0.5 * zi * zi);

            gsl_matrix_set(J, i, 3*j  , -ei);
            gsl_matrix_set(J, i, 3*j+1, -(a/c) * ei * zi);
            gsl_matrix_set(J, i, 3*j+2, -(a/c) * ei * zi * zi);
        }
    }

    return GSL_SUCCESS;
}

void iterCallback(std::size_t iterNum, void*, const gsl_multifit_nlinear_workspace* w){
    assert(w);

    const gsl_vector* x = gsl_multifit_nlinear_position(w);

    if(spdlog::default_logger()->level() == spdlog::level::trace){  //Don't do the next part unless trace
        spdlog::trace("Iteration {}\tData: {}", iterNum, gaussianToString(*x));
    }
}

bool solveSystem(gsl_multifit_nlinear_workspace* workspace, gsl_vector* results){
    assert(results);
    assert(workspace);

    const gsl_vector* params = gsl_multifit_nlinear_position(workspace);
    assert(params && params->size == results->size);

    const size_t maxIter = 100;
    const double xTol = 1.0e-2;
    const double gTol = 1.0e-8;//std::pow(GSL_DBL_EPSILON, 1/3); //Recommended in docs @@TODO make constant
    const double fTol = 1.0e-8; //@@TODO no idea

    spdlog::debug("Starting fitting with guesses {}", gaussianToString(*params));


    int info = GSL_SUCCESS;
    int result = gsl_multifit_nlinear_driver(maxIter, xTol, gTol, fTol, iterCallback, nullptr, &info, workspace);

    gsl_vector_memcpy(results, params);  //Copy results into output vector

    if(result != GSL_SUCCESS){
        spdlog::error("Fitting failed with error \"{}\"", gsl_strerror(result));
        spdlog::error("Last guesses: {}", gaussianToString(*params));
        return false;
    }

    spdlog::debug("Fitting converged {}", info); //@@TODO log convergence reason
    spdlog::debug("Final Guesses: {}", gaussianToString(*params));
    return true;
}

gsl_multifit_nlinear_workspace* setupWorkspace(const Pulse& data, const gsl_vector* guess, gsl_multifit_nlinear_fdf& system, gsl_multifit_nlinear_parameters& params){
    assert(guess);

    params = gsl_multifit_nlinear_default_parameters();

    system.f = func_f;
    system.df = func_df;
    system.fvv=nullptr;
    params.trs = gsl_multifit_nlinear_trs_lmaccel;

    system.n = data.amplitudeData.size();   //number of data points
    system.p = guess->size; //Number of params
    system.params = reinterpret_cast<void*>(const_cast<Pulse*>(&data)); //Cast to void* (remove const then change type)

    gsl_multifit_nlinear_workspace* workspace = gsl_multifit_nlinear_alloc(gsl_multifit_nlinear_trust, &params, data.amplitudeData.size(), guess->size);

    gsl_multifit_nlinear_init(guess, &system, workspace);
    return workspace;
}


std::vector<Gaussian> fitGaussians(const std::vector<double>& indexData, const std::vector<double>& amplitudeData, const std::vector<Gaussian>& guesses){
    //@@TODO: prefix logs with function name?
    if(indexData.size() != amplitudeData.size()){
        spdlog::critical("Index data and amplitude data have mismatched sizes! ({}) and ({})", indexData.size(), amplitudeData.size());
        return {};
    }

    if(indexData.empty()){
        spdlog::error("No waveform data");
        return {};
    }

    if(guesses.empty()){
        spdlog::info("No peaks to fit");    //@@TODO level, this may be common
        return {};
    }

    //Write waveform if trace logging
    if(spdlog::default_logger()->level() == spdlog::level::trace){
        std::string tmp;
        for(auto val : indexData){
            tmp+=std::to_string(val)+" ";
        }
        spdlog::trace("Index Data:\n{}", tmp);

        tmp.clear();
        for(auto val : amplitudeData){
            tmp+=std::to_string(val)+" ";
        }

        spdlog::trace("Amplitude Data:\n{}", tmp);
    }


    //@@TODO: noise level currently doesnt do anything(?) regarding the actual fitting. Consider: For values less than it, dampen or floor them?
    //If so, that stuff doesn't belong here, it belongs before we do stuff with the data.
    
    //Create workspace and params
    std::unique_ptr<gsl_vector, decltype(&gsl_vector_free)> params (gsl_vector_alloc(3*guesses.size()), &gsl_vector_free);  //Auto free
    //@@TODO prepare defense of speed
    
    //Copy to gsl vector
    for(std::size_t i = 0; i < guesses.size(); ++i){
        gsl_vector_set(params.get(), i*3,   guesses[i].a);
        gsl_vector_set(params.get(), i*3+1, guesses[i].b);
        gsl_vector_set(params.get(), i*3+2, guesses[i].c);
    }

    const Pulse data{indexData, amplitudeData};  //For passing through void*
    gsl_multifit_nlinear_fdf system;
    gsl_multifit_nlinear_parameters fdf_params;;
    std::unique_ptr<gsl_multifit_nlinear_workspace, decltype(&gsl_multifit_nlinear_free)> workspace (setupWorkspace(data, params.get(), system, fdf_params), &gsl_multifit_nlinear_free);
    
    bool result = solveSystem(workspace.get(), params.get());
    if(!result){
        //@@TODO
    }

    std::vector<Gaussian> results;  //@@TODO how many peaks do we expect at most? if large number, .reserve

    for(std::size_t i = 0; i < guesses.size(); ++i){
        double a = gsl_vector_get(params.get(), i*3);
        double b = gsl_vector_get(params.get(), i*3+1);
        double c = gsl_vector_get(params.get(), i*3+2);
        results.push_back({a,b,c});
    }

    return results; //Someone else can check and see if the peaks make sense (i.e. check negative amplitude)
}
};

//@@TODO: Smoothing will break if the data is not all evenly spaced
int main(){
    spdlog::set_level(spdlog::level::trace);
    std::vector<double> ampData = 
    //{1,1,0,0,0,0,0,0,0,0,1,4,13,35,73,120,161,189,199,199,191,178,159,138,119,98,78,62,49,45,42,42,39,34,30,27,28,32,33,30,23,16,10,7,7,5,5,4,4,3,3,3,3,5,4,3,2,1,0,0}; //NayaniClipped1
    {0,0,0,0,0,0,0,0,0,0,0,0,1,6,17,37,63,85,96,95,87,80,74,77,91,112,128,135,139,150,163,166,151,117,75,44,24,17,13,11,11,11,11,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,2}; //NayaniClipped7
    
    std::vector<double> idxData(ampData.size(), 0.0);
    std::iota(idxData.begin(), idxData.end(), 0.0); //Generate range [0.....ampData.size()-1]

    std::vector<Fitter::Gaussian> guesses ={
        {97,    18,     3.88498},
        {167,   31,     4.40505},
        {12,    40.5,   4.14005}
    };

    auto result = Fitter::fitGaussians(idxData, ampData, guesses);
}
