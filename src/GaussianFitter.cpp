#include <algorithm>
#include <cassert>
#include <memory>
#include <utility>
#include <vector>

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_multifit_nlinear.h>
#include <gsl/gsl_vector.h>

#include "spdlog/spdlog.h"

#include "GaussianFitter.hpp"

namespace GaussianFitter{

//Small wrapper used to pass variables through the void* params pointer that GSL gives us.
struct Pulse{
    Pulse() = delete;   //It doesn't make sense to make an empty one of these, since it is a group of aliases
    Pulse(const std::vector<int>& idxData, const std::vector<int>& ampData, int minPeak) : indexData(idxData), amplitudeData(ampData), minPeak(minPeak){};
    const std::vector<int>& indexData;
    const std::vector<int>& amplitudeData;
    const int minPeak;
};

//https://en.wikipedia.org/wiki/Gaussian_function
double gaussianFunc(double a, double b, double c, double t){
    double z = (t-b)/c;
    return a * std::exp(-0.5 * z * z);
}

/**
 * Nicely formats the parameter vector for debug messages.
 * @param x     The vector to format, should be some multiple of {a,b,c}
 * @return      "{a, b, c} " where a b and c are padded to 6 characters with 2 decimals.
 */
std::string gaussianToString(const gsl_vector& x){
    std::string result;
    for(std::size_t i = 0; i < x.size/3; ++i){
        result += fmt::format("{{{:>6.2f}, {:>6.2f}, {:>6.2f}}} ",   //Pad to 6 chars wide, limit to 2 decimals
            gsl_vector_get(&x, i*3),
            gsl_vector_get(&x, i*3+1),
            gsl_vector_get(&x, i*3+2));
    }

    return result;
}

/**
 * Given the parameters x (which is a list of {a,b,c} tuples), put the error into the vector f.
 * @param x         The current parameters. Is some amount of {a,b,c} tuples.
 * @param params    Used to recover the index and amplitude data.
 * @param f         Output vector to store error in.
 * @return          Always GSL_SUCCESS
 */
int gaussianSumResidual(const gsl_vector* x, void* params, gsl_vector* f){
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

            //This is how we constrain the fitting to some boundaries. If the Gaussian exceeds these boundaries, we add a large error (1000)
            if(a < data.minPeak || b < 0 || b > data.indexData.back() || c < 1){    //@@TODO did we want minPeak or noise_level?
                y+=1000;    //Penalty to act as a constraint.
            }
        }

        gsl_vector_set(f, i, y);
    }

    return GSL_SUCCESS;
}

/**
 * Computes the Jacobian of the Gaussian sum equation.
 * @param x         Gaussian parameters
 * @param params    Used to recover the idx and amp data
 * @param J         Output matrix for the Jacobian
 * @return          Always GSL_SUCCESS
 */
