#ifndef GRAFICADOR_H
#define GRAFICADOR_H

#include <gtkmm.h>
#include <iostream>

using namespace std;

void graficar(Glib::RefPtr<Gdk::Pixbuf> pbuf, string formulaX, string formulaY,
  double p0[], double p1[], unsigned int semillas, unsigned int iteraciones);

#endif
