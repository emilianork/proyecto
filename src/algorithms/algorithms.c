#include "algorithms/algorithms.h"

#include "red_black_tree/rb_tree.h"
#include "types/types.h"

#include "points/2d_points.h"

#include "equations/equations.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

up_data* init_upgrade_data(void)
{
	up_data* data = (up_data*) malloc(sizeof(struct upgrade_data));

	data->center = NULL;
	
	data->a = NULL;
	data->intersection_a = NULL;
	
	data->b = NULL;
	data->intersection_b = NULL;

	data->voronoi = NULL;

	data->intersection_repeat = FALSE;

	return data;

}

void destroy_upgrade_data(up_data* data)
{
	free(data);
}


int steps_voronoi(voronoi* voronoi)
{
	if (voronoi == NULL)
		return FALSE;

	if (!empty_list(voronoi->processing))
		return TRUE;

	return FALSE;
}

void voronoi_incremental(voronoi* voronoi, vertex* vertex)
{
	if (voronoi == NULL)
		return;
	
    if (vertex != NULL) {
		
		/** Obtengo la lista de caras. */
		list* faces = rb_tree_to_list(voronoi->diagram->face);
		
		face* face = NULL;
		item *tmp;
		
		/** Itero las caras hasta dar con la que contiene a punto vertex. */
		for (tmp = faces->head; tmp != NULL; tmp = tmp->right) {
			
			/** Descarto la cara exterior de la caja. */
			if ( ((struct face*)tmp->element)->outer_component !=  NULL) {
				if (contain_vertex(tmp->element, vertex)) {
					face = tmp->element;
					break;
				}
				
			}
		}

		push_back(voronoi->processing, face);
		push_back(voronoi->seeds, vertex);		

		return;
		
	} 

	if (!empty_list(voronoi->processing)) {
		
		struct point* seed = voronoi->seeds->tail->element;
		
		face* face = pop_front(voronoi->processing);

		if (voronoi->seeds->size == 1) {
			
			face->center = seed;			
			seed->face = face;
			seed->distinct_color = FALSE;

		} else {

			list* incident_half_edge = incident_he_to_f(face);
			line* bisector = perpendicular_bisector(face->center, seed);
			
			half_edge* he_incident_1 = NULL;
			half_edge* he_incident_2 = NULL;
			
			struct point* intersection_1 = NULL;
			struct point* intersection_2 = NULL;
			
			item* tmp;
			for (tmp = incident_half_edge->head; tmp != NULL;
				 tmp = tmp->right) {

				struct point* intersection;
				intersection = intersection_segments(bisector, tmp->element);
				
				if (intersection != NULL) {
					if (he_incident_1 == NULL) {
						he_incident_1 = tmp->element;
						intersection_1 = intersection;
					} else {
						if (intersection_1->x == intersection->x &&
							intersection_1->y == intersection->y) {
							destroy_point(intersection);
						} else {
							he_incident_2 = tmp->element;
							intersection_2 = intersection;
						}
					}
				}
				
			}
			
			up_data* data = init_upgrade_data();
			
			data->center = seed;
			
			data->a = he_incident_1;
			data->intersection_a = intersection_1;
			
			data->b = he_incident_2;
			data->intersection_b = intersection_2;
			
			data->voronoi = voronoi;

			if (!contain_vertex(face, seed)) {				
				if (data->a->twin->incident_face == seed->face) {

					data->intersection_repeat = TRUE;

					data->a = he_incident_2;
					data->intersection_a = intersection_2;

					data->b = he_incident_1;
					data->intersection_b = intersection_1;
				}

			}

			add_half_edge_voronoi(data);
			
			destroy_upgrade_data(data);
		}
		
		if (empty_list(voronoi->processing)) {
			struct point* last_seed = voronoi->seeds->tail->element;
			last_seed->distinct_color = FALSE;
		}
		
		return;		
	}
	
	return;
}

