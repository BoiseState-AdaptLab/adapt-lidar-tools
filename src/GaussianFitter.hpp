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
#include <sstream>

// Compile-time defaults for fitter params
#define MAX_ITER 200

#define TOL_SCALES true
#define X_TOL .01
#define G_TOL .01
#define F_TOL .01

#define GUESS_LT0_DEFAULT 4
#define GUESS_UPPER_LIM 20
#define GUESS_UPPER_LIM_DEFAULT 10

#define MAX_AMP_MULTIPLIER 2.
#define AMP_LOWER_BOUND 10

class GaussianFitter{

    public:
        int find_peaks(std::vector<Peak*>* results,std::vector<int> ampData,
                std::vector<int> idxData, const size_t max_iter);
        int noise_level;
        int guess_peaks(std::vector<Peak*>* results, 
                std::vector<int> ampData, 
                std::vector<int> idxData);
        void smoothing_expt(std::vector<int> *waveArray);
        GaussianFitter();
        std::string get_equation(int idx);
        int get_fail();
        int get_pass();
        int get_total();

        void setDiagnostics(bool newval);

        std::vector<std::string> equations; //Fitted equations

        int max; //The max peak amplitude of each set of returning waves
        int fail;
        int pass;
        int total;

        // *** Fitter parameters (that were magic numbers once) ***

        bool tolerance_scales;
        double x_tolerance;
        double g_tolerance;
        double f_tolerance;

        int guess_lessthan_0_default; // If guess less than 0, it is set to this
        int guess_upper_lim;          // If guess greater than this value...
        int guess_upper_lim_default;  // It is set to this value

        float max_amp_multiplier; // Val is multiplied by max data point in wave
        float amp_lower_bound; // Val is unmodified (no multiplication)


    private:
        bool log_diagnostics;

        int solve_system (gsl_vector *x,
                gsl_multifit_nlinear_fdf *fdf,
                gsl_multifit_nlinear_parameters *params, int max,
                const size_t max_iter);

        std::vector<int> calculateFirstDifferences(std::vector<int>ampData);
        void incr_fail();
        void incr_pass();
        void incr_total();
};

#endif /* GAUSIANFITTING_HPP_ */
