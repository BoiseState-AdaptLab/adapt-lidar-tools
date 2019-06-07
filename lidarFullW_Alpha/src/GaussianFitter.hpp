//File name: GaussianFitting.hpp
//Created on: 13-October-2017
//Author: ravi

#ifndef GAUSIANFITTING_HPP_
#define GAUSIANFITTING_HPP_

#include "Peak.hpp"
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlinear.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_errno.h>
#include <vector>

class GaussianFitter{

    public:
        int find_peaks(std::vector<Peak*>* results,std::vector<int> ampData,
                std::vector<int> idxData);
        int noise_level;
        int guess_peaks(std::vector<Peak*>* results, 
                std::vector<int> ampData, 
                std::vector<int> idxData);
        void smoothing_expt(std::vector<int> *waveArray);
        GaussianFitter();
        int get_fail();
        int get_pass();
        int get_total();

        int max; //The max peak amplitude of each set of returning waves
        int fail;
        int pass;
        int total;

    private:
        std::vector<int> calculateFirstDifferences(std::vector<int>ampData);
        void incr_fail();
        void incr_pass();
        void incr_total();
};

#endif /* GAUSIANFITTING_HPP_ */
