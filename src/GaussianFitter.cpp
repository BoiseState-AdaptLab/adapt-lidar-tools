//File name: GaussianFitter.cpp
//Created on: 13-October-2017
//Author: ravi

#include "GaussianFitter.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

#include "spdlog/spdlog.h"

GaussianFitter::GaussianFitter(){
    fail = 0;
    pass = 0;
    total = 0;

    //Set instance variables to default values, as defined in header.
    tolerance_scales = TOL_SCALES;
    x_tolerance = X_TOL;
    g_tolerance = G_TOL;
    f_tolerance = F_TOL;

    guess_lessthan_0_default = GUESS_LT0_DEFAULT;
    guess_upper_lim = GUESS_UPPER_LIM;
    guess_upper_lim_default = GUESS_UPPER_LIM_DEFAULT;

    max_amp_multiplier = MAX_AMP_MULTIPLIER;
    amp_lower_bound = AMP_LOWER_BOUND;

    log_diagnostics = true;
}

void GaussianFitter::setDiagnostics(bool newval) {
    log_diagnostics = newval;
}


void GaussianFitter::incr_total(){
    total++;
}


int GaussianFitter::get_total(){
    return total;

}


void GaussianFitter::incr_pass(){
    pass++;
}


int GaussianFitter::get_pass(){
    return pass;
}


void GaussianFitter::incr_fail(){
    fail++;
}


int GaussianFitter::get_fail(){
    return fail;
}


/**
 *
 */
struct data
{
    double *t;
    double *y;
    size_t n;
    double *peak_locations;
};


/* model function: a * exp( -1/2 * [ (t - b) / c ]^2 ) */
double
gaussian(const double a, const double b, const double c, const double t)
{
  const double z = (t - b) / c;
  return (a * exp(-0.5 * z * z));
}
/**
 * model function: a * exp( -1/2 * [ (t - b) / c ]^2 )
 * @param x
 * @param t
 * @return
 */
double gaussianSum(const gsl_vector * x,double* locs,const double t) {

    int i = 0;
    double value = 0.;
    int n = x->size/2;
    for(i=0;i<n;i++){
        double a = gsl_vector_get(x,2*i+0);
        double b = locs[i];
        double c = gsl_vector_get(x,2*i+1);
        value += gaussian(a,b,c,t);
    }
    return value;
}

int
func_f (const gsl_vector * x, void *params, gsl_vector * f)
{
  struct data *d = (struct data *) params;

  size_t i;
  for (i = 0; i < d->n; ++i) {

    double ti = d->t[i];
    double yi = d->y[i];
    double y = 0.;
      for(int j = 0; j<(x->size/2);j++){
        double a = gsl_vector_get(x, 2*j+0);
        double b = d->peak_locations[j];
        double c = gsl_vector_get(x, 2*j+1);

        y += gaussian(a, b, c, ti);
      }
      gsl_vector_set(f, i, yi - y);
    }

  return GSL_SUCCESS;
}

/**
 *
 * @param x
 * @param params
 * @param f
 * @return
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

 */

/**
 *
 * @param x, these are the unknowns being sent to the fitter
 * @param params, this is a struct that contains the raw data
 * @param J, internal state for the solver
 *
 * This function provides the Jacobian matrix. It comprises the
 * partial derivatives for the function that is
 * being matched. In this case we are using: f(x) = ae^(-4ln2((x-b)/w)^2)
 * We are going to simplify e^(-4ln2((x-b)^2/w^2)) to
 * J_{i,j} = \frac{\partial f_i(x)}{\partial x_j}
 * @return
 */
int
func_df (const gsl_vector * x, void *params, gsl_matrix * J)
{
  struct data *d = (struct data *) params;

  size_t i;
  for (i = 0; i < d->n; ++i){
    double ti = d->t[i];
    for(int j = 0; j<(x->size/2);j++){
      double a = gsl_vector_get(x, 2*j+0);
      double b = d->peak_locations[j];
      double c = gsl_vector_get(x, 2*j+1);

      double zi = (ti - b) / c;
      double ei = exp(-0.5 * zi * zi);

      gsl_matrix_set(J, i, 2*j+0, -ei);
      //gsl_matrix_set(J, i, 3*j+1, -(a / c) * ei * zi);
      gsl_matrix_set(J, i, 2*j+1, -(a / c) * ei * zi * zi);
    }
  }

  return GSL_SUCCESS;
}


/**
 *
 * @param x
 * @param v
 * @param params
 * @param fvv
 * @return
 */
