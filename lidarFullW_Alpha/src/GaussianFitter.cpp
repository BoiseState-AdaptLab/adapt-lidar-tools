//File name: GaussianFitter.cpp
//Created on: 13-October-2017
//Author: ravi

#include "GaussianFitter.hpp"
#include <iostream>
#include <math.h>
#include <algorithm>

struct data
{
  double *t;
  double *y;
  size_t n;
};


//model function: a * exp( -1/2 * [ (t - b) / c ]^2 )
double gaussianSum(const gsl_vector * x,const double t)
{

  int i = 0;
  double value = 0.;
  for(i=0;i<(x->size/3);i++){
    double a = gsl_vector_get(x,3*i+0);
    double b = gsl_vector_get(x,3*i+1);
    double c = gsl_vector_get(x,3*i+2);
    const double z = (t - b) / c;
    value += (a * exp(-0.5 * z * z));
  }
  return value;
}


//
int func_f (const gsl_vector * x, void *params, gsl_vector * f)
{
  struct data *d = (struct data *) params;
  size_t i;

  for (i = 0; i < d->n; ++i)
    {
      double ti = d->t[i];
      double yi = d->y[i];
      double y = gaussianSum(x, ti);

      //gsl_vector_set(f, i, y);
      gsl_vector_set(f, i, yi - y);
    }

  return GSL_SUCCESS;
}


//
int func_df (const gsl_vector * x, void *params, gsl_matrix * J){
  struct data *d = (struct data *) params;

  int npeaks = x->size/3;
  int j;
  size_t i;
  // for each value of time
  for (i = 0; i < d->n; ++i){
    double a_sum = 0;
    double b_sum = 0;
    double c_sum = 0;
    double ti = d->t[i];
    // calculate the sum of the derivatives
    for(j=0;j<npeaks;j++){
      double a = gsl_vector_get(x, j*3+0);
      double b = gsl_vector_get(x, j*3+1);
      double c = gsl_vector_get(x, j*3+2);

      double zi = (ti - b) / c;
      double ei = exp(-0.5 * zi * zi);

      a_sum = (-1)*ei;
      b_sum = (-1)*a*(ti-b)*ei*(1/(c*c));
      c_sum = (-1)*a*(ti-b)*(ti-b) * ei * (1/(c*c*c));

      // first derivative wrt a
      // ei
      //gsl_matrix_set(J, i,3*j+ 0, -ei);
      gsl_matrix_set(J, i, 3*j+0, a_sum);

      // first derivative wrt b
      // a(t-b)* e ^ ( -.5*((t-b)/c)^2)* (1/c^2)
      // a*(ti-b)*ei*(1/(c*c))
      // gsl_matrix_set(J, i,3*j+ 1, -(a / c) * ei * zi);
      gsl_matrix_set(J, i, 3*j+1, b_sum);

      // first derivative wrt c
      // a*(ti-b)*(ti-b) * ei * (1/(c*c*c))
      gsl_matrix_set(J, i, 3*j+2, c_sum);
    }
  }

  return GSL_SUCCESS;
}


//
int func_fvv (const gsl_vector * x, const gsl_vector * v,
          void *params, gsl_vector * fvv)
{
  struct data *d = (struct data *) params;
  size_t i;

  int npeaks = x->size/3;
  int j;
  for (i = 0; i < d->n; ++i){
    double sum = 0;
    double ti = d->t[i];

    for(j=0;j<npeaks;j++){
      double a = gsl_vector_get(x,3*j+ 0);
      double b = gsl_vector_get(x,3*j+ 1);
      double c = gsl_vector_get(x,3*j+ 2);
      double va = gsl_vector_get(v,3*j+ 0);
      double vb = gsl_vector_get(v,3*j+ 1);
      double vc = gsl_vector_get(v,3*j+ 2);

      double zi = (ti - b) / c;
      double ei = exp(-0.5 * zi * zi);
      double Dab = -zi * ei / c;
      double Dac = -zi * zi * ei / c;
      double Dbb = a * ei / (c * c) * (1.0 - zi*zi);
      double Dbc = a * zi * ei / (c * c) * (2.0 - zi*zi);
      double Dcc = a * zi * zi * ei / (c * c) * (3.0 - zi*zi);

      sum += 2.0 * va * vb * Dab +
            2.0 * va * vc * Dac +
                  vb * vb * Dbb +
            2.0 * vb * vc * Dbc +
                  vc * vc * Dcc;
      }
      gsl_vector_set(fvv, i, sum);
    }

  return GSL_SUCCESS;
}