void add_half_edge_voronoi(up_data* data)
{	
	vertex* center = data->center;
	
	half_edge* a = data->a;
	vertex* intersection_a = data->intersection_a;

	half_edge* b = data->b;
	vertex* intersection_b = data->intersection_b;

	dcel* diagram = data->voronoi->diagram;

	voronoi* voronoi = data->voronoi;

	if (!data->intersection_repeat) {

		struct face* face_1 = a->twin->incident_face; 
		struct face* face_2 = b->twin->incident_face;

		if (face_1->outer_component != NULL)
			push_back(voronoi->processing, face_1);
		if (face_2->outer_component != NULL)
			push_back(voronoi->processing, face_2);

		if (face_1->outer_component == NULL && 
			face_2->outer_component == NULL) {

			/**
			*
			*
			*
			*/

		}
		half_edge *tmp1, *tmp2;

		if (!(point_equals(a->first, intersection_a) ||
			point_equals(a->last, intersection_a))) {
			/** Arreglo a */

		
			tmp1 = init_half_edge(intersection_a, a->last,"\0");
		
			tmp2 = init_half_edge(a->last,intersection_a, "\0");
		
			a->last = intersection_a;
			a->twin->first = intersection_a;
		
			tmp1->twin = tmp2;
			tmp2->twin = tmp1;
		
			tmp1->prev = a;
			tmp2->next = a->twin;
		
			tmp1->next = a->next;
			tmp2->prev = a->twin->prev;
		
			tmp1->next->prev = tmp1;
			tmp2->prev->next = tmp2;
		
			a->next = tmp1;
			a->twin->prev = tmp2;
		
			tmp1->incident_face = a->incident_face;
			tmp2->incident_face = a->twin->incident_face;
		
			dcel_insert_half_edge(diagram, tmp1);
			dcel_insert_half_edge(diagram, tmp2);

			dcel_insert_vertex(diagram, intersection_a);
		} else {
			if (point_equals(a->first, intersection_a)) {
				a = a->prev;
				destroy_point(intersection_a);
			}
		}

		if (!(point_equals(b->first, intersection_b) ||
			point_equals(b->last, intersection_b))) {
		
			/** Arreglo b */
			tmp1 = init_half_edge(intersection_b, b->last,"\0");
		
			tmp2 = init_half_edge(b->last,intersection_b, "\0");
		
			b->last = intersection_b;
			b->twin->first = intersection_b;
		
			tmp1->twin = tmp2;
			tmp2->twin = tmp1;
		
			tmp1->prev = b;
			tmp2->next = b->twin;
		
			tmp1->next = b->next;
			tmp2->prev = b->twin->prev;
		
			tmp1->next->prev = tmp1;
			tmp2->prev->next = tmp2;
		
			b->next = tmp1;
			b->twin->prev = tmp2;
		
			tmp1->incident_face = b->incident_face;
			tmp2->incident_face = b->incident_face;
		
			dcel_insert_half_edge(diagram, tmp1);
			dcel_insert_half_edge(diagram, tmp2);

			dcel_insert_vertex(diagram, intersection_b);
		} else {
			if (point_equals(b->first, intersection_b)) {
				b = b->prev;
				destroy_point(intersection_b);
			}
		}

		/** Agrego la cara y las nuevas dos aristas. */
		tmp1 = init_half_edge(a->last, b->last, "\0");
		tmp2 = init_half_edge(b->last, a->last, "\0");
		
		tmp1->twin = tmp2;
		tmp2->twin = tmp1;
		
		tmp1->prev = a;
		tmp2->prev = b;
		
		tmp1->next = b->next;
		tmp2->next = a->next;
		
		tmp1->next->prev = tmp1;
		tmp2->next->prev = tmp2;

		a->next = tmp1;
		b->next = tmp2;
		
		dcel_insert_half_edge(diagram, tmp1);
		dcel_insert_half_edge(diagram, tmp2);
		
		char* name = (char*) malloc(sizeof(char)* STR_LENGTH);
		rand_str(name, STR_LENGTH);
		
		face* new_face;
		new_face = init_face((const char*) name, a, NULL);
		new_face->center = center;
		new_face->center->face = new_face; 

		tmp2->incident_face = b->incident_face;
		tmp1->incident_face = b->incident_face;

		b = tmp1;
		do {
			tmp1->incident_face = new_face;
			tmp1 = tmp1->next;
		} while(tmp1 != b);

		if (contain_vertex(a->incident_face, center)) {

			((face* )b->incident_face)->center = ((face* )a->incident_face)->center;

			((face* )b->incident_face)->center->face = b->incident_face;
			
			
			((face* )a->incident_face)->center = center;
			center->face = a->incident_face;
			
		} else {
			
			((face* )b->incident_face)->center = center;
			center->face = b->incident_face;
			
		}

		dcel_insert_face(diagram, new_face);

		return;
	}

	half_edge *tmp1, *tmp2;

	/** Si es la última cara a procesar, falta ese caso!! */

	if (((face*)a->twin->incident_face)->outer_component != NULL)
		push_back(voronoi->processing, a->twin->incident_face);

	if (point_equals(data->b->first, data->intersection_b)) {

		destroy_point(data->intersection_b);
		data->intersection_b = data->b->first;
		
		intersection_b = data->intersection_b;

		/** Parto la arista a en dos partes. */

		tmp1 = init_half_edge(a->first, intersection_a, "\0");
		tmp2 = init_half_edge(intersection_a, a->first, "\0");

		a->first = intersection_a;
		a->twin->last = intersection_a;

		tmp1->incident_face = a->incident_face;
		tmp2->incident_face = a->twin->incident_face;

		tmp1->twin = tmp2;
		tmp2->twin = tmp1;

		tmp1->next = a;
		tmp1->prev = a->prev;

		tmp2->next = a->twin->next;
		tmp2->prev = a->twin;

		a->prev = tmp1;
		a->twin->next = tmp2;

		tmp1->prev->next = tmp1;
		tmp2->next->prev = tmp2;

		/** Agrego la nueva intersección*/
		a = a->prev;
		tmp1 = init_half_edge(a->last, b->first, "\0");
		tmp2 = init_half_edge(b->first, a->last, "\0");

		tmp2->incident_face = a->incident_face;

		tmp1->twin = tmp2;
		tmp2->twin = tmp1;

		tmp1->next = b;
		tmp1->prev = a;

		tmp2->next = a->next;
		tmp2->prev = b->prev;

		a->next->prev = tmp2;
		a->next = tmp1;

		b->prev->next = tmp2;
		b->prev = tmp1;

		/** Borro la arista b por que ya no sirve. */
		tmp1->next = b->twin->next;
		tmp1->next->prev = tmp1;

		b->twin->prev->next = b->next;
		b->twin->prev->next->prev = b->twin->prev;

		destroy_half_edge(b->twin);
		destroy_half_edge(b);

		b = tmp1;

		do {
			tmp1->incident_face = center->face;
			tmp1 = tmp1->next;
		} while(tmp1 != b);

		return;

	} else {
		
		destroy_point(data->intersection_b);
		data->intersection_b = data->b->last;

		intersection_b = data->intersection_b;

		/** Parto la arista a en dos partes. */
		tmp1 = init_half_edge(intersection_a, a->last, "\0");
		tmp2 = init_half_edge(a->last, intersection_a, "\0");

		a->last = intersection_a;
		a->twin->first = intersection_a;

		tmp1->twin = tmp2;
		tmp2->twin = tmp1;

		tmp2->incident_face = a->twin->incident_face;
		tmp1->incident_face = a->incident_face;

		tmp1->next = a->next;
		tmp1->prev = a;

		tmp2->next = a->twin;
		tmp2->prev = a->twin->prev;

		a->next = tmp1;
		tmp1->next->prev = tmp1;

		a->twin->prev = tmp2;
		tmp2->next->prev = tmp2;


		/** Agrego la nueva intersección*/
		a = tmp1;

		tmp1 = init_half_edge(b->last, a->first, "\0");
		tmp2 = init_half_edge(a->first, b->last, "\0");

		tmp2->incident_face = tmp2->next->incident_face;

		tmp1->twin = tmp2;
		tmp2->twin = tmp1;

		tmp1->prev = b;
		tmp1->next = a;

		tmp2->prev = a->twin->next->twin;
		tmp2->next = b->twin->prev->twin->prev->twin;

		a->prev->next = tmp2;
		a->prev = tmp1;

		b->next->prev = tmp2,
		b->next = tmp1;

		/** Borro la arista b por que ya no sirve. */
		tmp1->prev = b->twin->prev;
		tmp1->prev->next = tmp1;

		b->twin->next->prev = b->prev;
		b->prev->next = b->twin->next;

		destroy_half_edge(b->twin);
		destroy_half_edge(b);

		b = tmp1;

		do {
			tmp1->incident_face = center->face;
			tmp1 = tmp1->next;
		} while(tmp1 != b);

		return;

	}

	

}
