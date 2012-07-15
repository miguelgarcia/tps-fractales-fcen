#include "mandelbrot.h"
#include "complex_utils.h"
#include <fparser.hh>
#include <iostream>

using namespace std;

FunctionParser_cd parse_function_mandelbrot(std::string formula)
{
  FunctionParser_cd parser;
  int r = parser.Parse(formula, "z,c");
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
 * Dibuja Mandelbrot por iteracion directa.
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
void mandelbrot_direct_iteration(
  guint32 *output,
  guint32 width,
  guint32 height,
  std::string function,
  std::complex<double> p0, std::complex<double> p1,
  double blowup,
  guint32 max_iter)
{
  FunctionParser_cd fz = parse_function_mandelbrot(function);

  double real_inc = (p1.real() - p0.real()) / ((double) width);
  double im_inc = (p1.imag() - p0.imag()) / ((double) height);
  complex_d vars[2];

  for(guint32 y=0; y < height; y++)
  {
    for(guint32 x=0; x < width; x++)
    {
      guint32 iter;
      // calcular el C para este punto
      vars[1] = complex_d((double) x * real_inc + p0.real(), (double) y * im_inc + p0.imag());
      vars[0] = complex_d(0, 0); // z = 0
      for(iter = 0; iter < max_iter && norm(vars[0]) < blowup; iter++)
      {
        vars[0] = fz.Eval(vars);
      }
      output[y * width + x] = iter;
    }
  }
}