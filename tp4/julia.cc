#include "julia.h"
#include <fparser.hh>
#include <cmath>
#include <iostream>
#include <stack>
#include <algorithm>

using namespace std;

//#define min(x,y) ((x) < (y) ? (x) : (y))

double norm(complex_d x)
{
  return sqrt((x.real() * x.real() + x.imag() * x.imag()));
}

complex_d rsqrt(const complex_d z)
{
  if((rand() % 2) == 0)
  {
    return sqrt(z);
  }
  else
  {
    return -sqrt(z);
  }
}

FunctionParser_cd parse_function(std::string formula)
{
  FunctionParser_cd parser;
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
      //color,4*(1-color)*color,1-color
      image[y * rowstride + x * 3] = iter;
      image[y * rowstride + x * 3 + 1] = min((guint32) 255, (255-iter) * iter);
      image[y * rowstride + x * 3 + 2] = 255 - iter;
    }
  }
}

void julia_inverse_iteration(
  Glib::RefPtr<Gdk::Pixbuf> pixbuf,
  std::string function,
  std::complex<double> p0, std::complex<double> p1,
  double min_diff,
  guint32 max_iter,
  guint32 seeds)
{
  FunctionParser_cd fz = parse_function(function);
  
  guint32 img_height = pixbuf->get_height();
  guint32 img_width = pixbuf->get_width();
  guint32 rowstride = pixbuf->get_rowstride();
  guint8 *image = pixbuf->get_pixels();

  double real_inc = (p1.real() - p0.real()) / ((double) img_width);
  double im_inc = (p1.imag() - p0.imag()) / ((double) img_height);
  complex_d vars[1], new_z;
  guint32 i, iter;
  double diff;
  
  for(i=0;i < seeds; i++)
  {
    vars[0] = complex_d((rand() % img_width) * real_inc + p0.real(), (rand() % img_height)* im_inc + p0.imag());
    diff = 1000;
    for(iter = 0; iter < max_iter && diff > min_diff; iter++)
    {
      new_z = rsqrt(fz.Eval(vars));
      diff = norm(new_z - vars[0]);
      vars[0] = new_z;

      gint32 py = (new_z.imag() - p0.imag()) / im_inc;
      gint32 px = (new_z.real() - p0.real()) / real_inc;

      if(px >= 0 && px < img_width && py >= 0 && py < img_height)
      {
        if(image[py * rowstride + px * 3] < 255)
          image[py * rowstride + px * 3] += 15;// = 255;
        if(image[py * rowstride + px * 3 + 1] < 255)
          image[py * rowstride + px * 3 + 1] += 15;// = 255;
        if(image[py * rowstride + px * 3 + 2] < 255)
          image[py * rowstride + px * 3 + 2] += 15;// = 255;
      }
    }
  }
}



void julia_preimage(
  Glib::RefPtr<Gdk::Pixbuf> pixbuf,
  std::string function,
  complex_d p0, complex_d p1,
  guint32 iterations,
  guint32 init_seeds)
{
  FunctionParser_cd fz = parse_function(function);
  
  guint32 img_height = pixbuf->get_height();
  guint32 img_width = pixbuf->get_width();
  guint32 rowstride = pixbuf->get_rowstride();
  guint8 *image = pixbuf->get_pixels();

  double real_inc = (p1.real() - p0.real()) / ((double) img_width);
  double im_inc = (p1.imag() - p0.imag()) / ((double) img_height);
  complex_d vars[1], new_z;
  
  std::stack<complex_d> *seeds, *next;
  seeds = new std::stack<complex_d>;
  next = new std::stack<complex_d>;
  
  for(guint32 i=0;i < init_seeds; i++)
  {
    seeds->push(complex_d((rand() % img_width) * real_inc + p0.real(), (rand() % img_height) * im_inc + p0.imag()));
  }
  
  for(guint32 i=0; i<iterations; i++)
  {
    while(!seeds->empty())
    {
      vars[0] = seeds->top();
      seeds->pop();
      new_z = sqrt(fz.Eval(vars));
      
      for(guint32 j=0; j <2; j++)
      { // 2 iteraciones, una para new_z y otra para -new_z
        next->push(new_z);      
        gint32 py = (new_z.imag() - p0.imag()) / im_inc;
        gint32 px = (new_z.real() - p0.real()) / real_inc;

        if(px >= 0 && px < img_width && py >= 0 && py < img_height)
        {
          if(image[py * rowstride + px * 3] < 255)
            image[py * rowstride + px * 3] += 5;// = 255;
          if(image[py * rowstride + px * 3 + 1] < 190)
            image[py * rowstride + px * 3 + 1] += 15;// = 255;
          if(image[py * rowstride + px * 3 + 2] < 128)
            image[py * rowstride + px * 3 + 2] = min(128, image[py * rowstride + px * 3 + 2]*image[py * rowstride + px * 3 + 2] + 2);// = 255;
        }
        new_z = -new_z;
      }
    }
    std::swap(seeds, next);
  }
}