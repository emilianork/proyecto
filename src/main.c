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


	/**
	 * Codigo de ejemplo:
	 * 1.  Genero las semillas y las guardo en una lista.
	 * 2.  La funcion process_incremental calcula el diagrama de voronoi.
	 * 3.  La salida se escribe en src/salida.txt o la pueden imprimir en 
	 *     pantalla.
	 */
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

