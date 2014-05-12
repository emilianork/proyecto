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
	destroy_double_linked_list(dcel->face);
	destroy_double_linked_list(dcel->vertex);
	destroy_double_linked_list(dcel->half_edge);

	free(dcel);
}

void dcel_insert_face(dcel* dcel, face* face)
{
	if (face == NULL || dcel == NULL)
		return;

	if (dcel->face == NULL)
		dcel->face = init_double_linked_list(FACE);

	push_back(dcel->face, face);
}

void dcel_insert_half_edge(dcel* dcel, half_edge* half_edge)
{
	if (half_edge == NULL || dcel == NULL)
		return;

	if (dcel->half_edge == NULL) {
		dcel->half_edge = init_double_linked_list(HALF_EDGE);
	}

	push_back(dcel->half_edge, half_edge);
}

void dcel_insert_vertex(dcel* dcel, vertex* vertex)
{
	if (dcel == NULL || vertex == NULL)
		return;

	if (dcel->vertex == NULL)
		dcel->vertex = init_double_linked_list(POINT);
	
	push_back(dcel->vertex,vertex);
}

void dcel_pop_vertex(dcel* dcel, vertex* vertex)
{
	if (dcel == NULL || vertex == NULL)
		return;

	if (dcel->vertex == NULL)
		return;

	item* tmp;
	for(tmp = dcel->vertex->head; tmp != NULL; tmp = tmp->right) {
		if (vertex == tmp->element) {
			/** Si borro el unico elmento. */
			if (dcel->vertex->size == 1) {
				dcel->vertex->head = NULL;
				dcel->vertex->tail = NULL;
				dcel->vertex->size = 0;

				return;
			}

			/** Si borro la cabeza*/
			if (tmp == dcel->vertex->head) {

				dcel->vertex->head = tmp->right;				
				dcel->vertex->head->left = NULL;

				dcel->vertex->size -= 1;

				return;
			}
				
			/** Si borro el ultimo element*/
			if (tmp == dcel->vertex->tail) {
				
				dcel->vertex->tail = tmp->left;
				dcel->vertex->tail->right = NULL;

				dcel->vertex->size -= 1;

				return;
			}

			tmp->left->right = tmp->right;
			tmp->right->left = tmp->left;

			dcel->vertex->size -= 1;

			return;
		}
	}

	return;
	
	printf("No se encontro el vertice en la DCEL: (%f,%f)\n", vertex->x,
		   vertex->y);

	printf("Vertices: \n");
	list* points = dcel->vertex;
	item* tmps;

	
	printf("Size: %d\n", points->size);

	for(tmps = points->head; tmps != NULL; tmps = tmps->right) {
		struct point *temp = tmps->element;
		
		printf("(%f,%f)\n", temp->x, temp->y);
		
	}
	
	
	exit(EXIT_FAILURE);
}

void dcel_pop_half_edge(dcel* dcel,half_edge* half_edge)
{
	if (dcel == NULL || half_edge == NULL)
		return;

	if (dcel->half_edge == NULL)
		return;

	item* tmp;
	for(tmp = dcel->half_edge->head; tmp != NULL; tmp = tmp->right) {
		if (half_edge == tmp->element) {
			/** Si borro el unico elmento. */
			if (dcel->half_edge->size == 1) {
				dcel->half_edge->head = NULL;
				dcel->half_edge->tail = NULL;
				dcel->half_edge->size = 0;

				return;
			}

			/** Si borro la cabeza*/
			if (tmp == dcel->half_edge->head) {

				dcel->half_edge->head = tmp->right;				
				dcel->half_edge->head->left = NULL;

				dcel->half_edge->size -= 1;

				return;
			}
				
			/** Si borro el ultimo element*/
			if (tmp == dcel->half_edge->tail) {
				
				dcel->half_edge->tail = tmp->left;
				dcel->half_edge->tail->right = NULL;

				dcel->half_edge->size -= 1;

				return;
			}

			tmp->left->right = tmp->right;
			tmp->right->left = tmp->left;

			dcel->half_edge->size -= 1;

			return;
		}
	}

	printf("No se encontro la arista en la DCEL: (%f,%f) (%f,%f) \n", 
		   half_edge->first->x, half_edge->first->y, half_edge->last->x,
		   half_edge->last->y);
	exit(EXIT_FAILURE);


}

