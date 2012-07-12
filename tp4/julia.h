#ifndef JULIA_H
#define JULIA_H

#include <gtkmm.h>
#include <string>
#include "types.h"

void julia_iteration(
  Glib::RefPtr<Gdk::Pixbuf> pixbuf,
  std::string function,
  complex_d p0, complex_d p1,
  double blowup,
  guint32 max_iter);

void julia_inverse_iteration(
  Glib::RefPtr<Gdk::Pixbuf> pixbuf,
  std::string function,
  complex_d p0, complex_d p1,
  double min_diff,
  guint32 max_iter);

void julia_preimage(
  Glib::RefPtr<Gdk::Pixbuf> pixbuf,
  std::string funcionF,
  complex_d p0, complex_d p1,
  guint32 seeds,
  double blowup,
  guint32 max_iter);

#endif
