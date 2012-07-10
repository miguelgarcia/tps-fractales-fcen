#ifndef JULIA_H
#define JULIA_H

#include <gtkmm.h>
#include <complex>
#include <fparser.hh>

void julia_iteration(
  Glib::RefPtr<Gdk::Pixbuf> pixbuf,
  FunctionParser_cd function,
  std::complex<double> p0, std::complex<double> p1,
  guint32 max_iter,
  double blowup);

void julia_preimage(
  Glib::RefPtr<Gdk::Pixbuf> pixbuf,
  std::string funcionF,
  std::complex<double> p0, std::complex<double> p1,
  guint32 seeds,
  guint32 max_iter,
  double blowup);

#endif
