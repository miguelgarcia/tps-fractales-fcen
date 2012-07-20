/* 
 * File:   GoL.h
 * Author: mike
 *
 * Created on 20 de julio de 2012, 10:57
 */

#ifndef GOL_H
#define	GOL_H


void GoL_iteration(int side, bool *grid, bool *next_grid,
        unsigned int min_life, unsigned int max_life,
        unsigned int min_born, unsigned int max_born);

#endif	/* GOL_H */