void callback(const size_t iter, void *params,
         const gsl_multifit_nlinear_workspace *w){
  gsl_vector *f = gsl_multifit_nlinear_residual(w);
  gsl_vector *x = gsl_multifit_nlinear_position(w);
  double avratio = gsl_multifit_nlinear_avratio(w);
  double rcond;

  (void) params; /* not used */

  /* compute reciprocal condition number of J(x) */
  gsl_multifit_nlinear_rcond(&rcond, w);
/*
  size_t npeaks = x->size/3;
  fprintf(stderr, "iter %2zu: ",iter);
  size_t j;
  for(j=0; j<npeaks; j++){
    fprintf(stderr," a = %.4f, b = %.4f, c = %.4f",
          gsl_vector_get(x,3*j+ 0),
          gsl_vector_get(x,3*j+ 1),
          gsl_vector_get(x,3*j+ 2));
  }
  fprintf(stderr,", |a|/|v| = %.4f cond(J) = %8.4f, |f(x)| = %.4f\n",
          avratio,
          1.0 / rcond,
          gsl_blas_dnrm2(f));*/
}


//

void handler (const char * reason,
              const char * file,
              int line,
              int gsl_errno){

  std::cerr << "FATAL Error:" << file << ": " << reason << std::endl;
  gsl_strerror (gsl_errno);
}

//
int solve_system(gsl_vector *x, gsl_multifit_nlinear_fdf *fdf,
             gsl_multifit_nlinear_parameters *params){
  const gsl_multifit_nlinear_type *T = gsl_multifit_nlinear_trust;
  const size_t max_iter = 200;
  const double xtol = 1.0e-8;
  const double gtol = 1.0e-8;
  const double ftol = 1.0e-8;
  const size_t n = fdf->n;
  const size_t p = fdf->p;

  //Error handling
  int status;
  //gsl_set_error_handler_off();

  gsl_multifit_nlinear_workspace *work =
    gsl_multifit_nlinear_alloc(T, params, n, p);
  gsl_vector * f = gsl_multifit_nlinear_residual(work);
  gsl_vector * y = gsl_multifit_nlinear_position(work);
  int info;
  double chisq0, chisq, rcond;

  /* initialize solver */
  gsl_multifit_nlinear_init(x, fdf, work);

  /* store initial cost */
  gsl_blas_ddot(f, f, &chisq0);

  /* iterate until convergence */
  status = gsl_multifit_nlinear_driver(max_iter, xtol, gtol, ftol,
                              callback, NULL, &info, work);
  if (status) {
    // std::cerr << "There was an error: " << gsl_strerror (status) << "\n" << std::endl;
  }

  /* store final cost */
  gsl_blas_ddot(f, f, &chisq);

  /* store cond(J(x)) */
  gsl_multifit_nlinear_rcond(&rcond, work);

  gsl_vector_memcpy(x, y);


  // PRINT SUMMARY FOR TESTING PUPOSES
  // fprintf(stderr, "NITER         = %zu\n", gsl_multifit_nlinear_niter(work));
  // fprintf(stderr, "NFEV          = %zu\n", fdf->nevalf);
  // fprintf(stderr, "NJEV          = %zu\n", fdf->nevaldf);
  // fprintf(stderr, "NAEV          = %zu\n", fdf->nevalfvv);
  // fprintf(stderr, "initial cost  = %.12e\n", chisq0);
  // fprintf(stderr, "final cost    = %.12e\n", chisq);
  // int i;
  // for(i=0;i<p/3;i++){
  //   fprintf(stderr, "final x       = (%.12e, %.12e, %12e)\n",
  //         gsl_vector_get(x,i*3+0), gsl_vector_get(x,i*3+1), gsl_vector_get(x,i*3+2));
  //   fprintf(stderr, "final cond(J) = %.12e\n", 1.0 / rcond);
  // }


  gsl_multifit_nlinear_free(work);
  if(status){
    return 1;
  }
  else{
    return 0;
  }
}


