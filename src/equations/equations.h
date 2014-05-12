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

/**
 * Una linea es representada como y = m * x + b
 */
struct line {
	double m;
	double b;
};

/** Renombramos la estructura para un manejo mas facil. */
typedef struct line line;

/** 
 * Calcula el bisector perpendicular, que separa en dos partes al plano 
 * cartesiano, se represtan con una funcion.
 */
line* perpendicular_bisector(vertex* a, vertex* b);

/** 
 * Calcula el punto de interseccion de una arista con un bisecto.
 * Si la intersecion no existe, entonces va a regresar nulo, en otro
 * caso, regresa el punto donde se intersectan.
 */
vertex* intersection_segments(line* line, half_edge* half_edge);

#endif
