/*
 * File name: WaveformGaussianFitting.hpp
 * Created on: 12-July-2017
 * Author: olschanowsky
 */

#ifndef WAVEFORMGAUSSIANFIT_HPP_
#define WAVEFORMGAUSSIANFIT_HPP_

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlinear.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

class WaveformGaussianFitting{

private:
  struct data fit_data;  
public:  
  void fitPairData(std::Vector waveOut,std::Vector timeOut,
                   std::Vector waveIn,std:Vector timeIn);
  
};

#endif /* WAVEFORMGAUSSIANFIT_HPP_ */
