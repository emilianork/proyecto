#include "tests/voronoi_tests.h"
#include "types/types.h"

#include "double_linked_list/double_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

int simple_case()
{
	list* vertices = init_double_linked_list(POINT);

	vertex *A, *B;
	A = init_point(100.0,100.0, "\0");
	B = init_point(300.0,100.0, "\0");

	push_back(vertices, A);
	push_back(vertices, B);

	voronoi* voronoi = process_incremental(WIDTH,HEIGHT,vertices);

	vertex* intersection_a = search_vertex(voronoi->diagram,
										   init_point(200.0,400.0,"\n"));
	
	vertex* intersection_b = search_vertex(voronoi->diagram,
										   init_point(200.0,-0.0,"\n"));

	if (intersection_a != NULL && intersection_b != NULL)
		return TRUE;
	else
		return FALSE;
	
}

int degenerate_case()
{
	return TRUE;
}
