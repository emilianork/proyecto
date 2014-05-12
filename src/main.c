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

	//[26.0, 38.0], [215.0, 115.0], [122.0, 60.0], [66.0, 81.0],

	//10.000000 362.000000
	//309.000000 116.000000
	//103.000000 67.000000
	//87.000000 194.000000
	//277.000000 126.000000
	//309.000000 349.000000

	//299.0, 193.0
	//64.0,  58.0
	//40.0,  267.0
	//391.0, 87.0
	//218.0, 374.0,
	//284.0, 268.0
	//270.0, 226.

	vertex* A = init_point(50.0, 100.0, "\0");
	vertex* B = init_point(350.0, 100.0,  "\0");
	vertex* C = init_point(200.0, 100.0, "\0");
						   
	list* vertices = init_double_linked_list(POINT);
	
	push_back(vertices, A);
	push_back(vertices, B);
	push_back(vertices,C);
	
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