void dcel_pop_face(dcel* dcel, face* face)
{
	if (dcel == NULL || face == NULL)
		return;

	if (dcel->face == NULL)
		return;

	item* tmp;
	for(tmp = dcel->face->head; tmp != NULL; tmp = tmp->right) {
		if (face == tmp->element) {
			/** Si borro el unico elmento. */
			if (dcel->face->size == 1) {
				dcel->face->head = NULL;
				dcel->face->tail = NULL;
				dcel->face->size = 0;

				return;
			}

			/** Si borro la cabeza*/
			if (tmp == dcel->face->head) {

				dcel->face->head = tmp->right;				
				dcel->face->head->left = NULL;

				dcel->face->size -= 1;

				return;
			}
				
			/** Si borro el ultimo element*/
			if (tmp == dcel->face->tail) {
				
				dcel->face->tail = tmp->left;
				dcel->face->tail->right = NULL;

				dcel->face->size -= 1;

				return;
			}

			tmp->left->right = tmp->right;
			tmp->right->left = tmp->left;

			dcel->face->size -= 1;

			return;
		}
	}

	printf("No se encontro la cara en la DCEL: %s \n", face->name);
	exit(EXIT_FAILURE);
	
}


list* incident_he_to_v(vertex* vertex)
{	
	list* list = init_double_linked_list(HALF_EDGE);

	push_back(list, vertex->incident_edge);

	half_edge* tmp;
	tmp = vertex->incident_edge;
	tmp = tmp->twin->next;

	for(; tmp != list->head->element; tmp = tmp->twin->next)
		push_back(list,tmp);

	return list;
}

list* incident_f_to_f(face* face) 
{
	return NULL;
}

list* incident_he_to_f(face* face)
{
	list* list = init_double_linked_list(HALF_EDGE);

	int count = 0;

	if (face->outer_component != NULL) {

		half_edge* init_half_edge = face->outer_component;
		half_edge* tmp;

		push_back(list,init_half_edge);

		for (tmp = init_half_edge->next; tmp != init_half_edge; 
			 tmp = tmp->next) {

			push_back(list, tmp);

			if (++count == 1000) {
				printf("incident_he_to_f(): Entro en loop\n");
				exit(EXIT_FAILURE);
			}

		}

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

	printf("Contain_vertex(), (%f,%f)\n", vertex->x, vertex->y);
	
	list* incident_he = incident_he_to_f(face);

	int direction = LEFT;

	item *tmp;

	
	for (tmp = incident_he->head; tmp != NULL; tmp = tmp->right) {
		

		half_edge* tmp_he = tmp->element;
		

		printf("Arista: (%f,%f), (%f,%f)\n", tmp_he->first->x, tmp_he->first->y,
			   tmp_he->last->x, tmp_he->last->y);

		

		if (curve_orientation(tmp_he->first, tmp_he->last, vertex) != LEFT) {
			direction = RIGHT;
			break;
		}

	}

	if (direction == LEFT)
		return TRUE;

	for (tmp = incident_he->head; tmp != NULL; tmp = tmp->right) {
		
		half_edge* tmp_he = tmp->element;
		
		if (curve_orientation(tmp_he->first, tmp_he->last, vertex) != RIGHT) {
			direction = LEFT;
			break;
		}
	}
	
	if (direction == RIGHT)
		return TRUE;
	
	return FALSE;
}

vertex* search_vertex(dcel* dcel, vertex* vertex)
{
	if (dcel->vertex == NULL)
		return NULL;

	item* tmp;
	struct point* tmp_v;
	for(tmp = dcel->vertex->head; tmp != NULL; tmp = tmp->right) {
		
		tmp_v = tmp->element;
		
		if (point_equals(tmp_v, vertex))
			return tmp_v;
	}

	return NULL;
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
