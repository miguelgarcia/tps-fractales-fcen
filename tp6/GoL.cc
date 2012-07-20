#include "GoL.h"
#include <algorithm>

using namespace std;

unsigned int count_alive_neighbors(int x, int y, bool *grid, 
        int side)
{
  unsigned int alive = 0;
  int nx, ny;
  for(ny=max(0, y-1); ny<=min(side-1, y+1); ny++)
  {
    for(nx=max(0, x-1); nx<=min(side-1, x+1); nx++)
    {
      if(grid[ny * side + nx] && (nx != x || ny != y))
      {
        alive++;
      }
    }
  }
  return alive;
}

void GoL_iteration(int side, bool *grid, bool *next_grid,
        unsigned int min_life, unsigned int max_life,
        unsigned int min_born, unsigned int max_born)
{
  int x, y;
  unsigned int alive_neighbors;
  
  for(y=0; y<side; y++)
  {
    for(x=0; x<side; x++)
    {
      alive_neighbors = count_alive_neighbors(x, y, grid, side);
      
      next_grid[y * side + x] =
              (grid[y * side + x] && alive_neighbors >= min_life && alive_neighbors <= max_life)
              || (!grid[y * side + x] && alive_neighbors >= min_born && alive_neighbors <= max_born);
    }
  }
}

