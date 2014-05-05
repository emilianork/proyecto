/**
 * Author: José Emiliano Cabrera Blancas (jemiliano.cabrera@gmail.com)
 *
 * Description:
 * Contiene la estructura que representa una recta y las operaciones que 
 * pueden hacer con esta.
 */

#ifndef EQUATIONS_H_
#define EQUATIONS_H_

#include "points/2d_points.h"
#include "half_edge/half_edge.h"

struct line {
	double m;
	double b;
};

typedef struct line line;

line* perpendicular_bisector(vertex* a, vertex* b);

vertex* intersection_segments(line* line, half_edge* half_edge);

#endif