//Find the peaks and return the peak count
int GaussianFitter::findPeaks(std::vector<Peak>* results,
                              std::vector<int> ampData,
                              std::vector<int> idxData){

  //Error handling
  int status;
  gsl_set_error_handler(handler);

  //figure out how many items there are in the ampData
  size_t n = ampData.size();

  //figure out how many peaks there are in the data
  std::vector<int> guesses = guessPeaks(ampData);
  size_t peakCount = guesses.size();

  // FOR TESTING PURPOSES
  // fprintf(stderr, "Peak count is %d\n", peakCount);

  size_t p = peakCount*3;

  // try to filter out some of the guesses if they are
  // near the noise

  //allocate space for fitting
  const gsl_rng_type * T = gsl_rng_default;
  gsl_vector *f = gsl_vector_alloc(n);
  gsl_vector *x = gsl_vector_alloc(p);
  gsl_multifit_nlinear_fdf fdf;
  gsl_multifit_nlinear_parameters fdf_params =
                                  gsl_multifit_nlinear_default_parameters();
  struct data fit_data;
  gsl_rng * r;
  int i;

  gsl_rng_env_setup ();
  r = gsl_rng_alloc (T);

  fit_data.t = (double*)malloc(n * sizeof(double));
  fit_data.y = (double*)malloc(n * sizeof(double));
  fit_data.n = n;

  //copy the data to a format
  for(i=0;i<ampData.size();i++){
    fit_data.t[i] = (double)idxData[i];
    fit_data.y[i] = (double)ampData[i];
  }

  //define function to be minimized
  fdf.f = func_f;
  fdf.df = func_df;
  fdf.fvv = func_fvv;
  fdf.fvv = NULL;
  fdf.n = n;
  fdf.p = p;
  fdf.params = &fit_data;

  //this is a guess starting point
  int j;
  for(i=0; i< peakCount; i++){
    gsl_vector_set(x, i*3+0, ampData[guesses[i]]);
    gsl_vector_set(x, i*3+1, idxData[guesses[i]]);

    // Create a better guess by using a better width
    int half_ampData_guess = ampData[guesses[i]]/2;
    int idx_lo=0,idx_hi=0;
    // look low
    for(j=guesses[i];j>0;j--){
      if(ampData[j] < half_ampData_guess){
        idx_lo = j;
        break;
      }
    }
    // look hi
    for(j=guesses[i];j<n;j++){
      if(ampData[j] < half_ampData_guess){
        idx_hi = j;
        break;
      }
    }
    int guess = idx_hi-idx_lo-1;
    if(guess > 20){guess = 10;}
    gsl_vector_set(x, i*3+2, guess);
  }

  fdf_params.trs = gsl_multifit_nlinear_trs_dogleg;
  //fdf_params.trs = gsl_multifit_nlinear_trs_lm;
  fdf_params.scale = gsl_multifit_nlinear_scale_more;
  fdf_params.solver = gsl_multifit_nlinear_solver_svd;
  fdf_params.fdtype = GSL_MULTIFIT_NLINEAR_CTRDIFF;

  if(!solve_system(x, &fdf, &fdf_params)){
    //this loop is going through every peak
    for(i=0; i< peakCount; i++){
      Peak* peak = new Peak();
      peak->amp = gsl_vector_get(x,3*i+ 0);
      peak->location = gsl_vector_get(x,3*i+ 1);
      double c = gsl_vector_get(x,3*i+ 2);

      //calculate fwhm: full width at half maximum
      // y = a * exp( -1/2 * [ (t - b) / c ]^2 )
      // where, y: amplitude at the t we are solving for
      //        a: amplitude at the peak
      //        t: time
      // time = +/-sqrt((-2)*(c^2)*ln(y/a) +b
      peak->fwhm_t_positive = sqrt((-2)*(c*c)*log((peak->amp/2)/peak->amp))
                              + peak->location;
      peak->fwhm_t_negative = (-1)*sqrt((-2)*(c*c)*log((peak->amp/2)/peak->amp))
                              + peak->location;
      peak->fwhm = abs(peak->fwhm_t_positive - peak->fwhm_t_negative);

      // FOR TESTING PURPOSES
      // printf("fwhm_t_positive: %f\nfwhm_t_negative: %f\n",
      //         peak->fwhm_t_positive, peak->fwhm_t_negative);
      // printf("fwhm: %f", peak->fwhm);

      //calculate triggering location
      peak->triggering_amp = noise_level + 1;
      peak->triggering_location = std::min(
            sqrt((-2)*(c*c)*log(peak->triggering_amp/peak->amp)) + peak->location,
       (-1)*sqrt((-2)*(c*c)*log(peak->triggering_amp/peak->amp)) + peak->location
                                          );
      if(peak->triggering_location > n || peak->triggering_location <0){
        //Print amplitude information that is causing the error
        // std::cerr << "\nTriggering location: "<< peak->triggering_location \
        //           << " not in range: " << n <<std::endl;
        // // FOR TESTING PURPOSES
        // std::cerr << "Amplitudes: " << std::endl;
        // for(int i=0; i< (int)ampData.size(); i++){
        //   std::cerr<< ampData[i] << " ";
        // }
        // std::cerr << std::endl ;
      }
      else{
        //add the peak to our result
        results->push_back(*peak);
      }
    }
  }
  else{
      // FOR TESTING PURPOSES
      // std::cerr << std::endl << "Exceeding max no of iterations: " << std::endl;
      // std::cerr << "Amplitudes: " << std::endl;

      // for(int i=0; i< (int)ampData.size(); i++){
      //   std::cerr<< ampData[i] << " ";
      // }
      // std::cerr << std::endl ;
      peakCount = 0;
  }

  // PRINT DATA AND MODEL FOR TESTING PURPOSES
  // for (i = 0; i < n; ++i){
  //     double ti = fit_data.t[i];
  //     double yi = fit_data.y[i];
  //     double fi = gaussianSum(x, ti);
  //     printf("%f %f %f\n", ti, yi, fi);
  //   }

  gsl_vector_free(f);
  gsl_vector_free(x);
  gsl_rng_free(r);

  return peakCount;
}


