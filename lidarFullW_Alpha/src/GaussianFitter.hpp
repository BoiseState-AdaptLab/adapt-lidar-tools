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
    int find_peaks(std::vector<Peak>* results,std::vector<int> ampData,
                  std::vector<int> idxData);
    int noise_level;
    std::vector<int> guess_peaks(std::vector<int> ampData);
    void smoothing_expt(std::vector<int> *waveArray);
    GaussianFitter();
    int get_fail();

    //The max peak amplitude of each set of returning waves
    int max; 
    int fail;

  private:
    int getNumPeaks();
    std::vector<int> calculateFirstDifferences(std::vector<int>ampData);
    void calculateSecondDifference();
    int medianOfFive(int a, int b, int c, int d, int e);
    void calculateSmoothSecondDifference();
    void incr_fail();


};

#endif /* GAUSIANFITTING_HPP_ */
