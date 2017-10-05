#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlinear.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

g_fit() {
  const gsl_rng_type * T = gsl_rng_default;
  gsl_vector *f = gsl_vector_alloc(n);
  gsl_vector *x = gsl_vector_alloc(p);
  gsl_multifit_nlinear_fdf fdf;
  gsl_multifit_nlinear_parameters fdf_params =
    gsl_multifit_nlinear_default_parameters();
  struct data fit_data;
  gsl_rng * r;
  size_t i;

  gsl_rng_env_setup ();
  r = gsl_rng_alloc (T);



  //TODO
  /*
  int i = 0;
  int j = 0;
  for each peak {
    gsl_vector_set(x, i+0, peaks[j].amp);
    gsl_vector_set(x, i+1, peaks[j].time);
    gsl_vector_set(x, i+2, 5);  //TODO FIXME
    i+= 3;
    j++;
  }

   */
  gsl_vector_free(f);
  gsl_vector_free(x);
  gsl_rng_free(r);

  return 0;
  }
