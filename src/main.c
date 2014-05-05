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

	voronoi *voronoi = init_voronoi_diagram(400.0,400.0);

	vertex* A = init_point(100.0,100.0,"\0");
	vertex* B = init_point(300.0,300.0,"\0");
	vertex* C = init_point(200.0,1,"\0");

	voronoi_incremental(voronoi, A);

	while(steps_voronoi(voronoi)) {
		voronoi_incremental(voronoi, NULL);
	}

	voronoi_incremental(voronoi, B);
	
	while(steps_voronoi(voronoi)) {
		voronoi_incremental(voronoi, NULL);
	}

	voronoi_incremental(voronoi, C);	

	while(steps_voronoi(voronoi)) {
		voronoi_incremental(voronoi, NULL);
	}

	return EXIT_SUCCESS;
}

