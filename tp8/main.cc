#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <gdkmm-3.0/gdkmm/pixbuf.h>

using namespace std;

typedef struct
{
  double a, b, c, d, e, f, probability;
} IFSFormula;

typedef struct
{
  double minX, maxX;
  double maxY, minY;
  double x0, y0;
  guint32 img_width, img_height;
  guint32 backcolor;
  guint32 frontcolor;
  guint32 n_formulas;
  IFSFormula *formulas;
} IFSDefinition;

void show_def(IFSDefinition *ifs)
{
  cout << "x range:" << ifs->minX << ", " << ifs->maxX << endl;
  cout << "y range:" << ifs->minY << ", " << ifs->maxY << endl;
  cout << "img size:" << ifs->img_width << " x " << ifs->img_height << endl;
  cout << "p0:" << ifs->x0 << ", " << ifs->y0 << endl;
  cout << "colors:" << ifs->backcolor << ", " << ifs->frontcolor << endl;
}

IFSDefinition *readIFS(ifstream &in)
{
  guint32 r, g ,b;
  
  IFSDefinition *def = new IFSDefinition;
  in >> def->minX;
  in >> def->maxX;
  in >> def->minY;
  in >> def->maxY;
  in >> def->x0;
  in >> def->y0;
  in >> def->img_width;
  in >> def->img_height;
  in >> r >> g >> b;
  def->backcolor = (r << 16) | (g << 8) | b;
  in >> r >> g >> b;
  def->frontcolor = (r << 16) | (g << 8) | b;
  in >> def->n_formulas;
  def->formulas = new IFSFormula[def->n_formulas];

  for(guint32 i=0; i<def->n_formulas; i++)
  {
    in >> def->formulas[i].a >> def->formulas[i].b >> def->formulas[i].c >> 
            def->formulas[i].d >> def->formulas[i].e >> def->formulas[i].f >> def->formulas[i].probability;
  }
  return def;
}

guint32 choose_formula(IFSFormula *formulas, guint32 n)
{
  guint32 formula_idx;
  double rnd = ((double) rand()) / ((double) RAND_MAX);
    // Elegir una formula segun su probabilidad
  for(formula_idx = 0; formula_idx < n; formula_idx++)
  {
    rnd -= formulas[formula_idx].probability;
    if(rnd <= 0)
    {
      return formula_idx;
    }
  }
  assert(formula_idx < n);
  return 0;
}

void dibujar(IFSDefinition *ifs, guint32 iteraciones, Glib::RefPtr<Gdk::Pixbuf> pbuf)
{
  double x = ifs->x0, y = ifs->y0;
  guint32 i, formula_idx;
  double x_scale = ifs->img_width / (ifs->maxX - ifs->minX);
  double y_scale = ifs->img_height / (ifs->maxY - ifs->minY);
  guint8 *pixels = pbuf->get_pixels();
  guint32 rowstride = pbuf->get_rowstride();
  pbuf->fill((ifs->backcolor << 8) | 0xFF);

  guint8 r = ifs->frontcolor >> 16;
  guint8 g = (ifs->frontcolor >> 8) & 0xFF;
  guint8 b = ifs->frontcolor  & 0xFF;
  
  for(i=0; i<iteraciones;i++)
  {

    formula_idx = choose_formula(ifs->formulas, ifs->n_formulas);
    // calcular el nuevo punto
    double new_x = ifs->formulas[formula_idx].a * x
      + ifs->formulas[formula_idx].b * y + ifs->formulas[formula_idx].e;
    double new_y = ifs->formulas[formula_idx].c * x
      + ifs->formulas[formula_idx].d * y + ifs->formulas[formula_idx].f;
    x = new_x;
    y = new_y;
    
    // convertir a pixeles
    int pixX = (x - ifs->minX) * x_scale;
    int pixY = (y - ifs->minY) * y_scale;
    pixX = max(0, min(pixX, (int) ifs->img_width));
    pixY = max(0, min(pixY, (int) ifs->img_height));
    pixels[pixY * rowstride + pixX * 3] = r;
    pixels[pixY * rowstride + pixX * 3 + 1] = g;
    pixels[pixY * rowstride + pixX * 3 + 2] = b;
  }
}

int main (int argc, char **argv)
{
  Gtk::Main kit(argc, argv);
  if(argc < 4)
  {
    cout << "Uso: tp8 input.in iterations output.png" << endl;
    return 1;
  }
  ifstream in(argv[1]);
  
  IFSDefinition *ifs = readIFS(in);
  show_def(ifs);
  Glib::RefPtr<Gdk::Pixbuf> pbuf = Gdk::Pixbuf::create(
    Gdk::COLORSPACE_RGB ,
    false,
    8,
    ifs->img_width,
    ifs->img_height
  );
  dibujar(ifs, atoi(argv[2]), pbuf);
  pbuf->rotate_simple(Gdk::PIXBUF_ROTATE_UPSIDEDOWN)->save(argv[3], "png");
  return 0;
}
