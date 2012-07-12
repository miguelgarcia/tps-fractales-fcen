#include "julia.h"
#include <fparser.hh>
#include <cmath>
#include <iostream>

double norm(complex_d x)
{
  return sqrt((x.real() * x.real() + x.imag() * x.imag()));
}

complex_d rsqrt(const complex_d *z)
{
  if((rand() % 2) == 0)
  {
    return sqrt(*z);
  }
  else
  {
    return -sqrt(*z);
  }
}

FunctionParser_cd parse_function(std::string formula)
{
  FunctionParser_cd parser;
  parser.AddFunction("rsqrt", rsqrt, 1);
  int r = parser.Parse(formula, "z");
  if(r >= 0)
  {
    std::cout << parser.ErrorMsg() << "\n\n";
  }
  else
  {
    parser.Optimize();
  }

  return parser;
}

void julia_iteration(
  Glib::RefPtr<Gdk::Pixbuf> pixbuf,
  std::string function,
  std::complex<double> p0, std::complex<double> p1,
  double blowup,
  guint32 max_iter)
{
  FunctionParser_cd fz = parse_function(function);
  
  guint32 img_height = pixbuf->get_height();
  guint32 img_width = pixbuf->get_width();
  guint32 rowstride = pixbuf->get_rowstride();
  guint8 *image = pixbuf->get_pixels();

  double real_inc = (p1.real() - p0.real()) / ((double) img_width);
  double im_inc = (p1.imag() - p0.imag()) / ((double) img_height);
  complex_d vars[1];

  for(guint32 y=0; y < img_height; y++)
  {
    for(guint32 x=0; x < img_width; x++)
    {
      guint32 iter;
      vars[0] = complex_d((double) x * real_inc + p0.real(), (double) y * im_inc + p0.imag());
      for(iter = 0; iter < max_iter && norm(vars[0]) < blowup; iter++)
      {
        vars[0] = fz.Eval(vars);
      }
      image[y * rowstride + x * 3] = iter;
      image[y * rowstride + x * 3 + 1] = iter;
      image[y * rowstride + x * 3 + 2] = iter;
    }
  }
}

void julia_inverse_iteration(
  Glib::RefPtr<Gdk::Pixbuf> pixbuf,
  std::string function,
  std::complex<double> p0, std::complex<double> p1,
  double min_diff,
  guint32 max_iter)
{
  FunctionParser_cd fz = parse_function(function);
  
  guint32 img_height = pixbuf->get_height();
  guint32 img_width = pixbuf->get_width();
  guint32 rowstride = pixbuf->get_rowstride();
  guint8 *image = pixbuf->get_pixels();

  double real_inc = (p1.real() - p0.real()) / ((double) img_width);
  double im_inc = (p1.imag() - p0.imag()) / ((double) img_height);
  complex_d vars[1], new_z;
  guint seeds;
  for(seeds=0;seeds < 300; seeds++)
  {
    vars[0] = complex_d((rand() % img_width) * real_inc + p0.real(), (rand() % img_height)* im_inc + p0.imag());
    guint32 iter;
    max_iter = 10000;
    for(iter = 0; iter < max_iter; iter++)
    {
      new_z = fz.Eval(vars);
    //      diff = norm(new_z - vars[0]);
      vars[0] = new_z;

      if(iter > 50)
      {
        gint32 py = (new_z.imag() - p0.imag()) / im_inc;
        gint32 px = (new_z.real() - p0.real()) / real_inc;

        if(px >= 0 && px < img_width && py >= 0 && py < img_height)
        {
          if(image[py * rowstride + px * 3] < 255)
            image[py * rowstride + px * 3] += 5;// = 255;
          if(image[py * rowstride + px * 3 + 1] < 250)
            image[py * rowstride + px * 3 + 1] += 10;// = 255;
          if(image[py * rowstride + px * 3 + 2] < 255)
            image[py * rowstride + px * 3 + 2] += 15;// = 255;
        }
      }
    }
  }
}