//File name: GaussianFitter.cpp
//Created on: 13-October-2017
//Author: ravi

#include "GaussianFitter.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

#include "spdlog/spdlog.h"
#include "Fitter.hpp"

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
                               std::vector<int> ampData,
                               std::vector<int> idxData,
                               const size_t max_iter) {
    assert(results);
    results->clear();

    if(ampData.empty()){
        return 0;
    }

    smoothing_expt(&ampData);
    std::vector<Fitter::Gaussian> guesses;
    Fitter::guessGaussians(idxData, ampData, noise_level, guesses);

    if(guesses.empty()){
        return 0;
    }

    bool result = Fitter::fitGaussians(idxData, ampData, guesses);

    if(!result){
        spdlog::error("Failed to fit waveform");
        return 0;   //@@TODO
    }

    //Fill in properties
    bool valid = true;
    for(const Fitter::Gaussian& peak : guesses){
        if(peak.a > 500 || peak.a < noise_level/2){ //@@TODO reasonable upper bound
            spdlog::error("Amplitude OOB: {}", peak.a);
            valid = false;
        }

        if(peak.b < 0 || peak.b > idxData.size()){  //@@TODO determine good bounds
            spdlog::error("Location OOB: {}", peak.b);
            valid = false;
        }

        if(peak.c <= 0.1){  //@@TODO determine good bounds
            spdlog::error("Width OOB: {}", peak.c);
            valid = false;
        }

        Peak* peakPtr = new Peak();
        peakPtr->amp = peak.a;
        peakPtr->location = peak.b;
        peakPtr->fwhm = peak.c * C_TO_FWHM;
        //@@TODO calculate other properties

        results->push_back(peakPtr);
    }

    if(!valid){
        for(Peak* ptr : *results){
            delete(ptr);
        }
        return 0;
    }

    return guesses.size();
}
/*


    spdlog::trace("--NEW WAVEFORM--");

    incr_total();

    //Error handling
    int status;
    gsl_set_error_handler(handler);

    //Clear fitted equations
    equations.clear();

    //figure out how many items there are in the ampData
    size_t n = ampData.size();

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
        fit_data.t[i] = (double)idxData[i];
        fit_data.y[i] = (double)ampData[i];
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
    std::vector<Fitter::Gaussian> guesses;
    int j;
    for(i=0; i< peakCount; i++){
        double a = (*results)[i]->amp;
        double b = (*results)[i]->location;
        double c = (*results)[i]->fwhm/C_TO_FWHM;
        gsl_vector_set(x, i*2+0, a);
        gsl_vector_set(x, i*2+1, c);
        guesses.emplace_back(a,b,c);
    }



    // PRINT DATA AND MODEL FOR TESTING PURPOSES
    spdlog::trace("Gaussian sum based on guesses - before solve system:");
    std::ostringstream function;
    for (int i = 0; i < n; ++i){
        double ti = fit_data.t[i];
        // double yi = fit_data.y[i];
        function << gaussian((*results)[0]->amp,
                            (*results)[0]->location,
                            (*results)[0]->fwhm/C_TO_FWHM,ti) << " ";
    }
    spdlog::trace("Model Data: {}", function.str());


    //fdf_params.trs = gsl_multifit_nlinear_trs_dogleg;
    //fdf_params.scale = gsl_multifit_nlinear_scale_more;
    //fdf_params.solver = gsl_multifit_nlinear_solver_svd;
    //fdf_params.fdtype = GSL_MULTIFIT_NLINEAR_CTRDIFF;


    //if(!solve_system(x, &fdf, &fdf_params, max, max_iter)){
    if(Fitter::fitGaussians(idxData, ampData, guesses)){
        incr_pass();

        // save value for later
        double neg4ln2 = -4.*log(2.);
        //this loop is going through every peak
        int i=0;
        for(auto iter = results->begin(); iter != results->end(); ++iter) {
            Peak *peak = *iter;

            std::stringstream before;
            before << "Before Fitting: y=" << peak->amp << "*e^(-.5*((t - "
                << peak->location << ") / " << peak->fwhm/C_TO_FWHM << ") ^ 2)";
            spdlog::debug(before.str());

            double a = guesses.at(i).a;
            double b = guesses.at(i).b;
            double c = guesses.at(i).c;
            peak->amp = a;
            peak->location = b;

            //peak->amp = gsl_vector_get(x,2*i+ 0);
            //peak->location =
            //double c = gsl_vector_get(x,2*i+ 1);
            peak->fwhm = c * C_TO_FWHM;
            if(peak->fwhm < 0 ){
              peak->fwhm = peak->fwhm*(-1.);
            }

            std::stringstream after;
            after << "After Fitting: y=" << peak->amp << "*e^(-.5*((t - "
                << peak->location << ") / " << c << ") ^ 2)";
            spdlog::debug(after.str());

            //calculate triggering location
            peak->triggering_location = ceil(peak->location -
                sqrt((log(noise_level/peak->amp)*c*c)/neg4ln2));
            // TODO: fix to use correct function
            peak->triggering_amp = peak->amp * exp(neg4ln2 *
                (peak->triggering_location-peak->location)*
                (peak->triggering_location-peak->location)*
                (1/(c*c)));

            //calculate rise time
            peak->rise_time = peak->location - peak->triggering_location;

            // if any of these are true then the peaks and the whole wave
            // are invalid -- this should be logged
            if(peak->amp >= 300 ){
                spdlog::error("Results invalid: amp too large");
                results->clear();
                break;
            }else if(peak->amp < (noise_level/2.)){
                spdlog::error("Results invalid: amp too small");
                results->clear();
                break;
            }else if(peak->triggering_location > n){
                spdlog::error("Results invalid: triggering location > n");
                results->clear();
                break;
            }else if(peak->triggering_location <0) {
                spdlog::error("Results invalid: triggering location <n");
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
*/


