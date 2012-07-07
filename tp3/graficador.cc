#include "graficador.h"
#include <fparser.hh>

using namespace std;
#define pow2(x) ((x)*(x))
#define max(x,y) ((x) > (y) ? (x) : (y))

void putpixel(Glib::RefPtr<Gdk::Pixbuf> pbuf, int x, int y,
  guint8 r, guint8 g, guint8 b)
{
  if(x >= 0 && x < pbuf->get_width() && y >= 0 && y < pbuf->get_height())
  {
    int rowstride = pbuf->get_rowstride();
    guint8 *pixels = pbuf->get_pixels();
    pixels[y * rowstride + x * 3] = r; 
    pixels[y * rowstride + x * 3 + 1] = g; 
    pixels[y * rowstride + x * 3 + 2] = b; 
  }
}

void putline(Glib::RefPtr<Gdk::Pixbuf> pbuf, int x0, int y0, int x1, int y1,
  guint8 r, guint8 g, guint8 b)
{
  if(x0 == x1 && y0 == y1)
    return;
  int rowstride = pbuf->get_rowstride();
  guint8 *pixels = pbuf->get_pixels();
  int lineY = y0;
  for(int lineX = x0; lineX != x1; lineX += (x0 < x1 ? 1 : -1))
  {
    if(lineX >= 0 && lineX < pbuf->get_width() && lineY >= 0 && lineY < pbuf->get_height())
    {
      pixels[lineY * rowstride + lineX * 3] = r; 
      pixels[lineY * rowstride + lineX * 3 + 1] = g; 
      pixels[lineY * rowstride + lineX * 3 + 2] = b;
    }
    lineY += (y0 < y1 ? 1 : -1);
  }
  putpixel(pbuf, x0, y0, 0, 255, 0);
}

void graficar(Glib::RefPtr<Gdk::Pixbuf> pbuf, string formulaX, string formulaY,
  double p0[], double p1[], unsigned int semillas, unsigned int iteraciones)
{
  double minX = p0[0], minY = p0[0];
  double maxX = p1[0], maxY = p1[1];
  double incX = (maxX - minX) / semillas;
  double incY = (maxY - minY) / semillas;
  double x, y;
  double newX, newY;
  double vars[2];
  unsigned int pbuf_width = pbuf->get_width();
  unsigned int pbuf_height = pbuf->get_height();
  FunctionParser fparserX, fparserY;

  if(formulaX == "" || formulaY == "")
    return;

  int res = fparserX.Parse(formulaX, "x,y");
  if(fparserX.EvalError() < 0)
  {
    std::cout << fparserX.ErrorMsg() << "\n\n";
    return;
  }
  res = fparserY.Parse(formulaY, "x,y");
  if(fparserY.EvalError() < 0)
  {
    std::cout << fparserY.ErrorMsg() << "\n\n";
    return;
  }
  fparserX.Optimize();
  fparserY.Optimize();
  cout << "Graficando" << endl;
  double maxInc = max(incX, incY);
  int pixOldX, pixOldY;
  for(x=minX + incX/2; x < maxX; x+=incX)
  {
    for(y=minY + incY/2; y < maxY; y+=incY)
    {
      // elegir punto inicial
      newX = x+(rand()%100)*incX/100.;
      newY = y+(rand()%100)*incY/100.;
      vars[0] = newX;
      vars[1] = newY;
      for(unsigned int i = 0; i < iteraciones; i++)
      {
        newX = fparserX.Eval(vars);
        newY = fparserY.Eval(vars);
        double norm = sqrt(pow2(newX-vars[0])+pow2(newY-vars[1]));
        if(norm > 0)
        {
          // Moverse por el vector normalizado
          vars[0] += (newX / norm);
          vars[1] += (newY / norm);
        }
        int pixX = (vars[0]-minX) / (maxX - minX) * pbuf_width;
        int pixY = (vars[1]-minY) / (maxY - minY) * pbuf_height;
        unsigned int color = 20. * min(12., norm);
        if(i > 0)
        {
          putline(pbuf, pixX, pixY, pixOldX, pixOldY, color, 0, 0);
        }
        //putpixel(pbuf, pixX, pixY, color, 0, 0);
        pixOldX = pixX;
        pixOldY = pixY;
      }
    }
  }
}
