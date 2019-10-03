//File name: GaussianFittingGpu.cuh
//Created on: 2-October-2019
//Author: brandon 

#ifndef GAUSIANFITTINGGPU_CUH_
#define GAUSIANFITTINGGPU_CUH_

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

// Compile-time default for number of threads for x and y for GPU
#define DEF_NUM_THREADS 16

void func_df_gpu(const gsl_vector* x, double* data_t, size_t data_t_size, gsl_matrix* J);

#endif /* GAUSIANFITTINGGPU_CUH_ */
