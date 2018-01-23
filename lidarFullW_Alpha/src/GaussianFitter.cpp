/*
 * File name: GaussianFitter.cpp
 * Created on: 13-October-2017
 * Author: ravi
 */

#include "GaussianFitter.hpp"
#include <math.h>

struct data
{
  double *t;
  double *y;
  size_t n;
};

/* model function: a * exp( -1/2 * [ (t - b) / c ]^2 ) */
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
      //gsl_matrix_set(J, i,3*j+ 1, -(a / c) * ei * zi);
      gsl_matrix_set(J, i, 3*j+1, b_sum);

      // first derivative wrt c
      // a*(ti-b)*(ti-b) * ei * (1/(c*c*c))
      gsl_matrix_set(J, i, 3*j+2, c_sum);
    }
  }

  return GSL_SUCCESS;
}


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
         const gsl_multifit_nlinear_workspace *w)
{
  gsl_vector *f = gsl_multifit_nlinear_residual(w);
  gsl_vector *x = gsl_multifit_nlinear_position(w);
  double avratio = gsl_multifit_nlinear_avratio(w);
  double rcond;

  (void) params; /* not used */

  /* compute reciprocal condition number of J(x) */
  gsl_multifit_nlinear_rcond(&rcond, w);

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
          gsl_blas_dnrm2(f));
}

void solve_system(gsl_vector *x, gsl_multifit_nlinear_fdf *fdf,
             gsl_multifit_nlinear_parameters *params)
{
  const gsl_multifit_nlinear_type *T = gsl_multifit_nlinear_trust;
  const size_t max_iter = 200;
  const double xtol = 1.0e-8;
  const double gtol = 1.0e-8;
  const double ftol = 1.0e-8;
  const size_t n = fdf->n;
  const size_t p = fdf->p;
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
  gsl_multifit_nlinear_driver(max_iter, xtol, gtol, ftol,
                              callback, NULL, &info, work);

  /* store final cost */
  gsl_blas_ddot(f, f, &chisq);

  /* store cond(J(x)) */
  gsl_multifit_nlinear_rcond(&rcond, work);

  gsl_vector_memcpy(x, y);

  /* print summary */

  fprintf(stderr, "NITER         = %zu\n", gsl_multifit_nlinear_niter(work));
  fprintf(stderr, "NFEV          = %zu\n", fdf->nevalf);
  fprintf(stderr, "NJEV          = %zu\n", fdf->nevaldf);
  fprintf(stderr, "NAEV          = %zu\n", fdf->nevalfvv);
  fprintf(stderr, "initial cost  = %.12e\n", chisq0);
  fprintf(stderr, "final cost    = %.12e\n", chisq);
  fprintf(stderr, "final x       = (%.12e, %.12e, %12e)\n",
          gsl_vector_get(x, 0), gsl_vector_get(x, 1), gsl_vector_get(x, 2));
  fprintf(stderr, "final cond(J) = %.12e\n", 1.0 / rcond);

  gsl_multifit_nlinear_free(work);
}

//Find the peaks and return the peak count
int GaussianFitter::findPeaks(std::vector<Peak>* results,
                              std::vector<int> ampData,
                              std::vector<int> idxData){


  // figure out how many items there are in the ampData
  size_t n = ampData.size();

  // figure out how many peaks there are in the data
  std::vector<int> guesses = guessPeaks(ampData);
  size_t peakCount = guesses.size();
  fprintf(stderr, "Peak count is %d\n", peakCount);

  // allocate space for fitting
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

  // copy the data to a format
  for(i=0;i<ampData.size();i++){
    fit_data.t[i] = (double)idxData[i];
    fit_data.y[i] = (double)ampData[i];
  }

  // define function to be minimized
  fdf.f = func_f;
  fdf.df = func_df;
  fdf.fvv = func_fvv;
  fdf.fvv = NULL;
  fdf.n = n;
  fdf.p = p;
  fdf.params = &fit_data;

  // this is a guess starting point
  for(i=0; i< peakCount; i++){
    gsl_vector_set(x, i*3+0, ampData[guesses[i]] );
    gsl_vector_set(x, i*3+1, idxData[guesses[i]]);
    gsl_vector_set(x, i*3+2, 2);
  }

  fdf_params.trs = gsl_multifit_nlinear_trs_dogleg;
  solve_system(x, &fdf, &fdf_params);

  double fwhm_t_positive;
  double fwhm_t_negative;
  // this loop is going through every peak
  for(i=0; i< peakCount; i++){
    Peak* peak = new Peak();
      peak.amp = gsl_vector_get(x,3*i+ 0);
      peak.location = gsl_vector_get(x,3*i+ 1);

      // calculate fwhm full width at half maximum
      fwhm_t_positive = sqrt((-2)*(c^2)*ln(y/a));
      fwhm_t_negative = (-1)*sqrt((-2)*(c^2)*ln(y/a));        
      // peak.fwhm = ;

      // calculate activation point in t
      peaks.peak_triggering_location = noise_level + 1;

      // add the peak to our result
      results->push_back(&peak);
  }

  // print data and model
  {
    double A = gsl_vector_get(x, 0);
    double B = gsl_vector_get(x, 1);
    double C = gsl_vector_get(x, 2);

    for (i = 0; i < n; ++i){

        double ti = fit_data.t[i];
        double yi = fit_data.y[i];
        double fi = gaussianSum(x, ti);

        printf("%f %f %f\n", ti, yi, fi);
      }
  }

  gsl_vector_free(f);
  gsl_vector_free(x);
  gsl_rng_free(r);

  return peakCount;
}

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

std::vector<int> GaussianFitter::guessPeaks(std::vector<int> data){

  //std::vector<int> data = calculateFirstDifferences(ampData);
  std::vector<int> peaksLocation;

  /* Level up to and including which peaks will be excluded
   * For the unaltered wave, noise_level = 16
   * for the scond derivative of the wave, noise_level = 3
   */
  noise_level = 3;
  int wideStart = -1;  //The start of any current wide peak

 /* Sign of gradient
  * =  1 for increasing
  * =  0 for level AND PREVIOUSLY INCREASING (so potential wide peak)
  * = -1 for decreasing OR level, but previously decreasing
  * A sharp peak is identified by grad=1 -> grad=-1
  * A wide  peak is identified by grad=0 -> grad=-1
  */
  int grad = -1;

  int count = 1;  //Keep track of the index
  for(int i = 0; i<(int)data.size()-1; i++){
    //First index represents the pulse index
    if(count == 1){
      i = i+1;
      count = count + 1;
    }

    //Only possibility of a peak
    if(data[i+1] < data[i]){
      //Sharp peak
      if(grad == 1 && data[i] > noise_level){
        // peaks.push_back(data[i]);    //Peak value
        peaksLocation.push_back(i);  //Peak location
      }
      //Wide peak
      else if(grad == 0 && data[i] > noise_level){
        // peaks.push_back(data[wideStart]);
        if ((i - wideStart) % 2 == 0) {
          peaksLocation.push_back(wideStart);
        }
        else {
          peaksLocation.push_back(wideStart + (((i - wideStart) / 2) + 1));
        }

      }
      count++;
      grad = -1;
    }
    //Start of a wide peak
    else if (data[i+1] == data[i]){
      count++;
      if(grad == 1){
        wideStart = i;  //Index where the wide peak begins
        grad = 0;
      }
    }
    else{
      grad = 1;
      count++;
    }

    //Reset the index
    if (count == 60){
      count = 1;
    }

  }
  return peaksLocation;
}

