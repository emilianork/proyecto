/**
 * Author: José Emiliano Cabrera Blancas (jemiliano.cabrera@gmail.com)
 *
 */

#include "dcel/dcel.h"
#include "double_linked_list/double_linked_list.h"

#include "algorithms/algorithms.h"
#include "voronoi/voronoi.h"

#include "equations/equations.h"

#include <stdlib.h>
#include <stdio.h>

int main(void)
{			  
	//10.000000 362.000000
	//309.000000 116.000000
	//103.000000 67.000000
	//87.000000 194.000000
	//277.000000 126.000000
	//309.000000 349.000000

	vertex* A = init_point(299.0, 193.0, "\0");
	vertex* B = init_point(64.0,  58.0,  "\0");
	vertex* C = init_point(40.0,  267.0, "\0");
	vertex* D = init_point(391.0, 87.0,  "\0");
	vertex* E = init_point(218.0, 374.0, "\0");
	vertex* F = init_point(284.0, 268.0, "\0");
	vertex* G = init_point(270.0, 226.0, "\0");

	list* vertices = init_double_linked_list(POINT);
	
	push_back(vertices, A);
	push_back(vertices, B);
	push_back(vertices, C);
	push_back(vertices, D);
	push_back(vertices, E);	
	push_back(vertices, F);
	push_back(vertices, G);
	

	voronoi* voronoi = process_incremental(400.0,400.0, vertices);

	printf("\n\nAristas Finales\n");
	
	item* tmps;
	for(tmps = voronoi->diagram->half_edge->head;
		tmps != NULL; tmps = tmps->right) {
		
		half_edge* tmp_he = tmps->element;
		
		printf("(%f,%f) (%f,%f)\n", tmp_he->first->x,
			   tmp_he->first->y, tmp_he->last->x, 
			   tmp_he->last->y);
		
	}
	return EXIT_SUCCESS;
}

