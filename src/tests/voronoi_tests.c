#include "tests/voronoi_tests.h"
#include "types/types.h"

#include "double_linked_list/double_linked_list.h"

#include "points/2d_points.h"

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <math.h>

int simple_case()
{
	list* vertices = init_double_linked_list(POINT);

	vertex *A, *B;
	A = init_point(100.0,100.0, "\0");
	B = init_point(300.0,100.0, "\0");

	push_back(vertices, A);
	push_back(vertices, B);

	voronoi* voronoi = process_incremental((double)WIDTH,(double)HEIGHT,
										   vertices);

	vertex* intersection_a = search_vertex(voronoi->diagram,
										   init_point(200.0,400.0,"\n"));

	vertex* intersection_b = search_vertex(voronoi->diagram,
										   init_point(200.0,0.0,"\n"));

	return  (intersection_a != NULL && intersection_b != NULL);
}

int degenerate_case()
{
	list* vertices = init_double_linked_list(POINT);

	vertex *A, *B, *C;
	A = init_point(100.0,200.0, "\0");
	B = init_point(300.0,200.0, "\0");
	C = init_point(150.0,200.0, "\0");

	push_back(vertices, A);
	push_back(vertices, B);
	push_back(vertices, C);


	voronoi* voronoi = process_incremental((double)WIDTH, (double)HEIGHT,
										   vertices);

	vertex *tmp1, *tmp2, *tmp3, *tmp4;
	tmp1 = search_vertex(voronoi->diagram, init_point(125.0,400.0,"\0"));
	tmp2 = search_vertex(voronoi->diagram, init_point(125.0,0.0,"\0"));
	tmp3 = search_vertex(voronoi->diagram, init_point(225.0,400.0,"\0"));
	tmp4 = search_vertex(voronoi->diagram, init_point(225.0,0.0,"\0"));

	return  (tmp1 != NULL && tmp2 != NULL && tmp3 != NULL && tmp4 != NULL);

}


int verify_ecludian_distance(vertex* vertex, list* seeds) {

	double x1, y1, x2, y2;

	struct point* tmp_vertex = seeds->head->element;

	x1 = vertex->x;
	y1 = vertex->y;

	x2 = tmp_vertex->x;
	y2 = tmp_vertex->y;

	double first_distance = sqrt( pow( x2-x1 ,2.0)  + pow( y2-y1 ,2.0) );

	struct point* tmp1 = init_point(first_distance,0.0, "\0");

	item* tmp;
	for(tmp = seeds->head->right; tmp != NULL; tmp = tmp->right) {

		tmp_vertex = tmp->element;

		x2 = tmp_vertex->x;
		y2 = tmp_vertex->y;

		double tmp_distance = sqrt( pow( x2-x1 ,2.0)  + pow( y2-y1 ,2.0) );

		struct point* tmp2 = init_point(tmp_distance, 0.0, "\0");

		if (!(point_equals_x(tmp1, tmp2)))
			return FALSE;

	}

	return TRUE;
}


int general_case()
{

	list* seeds = init_double_linked_list(POINT);

	srand((unsigned)time(NULL));

	vertex* random_points[POINTS_SIZE];
	int i;
	for(i = 0; i < POINTS_SIZE; i++) {
		double x = (double)(rand() % (WIDTH - 1)) + 1;
		double y = (double)(rand() % (HEIGHT - 1)) + 1;

		random_points[i] = init_point(x, y, "\0");

		push_back(seeds,random_points[i]);

	}

	voronoi* voronoi = process_incremental((double)WIDTH, (double)HEIGHT,
										   seeds);

	/**
	 * 1. Recorro la lista de vertices y creo una lista que solo contenga
	 *    los vertices donde x != 0 && y != 0 && x != 400 && y != 400
	 **/


	vertex *tmp_v1, *tmp_v2;
	tmp_v1 = init_point(0.0, 0.0, "\0");
	tmp_v2 = init_point(400.0,400.0, "\0");

	list* vertices = init_double_linked_list(POINT);

	item* tmp;
	for(tmp = voronoi->diagram->vertex->head; tmp != NULL; tmp = tmp->right) {

		vertex* tmp_vertex = tmp->element;
		if (!(point_equals_x(tmp_v1, tmp_vertex) ||
			  point_equals_y(tmp_v1, tmp_vertex) ||
			  point_equals_x(tmp_v2, tmp_vertex) ||
			  point_equals_y(tmp_v2, tmp_vertex))) {

			push_back(vertices, tmp_vertex);
		}
	}

	if (vertices->size == 0) {
			return FALSE;
	}

	/**
	 * 2. Por cara vertice obtener las semillas de las caras adyacentes al punto
	 *    usando incident_he_v() y verificar que la distancia ecluidiana entre
	 *    las semillas y el vertice sean iguales.
	 */

	for(tmp = vertices->head; tmp != NULL; tmp = tmp->right) {
		vertex* tmp_vertex = tmp->element;
		list* incident_he = incident_he_to_v(tmp_vertex);

		list* incident_seeds = init_double_linked_list(POINT);

		item* tmp1;
		for (tmp1 = incident_he->head; tmp1 != NULL; tmp1 = tmp1->right) {

			half_edge* tmp_he = tmp1->element;
			face* tmp_face = tmp_he->incident_face;

			push_back(incident_seeds,tmp_face->center);
		}

		if (!(verify_ecludian_distance(tmp_vertex, incident_seeds))) {
			return FALSE;
		}
	}

	return TRUE;
}
