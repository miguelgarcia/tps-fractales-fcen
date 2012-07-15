#include "julia.h"
#include "complex_utils.h"
#include <fparser.hh>
#include <cmath>
#include <iostream>
#include <stack>
#include <algorithm>

using namespace std;

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

/**
 * Dibuja Julia por iteracion directa.
 * 
 * @param output
 * @param width
 * @param height
 * @param function
 * @param p0
 * @param p1
 * @param blowup
 * @param max_iter
 */
void julia_iteration(
  guint32 *output,
  guint32 width,
  guint32 height,
  std::string function,
  std::complex<double> p0, std::complex<double> p1,
  double blowup,
  guint32 max_iter)
{
  FunctionParser_cd fz = parse_function(function);

  double real_inc = (p1.real() - p0.real()) / ((double) width);
  double im_inc = (p1.imag() - p0.imag()) / ((double) height);
  complex_d vars[1];

  for(guint32 y=0; y < height; y++)
  {
    for(guint32 x=0; x < width; x++)
    {
      guint32 iter;
      vars[0] = complex_d((double) x * real_inc + p0.real(), (double) y * im_inc + p0.imag());
      for(iter = 0; iter < max_iter && norm(vars[0]) < blowup; iter++)
      {
        vars[0] = fz.Eval(vars);
      }
      output[y * width + x] = iter;
    }
  }
}


void julia_inverse_iteration(
  guint32 *output,
  guint32 width,
  guint32 height,
  std::string function,
  std::complex<double> p0, std::complex<double> p1,
  guint32 max_iter,
  guint32 seeds)
{
  FunctionParser_cd fz = parse_function(function);

  double real_inc = (p1.real() - p0.real()) / ((double) width);
  double im_inc = (p1.imag() - p0.imag()) / ((double) height);
  complex_d vars[1], new_z;
  guint32 i, iter;
  
  for(i=0;i < seeds; i++)
  {
    vars[0] = complex_d((rand() % width) * real_inc + p0.real(), (rand() % height)* im_inc + p0.imag());
    for(iter = 0; iter < max_iter; iter++)
    {
      new_z = rsqrt(fz.Eval(vars));
      vars[0] = new_z;

      gint32 y = (new_z.imag() - p0.imag()) / im_inc;
      gint32 x = (new_z.real() - p0.real()) / real_inc;

      if(x >= 0 && x < width && y >= 0 && y < height)
      {
        output[y * width + x] += 1;
      }
    }
  }
}



void julia_preimage(
  guint32 *output,
  guint32 width,
  guint32 height,
  std::string function,
  complex_d p0, complex_d p1,
  guint32 iterations,
  guint32 init_seeds)
{
  FunctionParser_cd fz = parse_function(function);

  double real_inc = (p1.real() - p0.real()) / ((double) width);
  double im_inc = (p1.imag() - p0.imag()) / ((double) height);
  complex_d vars[1], new_z;
  
  std::stack<complex_d> *seeds, *next;
  seeds = new std::stack<complex_d>;
  next = new std::stack<complex_d>;
  
  for(guint32 i=0;i < init_seeds; i++)
  {
    seeds->push(complex_d((rand() % width) * real_inc + p0.real(), (rand() % height) * im_inc + p0.imag()));
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
        gint32 y = (new_z.imag() - p0.imag()) / im_inc;
        gint32 x = (new_z.real() - p0.real()) / real_inc;

        if(x >= 0 && x < width && y >= 0 && y < height)
        {
          output[y * width + x] += 1;;
        }
        new_z = -new_z;
      }
    }
    std::swap(seeds, next);
  }
}