int
func_fvv (const gsl_vector * x, const gsl_vector * v,
          void *params, gsl_vector * fvv)
{
  struct data *d = (struct data *) params;

  size_t i;
  for (i = 0; i < d->n; ++i) {

    double ti = d->t[i];
    double sum=0;
    for(int j = 0; j<(x->size/3);j++){
      double a = gsl_vector_get(x, 3*j+0);
      double b = gsl_vector_get(x, 3*j+1);
      double c = gsl_vector_get(x, 3*j+2);
      double va = gsl_vector_get(v, 3*j+0);
      double vb = gsl_vector_get(v, 3*j+1);
      double vc = gsl_vector_get(v, 3*j+2);

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

/**
 *
 * @param iter
 * @param params
 * @param w
 */
void callback(const size_t iter, void *params,
              const gsl_multifit_nlinear_workspace *w)
{
    gsl_vector *f = gsl_multifit_nlinear_residual(w);
    gsl_vector *x = gsl_multifit_nlinear_position(w);
    double avratio = gsl_multifit_nlinear_avratio(w);
    double rcond;

    /* compute reciprocal condition number of J(x) */
    gsl_multifit_nlinear_rcond(&rcond, w);

    if (*((bool*) params)) { //solve system passes log_diagnostics over params.
        size_t npeaks = x->size/2;
        spdlog::trace("iter {}:",iter);
        size_t j;
        for(j=0; j<npeaks; j++){
            spdlog::trace(
                "peak {}: amp = {:#.6g},  width = {:#.6g}",
                j,
                gsl_vector_get(x,2*j+ 0),
                //gsl_vector_get(x,3*j+ 1),
                gsl_vector_get(x,2*j+ 1));
        }
        spdlog::trace(
            "Also, |a|/|v| = {:#.6g} cond(J) = {:#.6g}, |f(x)| = {:#.6g}",
            avratio,
            1.0 / rcond,
            gsl_blas_dnrm2(f));
    }
}


/**
 *
 * @param reason
 * @param file
 * @param line
 * @param gsl_errno
 */
void handler (const char * reason,
              const char * file,
              int line,
              int gsl_errno)
{

    //std::cerr << "FATAL Error:" << file << ": " << reason << std::endl;
    gsl_strerror (gsl_errno);
}


/**
 *
 * @param x
 * @param fdf
 * @param params
 * @return
 */
int GaussianFitter::solve_system(gsl_vector *x, gsl_multifit_nlinear_fdf *fdf,
                 gsl_multifit_nlinear_parameters *params, int max,
                 const size_t max_iter) {
    const gsl_multifit_nlinear_type *T = gsl_multifit_nlinear_trust;

  const double xtol = 1e-8;
  const double gtol = 1e-8;
  const double ftol = 1e-8;

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
                                         callback, (void*) &log_diagnostics,
                                         &info, work);

    /* store final cost */
    gsl_blas_ddot(f, f, &chisq);
    /* store cond(J(x)) */
    gsl_multifit_nlinear_rcond(&rcond, work);
    gsl_vector_memcpy(x, y);
    gsl_multifit_nlinear_free(work);

    if(status){
        return 1;
    }
    else{
        return 0;
    }
}

/**
 * Find the peaks and return the peak count
 * @param results pointer to vector to store peaks
 * @param ampData
 * @param idxData
 * @param max_iter
 * @return count of found peaks
 */
int GaussianFitter::find_peaks(std::vector<Peak*>* results,
                               struct vector ampData,
                               struct vector idxData,
                               const size_t max_iter) {
    spdlog::trace("--NEW WAVEFORM--");

    incr_total();

    //Error handling
    int status;
    gsl_set_error_handler(handler);

    //Clear fitted equations
    equations.clear();

    //figure out how many items there are in the ampData
    size_t n = ampData.size;

    //get guessed peaks and figure out how many peaks there are
    size_t peakCount = guess_peaks(results, ampData, idxData);
    smoothing_expt(&ampData);

    spdlog::trace("Peak Count = {}", peakCount);

    //No peaks found
    //Prvents the "Parameter 7 to routine source_gemv_r.h was incorrect" error
    if(peakCount == 0){
      return 0;
    }


    size_t p = peakCount*2;

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
    fit_data.peak_locations = (double*)malloc(peakCount * sizeof(double));
    for(i=0; i < peakCount; i++){
      fit_data.peak_locations[i] = (*results)[i]->location;
    }

    //copy the data to a format
    for(i=0;i<n;i++){
        fit_data.t[i] = (double)idxData.buffer[i];
        fit_data.y[i] = (double)ampData.buffer[i];
    }

    //define function to be minimized
    fdf.f = func_f;
    fdf.df = func_df;
    //fdf.fvv = func_fvv;
    fdf.fvv = NULL;
    fdf.n = n;
    fdf.p = p;
    fdf.params = &fit_data;

    //this is a guess starting point
    int j;
    for(i=0; i< peakCount; i++){
        gsl_vector_set(x, i*2+0, (*results)[i]->amp);
        gsl_vector_set(x, i*2+1, (*results)[i]->fwhm/(2*log(2)));
    }

    // PRINT DATA AND MODEL FOR TESTING PURPOSES
    spdlog::trace("Gaussian sum based on guesses - before solve system:");
    std::ostringstream function;
    for (int i = 0; i < n; ++i){
        double ti = fit_data.t[i];
        // double yi = fit_data.y[i];
        function << gaussian((*results)[0]->amp,
                            (*results)[0]->location,
                            (*results)[0]->fwhm/2.35482,ti) << " ";
    }
    spdlog::trace("Model Data: {}", function.str());


    //fdf_params.trs = gsl_multifit_nlinear_trs_dogleg;
    //fdf_params.scale = gsl_multifit_nlinear_scale_more;
    //fdf_params.solver = gsl_multifit_nlinear_solver_svd;
    //fdf_params.fdtype = GSL_MULTIFIT_NLINEAR_CTRDIFF;


    if(!solve_system(x, &fdf, &fdf_params, max, max_iter)){
        incr_pass();

        // save value for later
        double neg4ln2 = -4.*log(2.);
        //this loop is going through every peak
        int i=0;
        for(auto iter = results->begin(); iter != results->end(); ++iter) {
            Peak *peak = *iter;
            peak->amp = gsl_vector_get(x,2*i+ 0) ;
            //peak->location =
            double w = gsl_vector_get(x,2*i+ 1);
            peak->fwhm = w*2*log(2);
            if(peak->fwhm < 0 ){
              peak->fwhm = peak->fwhm*(-1.);
            }
            
            //calculate triggering location
            peak->triggering_location = ceil(peak->location -
                sqrt((log(noise_level/peak->amp)*w*w)/neg4ln2));
            // TODO: fix to use correct function
            peak->triggering_amp = peak->amp * exp(neg4ln2 *
                (peak->triggering_location-peak->location)*
                (peak->triggering_location-peak->location)*
                (1/(w*w)));

            std::stringstream ss;
            ss << "y = " << peak->amp << "*e^(-4ln2*((t-" << peak->location <<
                ")/" << w << ")^2)";
            equations.push_back(ss.str());

            //calculate rise time
            peak->rise_time = peak->location - peak->triggering_location;

            // if any of these are true then the peaks and the whole wave
            // are invalid -- this should be logged
            if(peak->amp >= 300 ){
                spdlog::trace("Results invalid: amp too large");
                results->clear();
                break;
            }else if(peak->amp < (noise_level/2.)){
                spdlog::trace("Results invalid: amp too small");
                results->clear();
                break;
            }else if(peak->triggering_location > n){
                spdlog::trace("Results invalid: triggering location > n");
                results->clear();
                break;
            }else if(peak->triggering_location <0) {
                spdlog::trace("Results invalid: triggering location <n");
                results->clear();
                break;
            } else{
                //set the peak position in the wave
                //this will be wrong if a previous peak was removed for any
                //reason
                peak->position_in_wave = i+1;
            }

            if (!results->empty()) {
                Peak* final_peak_ptr = results->back();
                final_peak_ptr->is_final_peak = true; //mark the last peak as
                                                      //final
            }
            i++;
        }

        spdlog::trace("Number of Peaks Found: {}", results->size());

        // PRINT DATA AND MODEL FOR TESTING PURPOSES
        spdlog::trace("Gaussian sum in solve system succeeded:");
        // TODO: this should be wrapped in an ifdef
        std::ostringstream model;       
        for (int i = 0; i < n; ++i){
            double ti = fit_data.t[i];
            model << gaussianSum(x,fit_data.peak_locations, ti) << " ";
        }
        spdlog::trace("Model Data: {}", model.str());
    }else{

        incr_fail();
       
        // PRINT DATA AND MODEL FOR TESTING PURPOSES
        spdlog::trace("Gaussian sum in solve system failed:");
        std::ostringstream time, data, model;
        for (int i = 0; i < n; ++i){
            double ti = fit_data.t[i];
            time << ti << " ";
            data << fit_data.y[i] << " ";
            model << gaussianSum(x,fit_data.peak_locations, ti) << " ";
        }
        spdlog::trace("Time Data: {}",time.str());
        spdlog::trace("Amp Data: {}",data.str());
        spdlog::trace("Model Data: {}",model.str());
        results->clear();
    }

    free(fit_data.t);
    free(fit_data.y);
    gsl_vector_free(f);
    gsl_vector_free(x);
    gsl_rng_free(r);

    return results->size();
}


/**
 * Calculate the first difference
 * @param ampData
 * @return
 */
struct vector GaussianFitter::calculateFirstDifferences(
    struct vector ampData){
    int first, second, fDiff, count = 0;
    int n = (int)ampData.size-2;
    struct vector firstDifference;
    firstDifference.buffer = new int[n];
    firstDifference.size = 0;
    firstDifference.capacity = n;

    for(int i = 0; i< n; i++){
        first = ampData.buffer[i+1];
        second = ampData.buffer[i+2];

        fDiff = second - first;

        push(&firstDifference, fDiff);
        count++;

        //TODO: why 59?
        if(count == 59){
                count = 0;
                i = i+2;
        }
    }
    return firstDifference;
}


/**
 * Estimate of peaks to be supplied to the gaussian fitter based on
 * first difference gradient
 * Returns guesses of full width half maximum
 * @param results pointer to vector to store found peaks
 * @param ampData
 * @param idxData
 * @return number of peaks found
 */
int GaussianFitter::guess_peaks(std::vector<Peak*>* results,
                                struct vector ampData,
                                struct vector idxData) {
    //std::vector<int> data = calculateFirstDifferences(ampData);
    //Empty our results vector just to be sure
    //We need to start this function with a clear vector.
    //We can't call destructors because we don't know if the pointers
    //are pointing to space used in LidarVolume
    results->clear(); 

    std::vector<float> peak_guesses_loc; //Store peaks x-values here
    std::vector<float> peak_guesses_amp;
    int prev_grad = -1;
    int grad = -1;
    int firstDiffs[(int)ampData.size];
    int secondDiffs[(int)ampData.size];
    firstDiffs[1] = ampData.buffer[1] - ampData.buffer[0];
    for(int i = 2; i<(int)ampData.size; i++){
      firstDiffs[i] = ampData.buffer[i] - ampData.buffer[i-1];
      secondDiffs[i] = firstDiffs[i] - firstDiffs[i-1];
    }
    // for the most part any time we get to a flat portion
    // we will be counting that as a peak.
    // The exception is when the data never has a real peak.
    // We need to ignore those waveforms
    int realPeaks = 0;
    int n = (int)ampData.size;
    for(int i = 2; i<n; i++){
      if(grad == 0){
        if(firstDiffs[i] > 0){
          // flat to positive (record peak if not a trough)
          if(ampData.buffer[i-1]>noise_level && prev_grad == 1){
              //handle  flat peaks
              int j = i-2;
              for( ; ampData.buffer[j] == ampData.buffer[i-1] ; j--);
              // there is no body to the above for loop -- just looking
              // for the stopping point
              float lenOfFlat = i-1 -j -1.;
              //this is truncating, we need
              //to fix this
              float loc = i-1 - lenOfFlat/2.;
              //record the peak
              peak_guesses_loc.push_back(loc);
              peak_guesses_amp.push_back(ampData.buffer[i-1]);
          }
          grad = 1;
        }else if(firstDiffs[i] < 0 ){
          // flat to negative (record peak)
          if(ampData.buffer[i-1]>noise_level){
              if(prev_grad == 1){
                realPeaks++;
              }
              //handle  flat peaks
              int j = i-2;
              for( ; ampData.buffer[j] == ampData.buffer[i-1] ; j--);
              // there is no body to the above for loop -- just looking
              // for the stopping point
              float lenOfFlat = i-1 -j -1.;
              //this is truncating, we need
              //to fix this
              float loc = i-1 - lenOfFlat/2.;
              //record the peak
              peak_guesses_loc.push_back(loc);
              peak_guesses_amp.push_back(ampData.buffer[i-1]);
          }
          grad = -1;
        }

      }else if(grad == 1){
        prev_grad = 1;
        if(firstDiffs[i] == 0 ){
          grad = 0;
        }else if(firstDiffs[i] < 0){
          // positive to negative (record peak)
          if(ampData.buffer[i-1]>noise_level){
              realPeaks++;
              //record the peak
              peak_guesses_loc.push_back(float(i-1));
              peak_guesses_amp.push_back(ampData.buffer[i-1]);
          }
          grad = -1;
        }

      }else{ // gradient is -1
        prev_grad = -1;
        if(firstDiffs[i] == 0 ){
          // down to flat
          grad = 0;
        }else if(firstDiffs[i] > 0){
          // down to up
          grad = 1;
        }
      }
    }

    //Figure out how many peaks there are
    size_t peakCount = peak_guesses_loc.size();

    // make a guess for the fwhm value
    float neg4ln2 = -4.*log(2);
    int j;
    int peaks_found=0;
    if(realPeaks > 0){
    for(int i=0; i< peakCount; i++){
        // Create a better guess by using a better width
        float guess_lo = -1; // "guess" represents our guess of the width value.
        float guess_hi = -1; // "guess" represents our guess of the width value.
        float half_ampData_guess = peak_guesses_amp[i]/2.;
        int idx_lo=0,idx_hi=0;
        // look low
        int prev = ampData.buffer[(int)peak_guesses_loc[i]];
        for(j=(int)peak_guesses_loc[i];j>0;j--){
            if(ampData.buffer[j] > prev){
                break;
            }
            prev = ampData.buffer[j];
            if(prev < half_ampData_guess){
                idx_lo = j;
                double diff1 = idxData.buffer[j]-peak_guesses_loc[i];
                double ydiva = (double)ampData.buffer[j]/peak_guesses_amp[i];
                guess_lo = sqrt(neg4ln2*diff1*diff1*(1./(log(ydiva))));
                spdlog::trace("Found a guess for FWHM lo{}",guess_lo);
                break;
            }
        }
        // look high
        prev = peak_guesses_amp[i];
        for(j=(int)peak_guesses_loc[i]+1;j<n;j++){
            if(ampData.buffer[j] > prev){
                break;
            }
                prev = ampData.buffer[j];
                if(ampData.buffer[j] < half_ampData_guess){
                idx_hi = j;
                double diff1 = idxData.buffer[j]-peak_guesses_loc[i];
                double ydiva = (double)ampData.buffer[j]/peak_guesses_amp[i];
                guess_hi = sqrt(neg4ln2*diff1*diff1*(1./(log(ydiva))));
                spdlog::trace("Found a guess for FWHM hi{}",guess_hi);
                    break;
            }
        }

        double guess;
        if(guess_lo > 0 && guess_hi > 0){
          guess = (guess_lo+guess_hi)*.5;
        }else if(guess_lo > 0){
          guess = guess_lo;
        }else if(guess_hi > 0){
          guess = guess_hi;
        }else{
            spdlog::warn( "Guess for peak width less than zero, "
                          " reverting to default lo:{} hi{}",
                          guess_lo, guess_hi);

            guess = guess_lessthan_0_default;
        }

        if (log_diagnostics) {
            spdlog::debug(
                "Guess for peak {}: amp {}; time: {}; width: {}",
                i, peak_guesses_amp[i], peak_guesses_loc[i],
                guess);
        }

        if(guess > guess_upper_lim) {guess = guess_upper_lim_default;}
        peaks_found++;

        //Create a peak
        Peak* peak = new Peak();
        peak->amp = peak_guesses_amp[i];
        peak->location = peak_guesses_loc[i];
        peak->fwhm = guess;
        results->push_back(peak);
    }}else{
      results->clear();
    }
    if (results->size() != 0){
        results->back()->is_final_peak=true;
    }

    return results->size();
}


/**
 * Experiment with smoothing
 * @param waveArray
 */
void GaussianFitter::smoothing_expt(struct vector *waveArray){
    for(int i=0; i<waveArray->size;i++){
       waveArray->buffer[i] = waveArray->buffer[i]-1;
       if(waveArray->buffer[i] < 0){waveArray->buffer[i] = 0;}
    }
    for(int i=2; i<waveArray->size-1;i++){
        if(waveArray->buffer[i] < 7){
            int temp = (waveArray->buffer[i-2] + waveArray->buffer[i-1] +
                                    waveArray->buffer[i]+waveArray->buffer[i+1])/4;

            if(abs(temp-waveArray->buffer[i])<2){
                waveArray->buffer[i] = temp;
            }
        }
    }
}

/*
 * Get the equations for a peak
 * @param idx index of peak to get equation from
 * @return equation for that peak
*/
std::string GaussianFitter::get_equation(int idx){
    return (idx >= equations.size()) ? "" : equations.at(idx);
}
