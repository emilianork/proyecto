#include "equations/equations.h"

#include <stdio.h>
#include <stdlib.h>


line* perpendicular_bisector(vertex* a, vertex* b)
{

	if (a == NULL || b == NULL)
		return NULL;
		

	double x1 = a->x;
	double y1 = a->y;
	
	double x2 = b->x;
	double y2 = b->y;

	line* line = (struct line*) malloc(sizeof(struct line));

	line->b = (y2*y2 - y1*y1 + x2*x2 - x1*x1)/(2*(y2-y1));
	
	line->m = - (x2-x1)/(y2-y1);
	
	return line;
}

vertex* intersection_segments(line* line, half_edge* half_edge)
{
	double x1 = half_edge->first->x;
	double y1 = half_edge->first->y;

	double x2 = half_edge->last->x;
	double y2 = half_edge->last->y;

	double f_x1 = line->m*x1 + line->b; 
	double f_x2 = line->m*x2 + line->b;

	if ((y1 <= f_x1 && f_x2 <= y2) || (y2 <= f_x2 && f_x1 <= y1)) {
	
		struct line *second_line;
		second_line = (struct line*) malloc(sizeof(struct line));
		
		second_line->m = (y2-y1)/(x2-x1);
		second_line->b = (y1*x2 - x1*y2) / (x2-x1);

		double x = (second_line->b - line->b) / (line->m - second_line->m);
		double y = (line->m*(second_line->b - line->b) + 
					line->b*(line->m - second_line->m)) / (line->m - 
														   second_line->m);

		return init_point(x,y,"\0");
	}
		
	return NULL;
}
