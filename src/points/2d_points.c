/**
 * Author: José Emiliano Cabrera Blancas (jemiliano.cabrera@gmail.com)
 * 
 */


#include "points/2d_points.h"
#include "types/types.h"

#include <stdlib.h>
#include <stdio.h>

vertex* init_point_empty()
{
	struct point* point = (struct point*) malloc(sizeof(struct point));

	if (point == NULL) {
		printf("Ya no hay memoria disponible: init_double_linked_list()");
		exit(EXIT_FAILURE);
	}
	
	int cero = 0.0;
	
	point->x = cero;
	point->y = cero;

	point->half_edge = point->intersection = point->face = NULL;
	point->distinct_color = FALSE;
	return point;
}

vertex* init_point(double x, double y, const char* name)
{
	struct point* point = (struct point*) malloc(sizeof(struct point));

	if (point == NULL) {
		printf("Ya no hay memoria disponible: init_double_linked_list()");
		exit(EXIT_FAILURE);
	}
	
	point->x = x;
	point->y = y;

	point->name = name;

	point->incident_edge = point->half_edge = point->intersection = NULL;
	point->face = NULL;
	point->distinct_color = FALSE;
	
	return point;
}

vertex* create_copy_point(vertex* point)
{
	struct point* new_point = init_point(point->x,point->y,point->name);
	
	new_point->incident_edge = point->incident_edge;
	new_point->half_edge = point->half_edge;
	new_point->intersection = point->intersection;

	new_point->face = point->face;
	new_point->distinct_color = point->distinct_color;
	
	return new_point;
}

void destroy_point(vertex* point)
{
	if (point != NULL)
		free(point);
}

int curve_orientation(vertex* a, vertex* b, vertex* c)
{
	double det_o = (b->x - a->x)*(c->y - a->y) - (c->x - a->x)*(b->y - a->y);

	if (det_o) {
		
		int count = 1;
		
		while (det_o == 0.0) {

			c->x += ((CONST*count)*EPSILON);
			c->y += ((CONST*count)*EPSILON);
		
		det_o = (b->x - a->x)*(c->y - a->y) - (c->x - a->x)*(b->y - a->y);

		count++;

		}
	}

	if (det_o < 0.0) {
		return RIGHT;
	} else if (det_o > 0.0) {
		return LEFT;
	} else {
		printf("Puntos en posicion general: ");
		printf("a = (%f,%f), b = (%f,%f), c = (%f,%f)\n",
			   a->x,a->y,b->x,b->y,c->x,c->y);
		exit(EXIT_FAILURE);
	}
}

int point_greater_than(vertex *a, vertex* b)
{
	if (a->y == b->y)
		return a->x < b->x;
	else
		return a->y > b->y;
}

int point_less_than(vertex* a, vertex* b)
{ 	
	if (a->y == b->y)
		return a->x > b->x;

	return a->y < b->y;
	
}

int point_equals(vertex* a, vertex* b)
{	
	
	return ((a->x - (CONST*EPSILON)) <= b->x && 
			b->x <= (a->x + (CONST*EPSILON)) &&
			(a->y - (CONST*EPSILON)) <= b->y &&
			b->y <= (a->y + (CONST*EPSILON)));

}

/** Funciones de orden con respecto al eje X. */
int point_less_than_x(vertex* a, vertex* b)
{
	return a->x < b->x;
}

int point_equals_x(vertex* a, vertex* b)
{
	return ((a->x - (CONST*EPSILON)) <= b->x && 
			b->x <= (a->x + (CONST*EPSILON)));
}

int point_equals_y(vertex* a, vertex* b)
{
	return ((a->y - (CONST*EPSILON)) <= b->y && 
			b->y <= (a->y + (CONST*EPSILON)));
}		
		

vertex* cast_point(void* a)
{
	return (vertex*) a;
}
