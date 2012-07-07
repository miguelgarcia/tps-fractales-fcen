#ifndef GRAFICADOR_H
#define GRAFICADOR_H

#include <gtkmm.h>
#include <iostream>

Glib::RefPtr<Gdk::Pixbuf> graficar(guint32 width, guint32 height,
        std::string formulaX, std::string formulaY,
        double p0[], double p1[],
        unsigned int semillas, unsigned int iteraciones);

#endif
