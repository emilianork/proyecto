/**
 * Author: José Emiliano Cabrera Blancas (jemiliano.cabrera@gmail.com)
 * 
 */

#include "algorithms/algorithms.h"

#include "types/types.h"

#include "points/2d_points.h"

#include "equations/equations.h"

#include <stdio.h>
#include <stdlib.h>


void merge_half_edge(half_edge* half_edge, dcel* diagram)
{
	struct half_edge *tmp1, *tmp2;
	tmp1 = half_edge->next;
	tmp2 = tmp1->twin;

	half_edge->next = tmp1->next;
	tmp1->next->prev = half_edge;

	half_edge->twin->prev = tmp2->prev;
	tmp2->prev->next = half_edge->twin;

	face* face_1 = half_edge->incident_face;
	face* face_2 = half_edge->twin->incident_face;
	

	if (face_1->outer_component != NULL)
		face_1->outer_component = half_edge;
	
	if (face_2->outer_component != NULL)
		face_2->outer_component = half_edge->twin;
	
	half_edge->last = tmp1->last;
	half_edge->twin->first = tmp2->first;

	half_edge->last->incident_edge = half_edge->next;

	dcel_pop_half_edge(diagram,tmp1);
	dcel_pop_half_edge(diagram,tmp2);

	dcel_pop_vertex(diagram,tmp1->first);

	destroy_point(tmp1->first);
	
	destroy_half_edge(tmp1);
	destroy_half_edge(tmp2);
	
}

void cut_half_edge(half_edge* a, vertex* intersection_a, dcel* diagram)
{
	half_edge *tmp1, *tmp2;

	tmp1 = init_half_edge(a->first, intersection_a, "\0");
	tmp2 = init_half_edge(intersection_a, a->first, "\0");
	
	a->first = intersection_a;
	a->twin->last = intersection_a;
	
	tmp1->next = a;
	tmp2->next = a->twin->next;
	
	tmp1->prev = a->prev;
	tmp2->prev = a->twin;
	
	tmp1->twin = tmp2;
	tmp2->twin = tmp1;
	
	tmp1->incident_face = a->incident_face;
	tmp2->incident_face = a->twin->incident_face;
	
	tmp1->prev->next = tmp1;
	tmp2->next->prev = tmp2;
	
	tmp2->prev->next = tmp2;
	tmp1->next->prev = tmp1;
	
	intersection_a->incident_edge = a;
	tmp1->first->incident_edge = tmp1;
	
	dcel_insert_half_edge(diagram, tmp1);
	dcel_insert_half_edge(diagram, tmp2);
	
	dcel_insert_vertex(diagram, intersection_a);
}

face* add_new_face(half_edge* a, half_edge* b, vertex* seed,
						vertex* face_center, dcel* diagram) 
{
	half_edge *tmp1, *tmp2;

	tmp1 = init_half_edge(b->last, a->last, "\0");
	tmp2 = init_half_edge(a->last, b->last, "\0");

	tmp1->twin = tmp2;
	tmp2->twin = tmp1;

	tmp1->next = a->next;
	tmp2->next = b->next;

	tmp1->prev = b;
	tmp2->prev = a;

	tmp1->incident_face = b->incident_face;
	tmp2->incident_face = a->incident_face;

	dcel_insert_half_edge(diagram, tmp1);
	dcel_insert_half_edge(diagram, tmp2);

	a->next->prev = tmp1;
	a->next = tmp2;

	b->next->prev = tmp2;
	b->next = tmp1;

	char* name = (char*) malloc(sizeof(char)*STR_LENGTH);
	rand_str(name, STR_LENGTH);

	face* new_face = init_face((const char*) name, b, NULL);
	
	do {
		tmp1->incident_face = new_face;
		tmp1 = tmp1->next;
	}  while(tmp1 != b->next);

	dcel_insert_face(diagram, new_face);
	
	face* aux_face = tmp2->incident_face;
	
	if (aux_face->outer_component != NULL)
		aux_face->outer_component = tmp2;
	
	new_face->outer_component = tmp1;

	/**
	 * Checo que cara contiene a face_center, por que
	 * este nunca va a ser nulo, seed si puede ser nulo.
	 */

	if (contain_vertex(new_face, face_center)) {
		
		new_face->center = face_center;
		face_center->face = new_face;

		if (seed != NULL) {
			((face*)tmp2->incident_face)->center = seed;
			seed->face = tmp2->incident_face;
		}

		return ((face*)tmp2->incident_face);
	} else {
		
		if (seed != NULL) {
			new_face->center = seed;
			seed->face = new_face;
		}
		return new_face;
	}
}

void merge_faces(face* original_face, face* new_face, dcel* diagram) 
{
	
}

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
	
}

void upgrade_voronoi_diagram(up_data* data)
{	
	
}


void write_voronoi(voronoi* voronoi, FILE* fp) {
	list* seeds = voronoi->seeds;

	fprintf(fp,"%s %d\n","Semillas:",seeds->size);
	
	item* tmp;
	for(tmp = seeds->head; tmp != NULL; tmp = tmp->right) {
		vertex* seed = tmp->element;
		fprintf(fp,"%f %f\n", seed->x, seed->y);
	}
	
	list* half_edges = voronoi->diagram->half_edge;
	
	fprintf(fp,"%s %d\n","Aristas:",half_edges->size);
	for(tmp = half_edges->head; tmp != NULL; tmp = tmp->right) {
		half_edge* he = tmp->element;
		fprintf(fp,"%f %f %f %f\n", he->first->x, he->first->y, he->last->x,
				he->last->y);
	}
}

voronoi* process_incremental(double width, double height, list* vertices) 
{
	if (vertices == NULL)
		return NULL;


	FILE * fp;

	fp = fopen ("salida.txt", "w");
	
	voronoi* voronoi = init_voronoi_diagram(width, height);
	
	write_voronoi(voronoi, fp);

	item* tmp;
	for(tmp = vertices->head; tmp != NULL; tmp = tmp->right) {
		vertex* vertex = tmp->element;
		
		voronoi_incremental(voronoi, vertex);
		write_voronoi(voronoi, fp);

		while(steps_voronoi(voronoi)) {
			voronoi_incremental(voronoi, NULL);

			write_voronoi(voronoi, fp);
		}
	}	

	fclose(fp);

	return voronoi;
}