//Calculate the first difference
std::vector<int> GaussianFitter::calculateFirstDifferences(
                                                std::vector<int> ampData){
  int first, second, fDiff, count = 0;
  std::vector<int> firstDifference;

  for(int i = 0; i< (int)ampData.size(); i++){
    first = ampData[i+1];
    second = ampData[i+2];

    fDiff = second - first;

    firstDifference.push_back(fDiff);
    count++;

    if(count == 59){
        count = 0;
        i = i+2;
    }
  }
  return firstDifference;
}


// Estimate of peaks to be supplied to the gaussian fitter based on
// first difference gradient
std::vector<int> GaussianFitter::guessPeaks(std::vector<int> data){

  //std::vector<int> data = calculateFirstDifferences(ampData);
  std::vector<int> peaksLocation;

  //Level up to and including which peaks will be excluded
  //For the unaltered wave, noise_level = 16
  //for the scond derivative of the wave, noise_level = 3
  //
  // this is creating guesses for a guassian fitter that does not do
  // well if we have guesses that have an amplitde more than an order
  // of magnitute apart. We are going to set the nose level to be the
  // max value/ 10 - max*.05;
  int max = 0;
  for(int i = 0; i<(int)data.size(); i++){
    if(data[i]>max){
      max = data[i];
    }
  }
  noise_level = ((float)max)*.09;
  if(noise_level < 6){
    noise_level = 6;
  }
  int wideStart = -1;  //The start of any current wide peak

  //Sign of gradient:
  // =  1 for increasing
  // =  0 for level AND PREVIOUSLY INCREASING (so potential wide peak)
  // = -1 for decreasing OR level, but previously decreasing
  //A sharp peak is identified by grad=1 -> grad=-1
  //A wide  peak is identified by grad=0 -> grad=-1

  int prev_grad = -1;
  int grad = -1;
  for(int i = 0; i<(int)data.size(); i++){

    if(data[i] > noise_level){
      // sloping down
      if(data[i+1] < data[i]){
        // were we sloping up before?
        if(grad == 1){
          //record the peak
          peaksLocation.push_back(i);  //Peak location

        // previously flat
        }else if(grad == 0){
          int width = (i-wideStart);
          // if we were sloping down and then flat and then down
          // we need a width of 2
          if(prev_grad == -1){
            if(width >2){
              // record the center of the flat
              peaksLocation.push_back(i-(width/2));  //Peak location
            }
          }else{
            peaksLocation.push_back(i-(width/2));  //Peak location
          }
        }
        grad = -1;
      // sloping up
      }else if(data[i+1] > data[i]){
        //was flat
        if(grad == 0){
          // need to look back to before going flat. If we were
          // going down then do not record.
          if(prev_grad == 1){
            peaksLocation.push_back(i-((i-wideStart)/2));  //Peak location
          }
        }
        // previously sloping up or down
        grad = 1;
      }else{
        // if we were flat
        if(grad != 0){
          wideStart = i;
        }
        prev_grad = grad;
        grad = 0;
      }

    }
  }

  // FOR TESTING PURPOSES
  // std::cerr << std::endl << "Guesses: \n";
  // for(int i=0;i<peaksLocation.size();i++){
  //   std::cerr << peaksLocation[i] << " ";
  // }

  return peaksLocation;
}