int gaussianSumResidualDerivative(const gsl_vector* x, void* params, gsl_matrix* J){
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

/**
 * Called once per iteration of the solver. Used only for trace logging.
 * @param iterNum   The iteration number (i.e. 1 = first iter)
 * @param void      Unused/notneeded
 * @param w         The current solver workspace
 */
void iterCallback(std::size_t iterNum, void* /*unused*/, const gsl_multifit_nlinear_workspace* w){
    assert(w);

    if(spdlog::default_logger()->level() == spdlog::level::trace){  //Don't do the next part unless trace
        const gsl_vector* x = gsl_multifit_nlinear_position(w);
        spdlog::trace("Iteration {}\tData: {}", iterNum, gaussianToString(*x));
    }
}

/**
 * Using an existing workspace, iterates until the system converges or errors/times out.
 * Stores the final parameters (regardless of success) in results.
 * @param workspace     A workspace ready to be iterated with
 * @param results       An allocated vector to store the results in.
 * @return              True if system successfully converges, false otherwise.
 */
bool solveSystem(gsl_multifit_nlinear_workspace* workspace, gsl_vector* results){
    assert(results);
    assert(workspace);

    const gsl_vector* params = gsl_multifit_nlinear_position(workspace);
    assert(params && params->size == results->size);

    const size_t maxIter = 150;
    const double xTol = 1.0e-2; //GSL recommends the power be the number of decimal places you want the accuracy to be
    const double gTol = 1.0e-8; //std::pow(GSL_DBL_EPSILON, 1./3); //Recommended to be this in docs @@TODO
    const double fTol = 1.0e-8; //@@TODO no idea what a good metric for this is

    spdlog::debug("Starting fitting with guesses {}", gaussianToString(*params));

    int info = GSL_SUCCESS;
    int result = gsl_multifit_nlinear_driver(maxIter, xTol, gTol, fTol, iterCallback, nullptr, &info, workspace);

    gsl_vector_memcpy(results, params);  //Copy results into output vector

    if(result != GSL_SUCCESS){
        spdlog::error("Fitting failed with error \"{}\"", gsl_strerror(result));
        spdlog::error("Last guesses: {}", gaussianToString(*params));
        return false;
    }

    if(info == 0){
        spdlog::debug("Fitting failed to converge");    //This shouldn't happen if the driver return GSL_SUCCESS
    }else if(info == 1){
        spdlog::trace("Fitting converged due to small step size");
    }else if (info == 2){
        spdlog::trace("Fitting converged due to a small gradient");
    }

    spdlog::debug("Final Guesses: {}", gaussianToString(*params));
    return true;
}

/**
 * Sets up the workspace for the solver. Note that the workspace returned will later need to be free.
 * @param data      Problem data
 * @param guess     Initial starting guess
 * @param system    Empty system, this needs to have the same lifetime as the returned workspace
 * @param params    Empty params, this needs to have the same lifetime as the returned workspace
 * @return          A ready to use workspace that needs to eventually be freed
 */
gsl_multifit_nlinear_workspace* setupWorkspace(const Pulse& data, const gsl_vector* guess, gsl_multifit_nlinear_fdf& system, gsl_multifit_nlinear_parameters& params){
    assert(guess);

    params = gsl_multifit_nlinear_default_parameters();

    system.f    = gaussianSumResidual;
    system.df   = gaussianSumResidualDerivative;
    system.fvv  = nullptr;
    params.trs  = gsl_multifit_nlinear_trs_lmaccel;

    system.n = data.amplitudeData.size();   //number of data points
    system.p = guess->size;                 //Number of params
    system.params = reinterpret_cast<void*>(const_cast<Pulse*>(&data)); //Cast to void* (remove const then change type)

    gsl_multifit_nlinear_workspace* workspace =
        gsl_multifit_nlinear_alloc(gsl_multifit_nlinear_trust, &params, data.amplitudeData.size(), guess->size);

    gsl_multifit_nlinear_init(guess, &system, workspace);
    return workspace;
}

//See GaussianFitter.hpp for docs
bool fitGaussians(const std::vector<int>& indexData, const std::vector<int>& amplitudeData, int minPeakAmp, std::vector<Gaussian>& guesses){
    if(indexData.size() != amplitudeData.size()){
        spdlog::critical("Index data and amplitude data have mismatched sizes! ({}) and ({})", indexData.size(), amplitudeData.size());
        return false;
    }

    if(indexData.empty()){
        spdlog::trace("No waveform data");
        return false;
    }

    if(guesses.empty()){
        spdlog::trace("No peaks to fit");
        return false;
    }

    //Lambda for printing out a vector https://en.cppreference.com/w/cpp/language/lambda
    auto printVec = [](const std::vector<int>& vec){
        std::string tmp;
        for(const int& i : vec){
            tmp+=std::to_string(i)+" ";
        }
        return tmp;
    };


    //Write waveform if trace logging
    if(spdlog::default_logger()->level() == spdlog::level::trace){
        spdlog::trace("Index Data:\n{}", printVec(indexData));
        spdlog::trace("Amplitude Data:\n{}", printVec(amplitudeData));
    }

    //Create workspace and params
    std::unique_ptr<gsl_vector, decltype(&gsl_vector_free)> params
        (gsl_vector_alloc(3*guesses.size()), &gsl_vector_free);  //Owning pointer auto frees
    //Note: Compilers seem to be able to completely optimize this away: https://godbolt.org/z/SooW3d

    //Copy to gsl vector
    for(std::size_t i = 0; i < guesses.size(); ++i){
        gsl_vector_set(params.get(), i*3,   guesses[i].a);
        gsl_vector_set(params.get(), i*3+1, guesses[i].b);
        gsl_vector_set(params.get(), i*3+2, guesses[i].c);
    }

    const Pulse data{indexData, amplitudeData, minPeakAmp};  //For passing through void*
    gsl_multifit_nlinear_fdf system;
    gsl_multifit_nlinear_parameters fdf_params;;
    std::unique_ptr<gsl_multifit_nlinear_workspace, decltype(&gsl_multifit_nlinear_free)> workspace
        (setupWorkspace(data, params.get(), system, fdf_params), &gsl_multifit_nlinear_free);

    bool result = solveSystem(workspace.get(), params.get());

    if(!result){    //Fitting failed, dump waveform
        spdlog::error("Fitting failed. Waveform:");
        spdlog::error("Index Data:\t\t{}", printVec(indexData));
        spdlog::error("Amplitude Data:\t{}", printVec(amplitudeData));
    }

    //Copy back to return the results
    for(std::size_t i = 0; i < guesses.size(); ++i){
        double a = gsl_vector_get(params.get(), i*3);
        double b = gsl_vector_get(params.get(), i*3+1);
        double c = gsl_vector_get(params.get(), i*3+2);
        guesses.at(i) = {a,b,c};
    }

    return result; //Someone else can check and see if the peaks make sense (i.e. check negative amplitude)
}

//See GaussianFitter.hpp for docs
void estimateGaussians(const std::vector<int>& indexData, const std::vector<int>& amplitudeData, int minPeakAmp, std::vector<Gaussian>& guesses){
    guesses.clear();

    if(indexData.size() != amplitudeData.size()){
        spdlog::critical("Index data and amplitude data have mismatched sizes! ({}) and ({})", indexData.size(), amplitudeData.size());
        return;
    }

    if(amplitudeData.empty()){
        spdlog::trace("No amplitude data");
        return;
    }

    int min2ndDiffVal = 0;
    int min2ndDiffIdx = -1;
    bool trackingPeak = false;

    for(std::size_t i = 1; i < amplitudeData.size()-1; ++i){
        int secondDeriv = amplitudeData[i+1] - 2*amplitudeData[i] + amplitudeData[i-1];

        if(indexData[i] - indexData[i-1] != 1 || indexData[i+1] - indexData[i] != 1){   //Gap in the data (segmented wave)
            secondDeriv = 0;
        }

        if(secondDeriv >= 0 && trackingPeak){   //Finished tracking a peak, add it to guesses
            int a = amplitudeData[min2ndDiffIdx];
            int b = indexData[min2ndDiffIdx];

            if(a > minPeakAmp){
                spdlog::trace("Found peak with a of {} at {}", a, b);
                guesses.emplace_back(a, b, 1);
            }

            min2ndDiffVal = 0;
            trackingPeak = false;
        }else if(secondDeriv < 0){  //Currently tracking a peak
            trackingPeak = true;
            if(secondDeriv < min2ndDiffVal || (secondDeriv == min2ndDiffVal && amplitudeData[i] > amplitudeData[min2ndDiffIdx])){     //New minimum, or same min but larger amplitude
                min2ndDiffVal = secondDeriv;
                min2ndDiffIdx = i;
            }
        }
    }

    if(trackingPeak){   //We were tracking a peak when we ran out of data
        spdlog::critical("@@@@ Peak ended while rising");   //@@TODO Remove this eventually
    }

    std::vector<std::size_t> deleteList;
    for(std::size_t i = 1; i < guesses.size(); ++i){
        if(guesses[i].b - guesses[i-1].b < 4){ //@@TODO find a good threshold
            deleteList.push_back(i);
        }
    }

    for(auto iter = deleteList.rbegin(); iter != deleteList.rend(); ++iter){
        spdlog::trace("Deleting peak at {}", *iter);
        guesses.erase(guesses.begin()+*iter);
    }
}

}  // namespace GaussianFitter
