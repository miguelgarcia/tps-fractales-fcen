#include <cstdlib>

using namespace std;

// gauss (-1, 1)
double gauss()
{
  int nrands = 12;
  double sum = -nrands / 2;
  for(int i=0; i<nrands; i++)
  {
    sum += ((double) rand()) / ((double) RAND_MAX);
  }
  // sum in [-nrands / 2, nrands / 2]
  // normalize it
  return sum / ((double) nrands / 2);
}
