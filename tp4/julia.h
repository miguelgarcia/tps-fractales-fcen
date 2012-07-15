#ifndef JULIA_H
#define JULIA_H

#include <gtkmm.h>
#include <string>
#include "types.h"

void julia_iteration(
  guint32 *output,
  guint32 width,
  guint32 height,
  std::string function,
  complex_d p0, complex_d p1,
  double blowup,
  guint32 max_iter
);

void julia_inverse_iteration(
  guint32 *output,
  guint32 width,
  guint32 height,
  std::string function,
  std::complex<double> p0, std::complex<double> p1,
  guint32 max_iter,
  guint32 seeds);

void julia_preimage(
  guint32 *output,
  guint32 width,
  guint32 height,
  std::string function,
  complex_d p0, complex_d p1,
  guint32 iterations,
  guint32 init_seeds);

#endif