/**
 * Calculate the first difference
 * @param ampData
 * @return
 */
std::vector<int> GaussianFitter::calculateFirstDifferences(
    std::vector<int> ampData){
    int first, second, fDiff, count = 0;
    std::vector<int> firstDifference;
    int n = (int)ampData.size()-2;

    for(int i = 0; i< n; i++){
        first = ampData[i+1];
        second = ampData[i+2];

        fDiff = second - first;

        firstDifference.push_back(fDiff);
        count++;

        //TODO: why 59?
        if(count == 59){
                count = 0;
                i = i+2;
        }
    }
    return firstDifference;
}

/*
 * @param data - Amplitude data for this waveform
 * @param idx - Index to start at
 * @param left - Direction to iterate through data, iterates to the left if true,
 *               false otherwise
 * @return index of the point of greatest change on the curve
 */
int GaussianFitter::greatest_change(const std::vector<int> &data, int idx, int max_amp, bool left) {
    if (max_amp <= noise_level * 2) {return idx + (left ? -1 : 1);}
    float lb = max_amp / 2;
    while (idx > 0 && idx < (int)data.size() - 1) {
        int last_idx = idx + (left ? 1 : -1);
        int next_idx = idx + (left ? -1 : 1);
        int last_val = data[last_idx];
        int val = data[idx];
        int next_val = data[next_idx];
        // Stop looping if we hit the noise level, we start going up,
        // or we go below half the max amplitude
        if (next_val <= noise_level || next_val > val || next_val < lb) {
            return idx;
        }
        idx += left ? -1 : 1;
    }
    return idx;
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
                                const std::vector<int>& ampData,
                                const std::vector<int>& idxData) {
    //std::vector<int> data = calculateFirstDifferences(ampData);
    //Empty our results vector just to be sure
    //We need to start this function with a clear vector.
    //We can't call destructors because we don't know if the pointers
    //are pointing to space used in LidarVolume
    results->clear();

    std::vector<float> peak_guesses_idx; // Peak idices
    std::vector<float> peak_guesses_loc; // Peak t-values
    std::vector<float> peak_guesses_amp; // Peak a-values
    // Loop through the data
    for (int i = 1; i < (int)ampData.size() - 1; i ++) {
        int a1 = ampData[i - 1];
        int a2 = ampData[i];
        int a3 = ampData[i + 1];
        // Peak amplitude must be greater than 10
        if (a2 >= 10) {
            // We were going up before and are now going down
            if (a2 - a1 > 0 && a3 - a2 < 0) {
                // Record amplitude and time value in a new Peak object
                Peak* peak = new Peak();
                peak->amp = a2;
                int t2 = idxData[i];
                peak->location = t2;
                // Calculate FWHM - averaged from the point to the left and right
                int idx = greatest_change(ampData, i - 1, a2, true);
                float fwhm = get_fwhm(a2,t2,ampData[idx],idxData[idx]);
                idx = greatest_change(ampData, i + 1, a2, false);
                fwhm += get_fwhm(a2,t2,ampData[idx],idxData[idx]);
                peak->fwhm = fwhm / 2.;
                results->push_back(peak);
            } else if (a3 == a2) {
                // Store data point before the flat section
                int before = i - 1;
                // When this loop ends, i will point to the last data point
                // or the point right after the flat section
                while (i < (int)ampData.size() - 1 && a2 == a3){
                    i ++;
                    a3 = ampData[i + 1];
                }
                // Make sure the flat section did end and it isn't a trough
                if (a2 != a3 and (a2 > a1 or a2 > a3)){
                    // Get the center of the flat section
                    float t2 = (idxData[before+1] + idxData[i]) / 2.;
                    // Record amplitude and time value in a new Peak object
                    Peak* peak = new Peak();
                    peak->amp = a2;
                    peak->location = t2;
                    // Calculate FWHM
                    float fwhm =0.;
                    if (a2 > a1) {
                        int idx = greatest_change(ampData, before, a2, true);
                        fwhm = get_fwhm(a2,t2,ampData[idx], idxData[idx]);
                    }
                    if (a2 > a3) {
                        int idx = greatest_change(ampData, i, a2, false);
                        fwhm += get_fwhm(a2,t2,ampData[idx], idxData[idx]);
                        // Check if we are taking the average of two points or not
                        if (a2 > a1) {fwhm /= 2;}
                    }
                    peak->fwhm = fwhm;
                    results->push_back(peak);
                }
            }
        }
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
void GaussianFitter::smoothing_expt(std::vector<int> *waveArray){
    for(std::size_t i=0; i < waveArray->size(); ++i){
        waveArray->at(i) = std::max(waveArray->at(i)-1, 0);
    }

    int n = waveArray->size()-1;
    for(int i=2; i<n;i++){
        if(waveArray->at(i) < 7){
            int temp = (waveArray->at(i-2) + waveArray->at(i-1) +
                                    waveArray->at(i)+waveArray->at(i+1))/4;

            if(abs(temp-waveArray->at(i))<2){
                waveArray->at(i) = temp;
            }
        }
    }
}


/**
 * Calculate that FWHM given the peak and a data point of the gaussia curve
 * @param a, t - The amplitdue and time location of the peak
 * @param ai, ti - A data point on the gaussian curve of the above peak
 * @return fwhm - FWHM for that gaussian curve
 */
float GaussianFitter::get_fwhm(int a, float t, int ai, float ti){
    float w = 2 * (t - ti);
    if (w < 0) {w *= -1.;}
    float sqrt_lnN = sqrt(log((float)a / ai));
    return w * SQRT_LN2 / sqrt_lnN;
}

/*
 * Get the equations for a peak
 * @param idx index of peak to get equation from
 * @return equation for that peak
*/
std::string GaussianFitter::get_equation(int idx){
    return (idx >= equations.size()) ? "" : equations.at(idx);
}
