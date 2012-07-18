#include <cmath>
#include <iostream>
#include "gaussian.h"

#define MAX_HEIGHT 255

using namespace std;

/**
 * 
 * @param m_result
 * @param side must be power of 2 + 1
 * @param h
 * @param noise
 */

#define AT(a, y, x) a[(y) * original_side + (x)]
void mdp2d(double * land, unsigned int original_side, double h, double noise)
{
  unsigned int parent_side = original_side, child_side;
  // seed corners
  AT(land, 0, 0) = gauss();
  AT(land, 0, original_side-1) = gauss();
  AT(land, original_side-1, 0) = gauss();
  AT(land, original_side-1, original_side-1) = gauss();
  
  double delta = 1;
  while(parent_side >= 3)
  {
    delta *= pow(0.5, 0.5 * h);
    child_side = parent_side / 2;
    //double noise_scale = child_side * noise;

    for(unsigned int square_left=0; square_left<original_side-1; square_left += (parent_side-1))
    {
      for(unsigned int square_top=0; square_top<original_side-1; square_top += (parent_side-1))
      {
        unsigned int center_y = square_top + child_side;
        unsigned int center_x = square_left + child_side;
        //cout << square_top << " " << square_left << " " << parent_side << " " << child_side << endl;
        AT(land, center_y, center_x) = delta * gauss() + (
                AT(land, square_top, square_left)
                + AT(land, square_top, square_left + parent_side - 1)
                + AT(land, square_top + parent_side - 1, square_left + parent_side - 1)
                + AT(land, square_top + parent_side - 1, square_left)) / 4.;
        
        AT(land, square_top, center_x) = delta * gauss() + (
                AT(land, square_top, square_left)
                + AT(land, square_top, square_left + parent_side - 1)) / 2.;
        
        AT(land, square_top+parent_side - 1, center_x) = delta * gauss() + (
                AT(land, square_top + parent_side - 1, square_left + parent_side - 1)
                + AT(land, square_top + parent_side - 1, square_left + parent_side - 1)) / 2.;
        
        AT(land, center_y, square_left) = delta * gauss() + (
                AT(land, square_top, square_left)
                + AT(land, square_top + parent_side - 1, square_left)) / 2.;
        
        AT(land, center_y, square_left + parent_side - 1) = delta * gauss() + (
                AT(land, square_top, square_left + parent_side - 1)
                + AT(land, square_top + parent_side - 1, square_left + parent_side - 1)) / 2.;
      }
    }
    parent_side = parent_side / 2 + 1;
  }
}
#undef AT