/**
 * Author: José Emiliano Cabrera Blancas (jemiliano.cabrera@gmail.com)
 *
 */

#include "dcel/dcel.h"
#include "double_linked_list/double_linked_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


dcel* init_dcel()
{
	dcel* dcel;
	dcel = (struct dcel*) malloc(sizeof(struct dcel));
	
	if (dcel == NULL) {
		printf("Ya no hay memoria disponible: init_dcel()\n");
		exit(EXIT_FAILURE);
	}

	dcel->face = NULL;
	dcel->vertex = NULL;
	dcel->half_edge = NULL;

	return dcel;
}

void destroy_dcel(dcel* dcel)
{
	destroy_rb_tree(dcel->face);
	destroy_rb_tree(dcel->vertex);
	destroy_rb_tree(dcel->half_edge);

	free(dcel);
}

void dcel_insert_face(dcel* dcel, face* face)
{
	if (face == NULL || dcel == NULL)
		return;

	if (dcel->face == NULL)
		dcel->face = init_rb_tree(FACE);

	if (rb_search(dcel->face,face) != NULL) {
		printf("dcel_insert_face(): Se repitio una cara\n");
		exit(EXIT_FAILURE);
	}
	
	rb_insert(dcel->face, face);
}

void dcel_insert_half_edge(dcel* dcel, half_edge* half_edge)
{
	if (half_edge == NULL || dcel == NULL)
		return;

	if (dcel->half_edge == NULL) {
		dcel->half_edge = init_rb_tree(HALF_EDGE);
	}

	if (rb_search(dcel->half_edge,half_edge) != NULL)
		return;

	rb_insert(dcel->half_edge, half_edge);
}

void dcel_insert_vertex(dcel* dcel, vertex* vertex)
{
	if (dcel == NULL || vertex == NULL)
		return;

	if (dcel->vertex == NULL)
		dcel->vertex = init_rb_tree(POINT);
	
	if (rb_search(dcel->vertex,vertex) != NULL)
		return;

	rb_insert(dcel->vertex,vertex);
}

list* incident_he_to_v(vertex* vertex)
{	
	return NULL;
}

list* incident_f_to_f(face* face) 
{
	rb_tree* incident_faces;
	incident_faces = init_rb_tree(FACE);

	if (face->outer_component != NULL) {
		
		half_edge* init_half_edge = face->outer_component;
		half_edge* tmp;
		
		rb_insert(incident_faces, init_half_edge->twin->incident_face);
		
		for(tmp = init_half_edge->next; tmp != init_half_edge; tmp = tmp->next) 
			if (rb_search(incident_faces, tmp->twin->incident_face) == NULL)
				rb_insert(incident_faces,tmp->twin->incident_face);
		
	}
	
	if (face->inner_components != NULL) {
		item* tmp1 = ((list*)face->inner_components)->head;
		half_edge *init_half_edge, *tmp2;
		
		
		for (;tmp1 != NULL; tmp1 = tmp1->right) {
			
			init_half_edge = tmp1->element;
			
			rb_insert(incident_faces, init_half_edge->twin->incident_face) ;
			
			for (tmp2 = init_half_edge->next; tmp2 != init_half_edge;
				 tmp2 = tmp2->next) {
				
				if (rb_search(incident_faces, tmp2->twin->incident_face) == NULL)
					rb_insert(incident_faces, tmp2->twin->incident_face);
				
			}
			
		}
	}
	
	return rb_tree_to_list(incident_faces);
}

list* incident_he_to_f(face* face)
{
	list* list = init_double_linked_list(HALF_EDGE);

	if (face->outer_component != NULL) {

		half_edge* init_half_edge = face->outer_component;
		half_edge* tmp;

		push_back(list,init_half_edge);

		for (tmp = init_half_edge->next; tmp != init_half_edge; tmp = tmp->next)
			push_back(list, tmp);

	}

	if (face->inner_components != NULL) {

		item* tmp1 = ((struct double_linked_list*) face->inner_components)->head;
		half_edge *init_half_edge, *tmp2;

		for (;tmp1 != NULL; tmp1 = tmp1->right) {

			init_half_edge = tmp1->element;

			push_back(list, init_half_edge);

			for (tmp2 = init_half_edge->next; tmp2 != init_half_edge;
				 tmp2 = tmp2->next) {

				push_back(list, tmp2);
			}
		}

	}

	return list;

}

int contain_vertex(face* face, vertex* vertex)
{
	list* incident_he = incident_he_to_f(face);
	
	/** Primero obtengo la dirección de la cara. */
	half_edge* tmp_he = incident_he->head->element;

	/** Direccion del interior de la cara para cada arista. */
	int direction;
	if (tmp_he->first->x == tmp_he->last->x) {
		if (tmp_he->first->y < tmp_he->last->y)
			direction = LEFT;
		else
			direction = RIGHT;
	} else {
		if (tmp_he->first->x < tmp_he->last->x)
			direction = LEFT;
		else
			direction = RIGHT;
	}
   

	item *tmp;

	for (tmp = incident_he->head; tmp != NULL; tmp = tmp->right) {
		
		tmp_he = tmp->element;
		/** 
		 * Si la vuelta no es hacia donde deberia, entonces la cara no
		 * contiene al vertice
		 */
		if (curve_orientation(tmp_he->first, tmp_he->last, vertex) != direction) {
			//return FALSE;
		}

	}
	
	return TRUE;
}

void rand_str(char *dest, int length)
{
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0) {
		rand();
        int index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}
