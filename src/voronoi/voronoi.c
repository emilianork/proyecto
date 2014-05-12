#include "voronoi/voronoi.h"

#include <stdio.h>
#include <stdlib.h>

voronoi* init_voronoi_diagram(double width, double height)
{
	voronoi* voronoi;
	voronoi = (struct voronoi_diagram*) malloc(sizeof(struct voronoi_diagram));

	if (voronoi == NULL) {
		printf("Ya no hay memoria disponible: init_voronoi_diagram()\n");
		exit(EXIT_FAILURE);
	}

	voronoi->diagram = construct_bounded_box(width, height);
	voronoi->processing = init_double_linked_list(FACE);
	
	voronoi->seeds = init_double_linked_list(POINT);

	voronoi->degenerate_case = FALSE;

	return voronoi;
}

void destroy_voronoi_diagram(voronoi* voronoi_diagram)
{
	free(voronoi_diagram);
}

dcel* construct_bounded_box(double width, double height)
{
	/** Declaramos e inicializamos los 4 vertices que definen a la caja. */
	vertex *A, *B, *C, *D;
	A = init_point(width, 0.0, "A\0");
	B = init_point(0.0,0.0, "B\0");
	C = init_point(0.0, height,"C\0");
	D = init_point(width,height,"D\0");
	
	/** Declaramos las semi aristas de la caja. */
	half_edge *a, *a_1, *b, *b_1, *c, *c_1, *d, *d_1; 
	a = init_half_edge(A,B, "a\0");
	a_1 = init_half_edge(B,A, "a'\0");
	
	b = init_half_edge(B,C, "b\0");
	b_1 = init_half_edge(C,B, "b'\0");

	c = init_half_edge(C,D, "c\0");
	c_1 = init_half_edge(D,C, "c'\0");

	d = init_half_edge(D,A, "d\0");
	d_1 = init_half_edge(A,D, "d'\0");
	
	/** Asignamos los valores correspondientes a cada semi arista. */
	a->twin = a_1;
	a_1->twin = a;

	b->twin = b_1;
	b_1->twin = b;
	
	c->twin = c_1;
	c_1->twin = c;
		
	d->twin = d_1;
	d_1->twin = d;
	
	
	a->next = b;
	b->next = c;
	c->next = d;
	d->next = a;
	
	a->prev = d;
	b->prev = a;
	c->prev = b;
	d->prev = c;

	a_1->next = d_1;
	b_1->next = a_1;
	c_1->next = b_1;
	d_1->next = c_1;

	a_1->prev = b_1;
	b_1->prev = c_1;
	c_1->prev = d_1;
	d_1->prev = a_1;

	/** Inicliazamos las dos caras. */
	list* f1_inner;
	f1_inner = init_double_linked_list(HALF_EDGE);
	push_back(f1_inner, a_1);
		
	face *f1, *f2;
	f1 = init_face("face 1\0", NULL, f1_inner);
	f2 = init_face("face 2\0", a, NULL);

	/** Le indicamos a las semi aristas quien es su cara adyacente. */
	a->incident_face = f2;
	a_1->incident_face = f1;

	b->incident_face = f2;
	b_1->incident_face = f1;

	c->incident_face = f2;
	c_1->incident_face = f1;
	
	d->incident_face = f2;
	d_1->incident_face = f1;

	f2->outer_component = a;

	/** Inicializamos la DCEL */
	dcel* bounded_box = init_dcel();
	
	dcel_insert_vertex(bounded_box, A);
	dcel_insert_vertex(bounded_box, B);
	dcel_insert_vertex(bounded_box, C);
	dcel_insert_vertex(bounded_box, D);

	dcel_insert_half_edge(bounded_box, a);
	dcel_insert_half_edge(bounded_box, a_1);
	dcel_insert_half_edge(bounded_box, b);
	dcel_insert_half_edge(bounded_box, b_1);
	dcel_insert_half_edge(bounded_box, c);	
	dcel_insert_half_edge(bounded_box, c_1);
	dcel_insert_half_edge(bounded_box, d);
	dcel_insert_half_edge(bounded_box, d_1);

	dcel_insert_face(bounded_box, f1);
	dcel_insert_face(bounded_box, f2);

	return bounded_box;
}
