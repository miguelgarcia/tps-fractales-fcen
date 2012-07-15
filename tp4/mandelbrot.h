/* 
 * File:   mandelbrot.h
 * Author: mike
 *
 * Created on 15 de julio de 2012, 19:40
 */

#ifndef MANDELBROT_H
#define	MANDELBROT_H

#include <gtkmm.h>
#include <string>
#include "types.h"

void mandelbrot_direct_iteration(
  guint32 *output,
  guint32 width,
  guint32 height,
  std::string function,
  complex_d p0, complex_d p1,
  double blowup,
  guint32 max_iter
);

#endif	/* MANDELBROT_H */

