#include "graficador.h"
#include <fparser.hh>

using namespace std;
#define pow2(x) ((x)*(x))

void graficar_trayectorias(Cairo::RefPtr<Cairo::Context> cr,
        FunctionParser &funcionX, FunctionParser &funcionY,
        double p0[], double p1[], unsigned int semillas, unsigned int iteraciones)
{
  double minX = p0[0], minY = p0[0];
  double maxX = p1[0], maxY = p1[1];
  double incX = (maxX - minX) / semillas;
  double incY = (maxY - minY) / semillas;
  double width = maxX- minX;
  double seedX, seedY;
  double newX, newY;
  double vars[2];
  cr->set_source_rgb(0,0,1);
  for(seedX=minX + incX/2; seedX < maxX; seedX+=incX)
  {
    for(seedY=minY + incY/2; seedY < maxY; seedY+=incY)
    {
      // elegir punto inicial (pertutbado)
      newX = seedX+(rand()%100)*incX/100.;
      newY = seedY+(rand()%100)*incY/100.;
      vars[0] = newX;
      vars[1] = newY;
      cr->move_to(newX, newY);
      double alfa = 0.3;
      double radio = cr->get_line_width() / 2.;
      for(unsigned int i = 0; i < iteraciones; i++)
      {
        newX = funcionX.Eval(vars);
        newY = funcionY.Eval(vars);
        double norm = sqrt(pow2(newX-vars[0])+pow2(newY-vars[1]));

        double color = norm/width;
        //cr->set_source_rgba(color,4*(1-color)*color,1-color, 0.5);
        if(norm > 0)
        {
          // Moverse por el vector normalizado
          vars[0] += ((newX-vars[0]) / norm) * radio;
          vars[1] += ((newY-vars[1]) / norm) * radio;
        }
        //cr->rel_line_to(vars[0], vars[1]);
        //cr->stroke();
        cr->set_source_rgba(color,4*(1-color)*color,1-color, alfa);
        cr->arc(vars[0], vars[1], radio, 0,  2 * M_PI);
        cr->fill();
        cr->stroke();
        alfa *= 1.2;
        radio *= 1.1;
      }
    }
  }
}


/**
 * Inicializa la transformacion de coordenadas de reales a pixeles y pone en
 * blanco el fondo de la imagen.
 * 
 * @param cr
 * @param width
 * @param height
 * @param p0
 * @param p1
 */
void inicializar_cairo(Cairo::RefPtr<Cairo::Context> cr,
        guint32 width, guint32 height,
        double p0[], double p1[])
{
    //fondo blanco :)
  cr->set_source_rgb(1, 1, 1);
  cr->rectangle(0, 0, width, height);
  cr->fill();
  
  // Escala y posicion
  double sx = width / (p1[0] - p0[0]);
  double sy = height / (p1[1] - p0[1]);
  cr->translate(-p0[0] * sx, -p0[1] * sy);
  cr->scale(sx, sy);
  cr->set_line_width(0.002*(p1[0] - p0[0]));
}

FunctionParser parsear_formula(string formula)
{
  FunctionParser parser;
  parser.Parse(formula, "x,y");
  if(parser.EvalError() < 0)
  {
    std::cout << parser.ErrorMsg() << "\n\n";
  }
  else
  {
    parser.Optimize();
  }
  return parser;
}
/**
 * Genera el grafico de trayectoris de un DS 2D.
 * 
 * @param width
 * @param height
 * @param formulaX
 * @param formulaY
 * @param p0
 * @param p1
 * @param semillas
 * @param iteraciones
 * @return 
 */
Glib::RefPtr<Gdk::Pixbuf> graficar(guint32 width, guint32 height,
        string formulaX, string formulaY,
        double p0[], double p1[],
        unsigned int semillas, unsigned int iteraciones)
  {
  Cairo::RefPtr<Cairo::ImageSurface> imSur = Cairo::ImageSurface::create(Cairo::FORMAT_RGB24, width, height);
  Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(imSur);

  inicializar_cairo(cr, width, height, p0, p1);

  FunctionParser funcionX = parsear_formula(formulaX);
  FunctionParser funcionY = parsear_formula(formulaY);

  if(funcionX.EvalError() >= 0 && funcionY.EvalError() >= 0)
  {
    graficar_trayectorias(cr, funcionX, funcionY, p0, p1, semillas, iteraciones);
  }
  return Gdk::Pixbuf::create(imSur, 0, 0, width, height);
}