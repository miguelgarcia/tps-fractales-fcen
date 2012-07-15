#include "complex_utils.h"
#include <algorithm>

using namespace std;

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
