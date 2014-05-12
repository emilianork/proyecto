#include "algorithms/algorithms.h"

#include "red_black_tree/rb_tree.h"
#include "types/types.h"

#include "points/2d_points.h"

#include "equations/equations.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void cut_vertex(half_edge* half_edge, dcel* diagram)
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

/** Ambos puntos que debo unir son a->last y b->last. */
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
	
	if (original_face->outer_component == NULL) {
		printf("merge_faces: original_face es la cara exterior\n");
		exit(EXIT_FAILURE);
	}

	if (new_face->outer_component == NULL) {
		printf("merge_faces: new_face es la cara exterior\n");
		exit(EXIT_FAILURE);
	}

	list* ori_he = incident_he_to_f(original_face);
	list* new_he = incident_he_to_f(new_face);

	item* temporal;

	printf("Original_Face:\n");

	for (temporal = ori_he->head; temporal != NULL; temporal = temporal->right) {
		half_edge* tmp_he = temporal->element;

		printf("(%f,%f) (%f,%f)\n", tmp_he->first->x, tmp_he->first->y,
			   tmp_he->last->x, tmp_he->last->y);
	}
	
	printf("New_Face:\n");
	for (temporal = new_he->head; temporal != NULL; temporal = temporal->right) {
		half_edge* tmp_he = temporal->element;

		printf("(%f,%f) (%f,%f)\n", tmp_he->first->x, tmp_he->first->y,
			   tmp_he->last->x, tmp_he->last->y);
	}


	list* incident_he = incident_he_to_f(original_face);
	item* tmp_list;
	
	/**
	 *  original_face->outer_component no debe ser una arista que 
	 *   comparta adyacencia con new_face. 
	 */
	
	for(tmp_list = incident_he->head; tmp_list != NULL; 
		tmp_list = tmp_list->right) {

		half_edge* tmp_he = tmp_list->element;

		if (tmp_he->twin->incident_face != new_face) {
			original_face->outer_component = tmp_he;
			break;
		}
	}

	/**
	 * Ahora que ya compusimos el apuntador outer_component buscamos la lista
	 * de half_edge para eliminar.
	 */
	
	list* to_destroy = init_double_linked_list(HALF_EDGE);
	

	for(tmp_list = incident_he->head; tmp_list != NULL; 
		tmp_list = tmp_list->right) {
		
		half_edge* tmp_he = tmp_list->element;
		
		if (tmp_he->twin->incident_face == new_face) {
			
			push_back(to_destroy, tmp_he);

		} else {

			if (to_destroy->size >= 1)
				break;
		}
	}

	half_edge *tmp1, *tmp2, *tmp3, *tmp4;

	
	tmp1 = to_destroy->head->element;
	tmp2 = to_destroy->tail->element;

	tmp3 = tmp1->prev;
	tmp4 = tmp2->twin->prev;

	tmp3->next = tmp1->twin->next;
	tmp3->next->prev = tmp3;

	tmp4->next = tmp2->next;
	tmp4->next->prev = tmp4;

	tmp4->last->incident_edge = tmp4->next;
	tmp3->last->incident_edge = tmp3->next;

	((face*)tmp4->incident_face)->outer_component = tmp4;

	/** Primero borro los puntos que ya no vamos a usar.*/

	for(tmp_list = to_destroy->head; tmp_list != NULL; 
		tmp_list = tmp_list->right) {
		
		half_edge *tmp_he = tmp_list->element;

		vertex *tmp_first = tmp_he->first;
		vertex *tmp_last = tmp_he->last;

		if (!point_equals(tmp_first, tmp4->last) &&
			!point_equals(tmp_first, tmp3->last)) {
			
			//dcel_pop_vertex(diagram,tmp_first);

			//destroy_point(tmp_first);

		}

		if (!point_equals(tmp_last, tmp4->last) &&
			!point_equals(tmp_last, tmp3->last)) {
			
			//dcel_pop_vertex(diagram,tmp_last);

			//destroy_point(tmp_last);
		}
	}
	
	
	/** Elimino las aristas. */
	for(tmp_list = to_destroy->head; tmp_list != NULL; 
		tmp_list = tmp_list->right) {
		
		half_edge* tmp_he = tmp_list->element;

		dcel_pop_half_edge(diagram, tmp_he->twin);
		//destroy_half_edge(tmp_he->twin);
		

		dcel_pop_half_edge(diagram, tmp_he);
		//destroy_half_edge(tmp_he);
		
	}

	tmp1 = original_face->outer_component;

	do {

		tmp1->incident_face = original_face;
		tmp1 = tmp1->next;

	} while(tmp1 != original_face->outer_component);

	dcel_pop_face(diagram,new_face);

	destroy_face(new_face);


	/**
	 * Elimino los vertices que ya no son necesarios.
	 */

	vertex* first = tmp3->first;
	vertex* second = tmp3->last;
	vertex* third = tmp3->next->last;

	if ((point_equals_x(first,second) && point_equals_x(first,third)) ||
		(point_equals_y(first,second) && point_equals_y(first,third))) {
		
		cut_vertex(tmp3, diagram);
	}

	first = tmp4->first;
	second = tmp4->last;
	third = tmp4->next->last;

	if ((point_equals_x(first,second) && point_equals_x(first,third)) ||
		(point_equals_y(first,second) && point_equals_y(first,third))) {
		
		cut_vertex(tmp4, diagram);
	}
	

}

void erase_half_edge(half_edge* a, dcel* diagram) 
{
	vertex* new_seed = ((face*)a->incident_face)->center;
	face* erase_face = a->incident_face;

	/**
	 * Compongo apuntadores de las aristas que voy a borrar.
	 */
	a->prev->next = a->twin->next;
	a->twin->next->prev = a->prev;

	a->first->incident_edge = a->prev->next;
	a->last->incident_edge = a->next;

	a->next->prev = a->twin->prev;
	a->twin->prev->next = a->next;


	if (erase_face->outer_component != NULL)
		erase_face->outer_component = a->prev;

	dcel_pop_half_edge(diagram,a->twin);
	dcel_pop_half_edge(diagram,a);

	list* incident_he = incident_he_to_v(a->first);

	if (incident_he->size <= 1) {
		/**
		 * Si tiene solo una arista incidente entonces hay que borrar
		 * el punto a->first, junto con su arista(tambien la gemela).
		 */

		half_edge *tmp1, *tmp2;
		tmp1 = a->prev->next->next;
		tmp2 = a->twin->next->next->prev;

		half_edge *tmp3, *tmp4;
		tmp3 = a->prev;
		tmp4 = a->prev->twin;

		half_edge *tmp5, *tmp6;
		tmp5 = a->twin->next;
		tmp6 = a->twin->next->twin;

		tmp3->last = tmp1->first;
		tmp4->first = tmp2->last;

		tmp2->last->incident_edge = tmp4;

		tmp1->prev = tmp3;
		tmp3->next = tmp1;

		tmp4->prev = tmp2;
		tmp2->next = tmp4;

		dcel_pop_half_edge(diagram, tmp5);
		dcel_pop_half_edge(diagram, tmp6);

		//destroy_half_edge(tmp5);
		//destroy_half_edge(tmp6);
		
	}

	destroy_double_linked_list(incident_he);

	incident_he = incident_he_to_v(a->last);
	if (incident_he->size <= 1) {
		/**
		 * Si tiene solo una arista incidente entonces hay que borrar
		 * el punto a->last, junto con su arista(tambien la gemela).
		 */
		
		half_edge *tmp1, *tmp2;
		tmp1 = a->next;
		tmp2 = a->next->twin;

		half_edge *tmp3, *tmp4;
		tmp3 = a->twin->prev;
		tmp4 = a->twin->prev->twin;

		half_edge *tmp5, *tmp6;
		tmp5 = a->twin->prev->prev;
		tmp6 = a->twin->prev->prev->twin;

		tmp1->first = tmp5->last;
		tmp2->last = tmp6->first;

		tmp2->last->incident_edge = tmp6;

		tmp1->prev = tmp5;
		tmp2->next = tmp6;

		tmp5->next = tmp1;
		tmp6->prev = tmp2;

		dcel_pop_half_edge(diagram, tmp3);
		dcel_pop_half_edge(diagram, tmp4);

		//destroy_half_edge(tmp3);
		//destroy_half_edge(tmp4);
		
	}

	destroy_double_linked_list(incident_he);

	/**
	 * Asigno la cara adyacente a->twin
	 */
	half_edge *tmp1 = erase_face->outer_component;

	if (tmp1 == a) {
		tmp1 = a->prev;
	}
	
	face* replace_face = a->twin->incident_face;
	
	if (replace_face->outer_component != NULL)
		replace_face->outer_component = tmp1;

	do {
		tmp1->incident_face = replace_face;
		tmp1 = tmp1->next;
	} while(tmp1 != erase_face->outer_component);

	dcel_pop_face(diagram, erase_face);

	destroy_face(erase_face);
	//destroy_half_edge(a->twin);
	//destroy_half_edge(a);
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
	if (voronoi == NULL)
		return;
	
	/**
	 * 1. Si el vertex es distinto de NULL. 
	 */
    if (vertex != NULL) {
		
		/**
		 * 1.1. Veo la cara donde cae y la encolo en voronoi->processing.
		 * 1.2. Agrego al vertice a la lista voronoi->seeds.
		 */

		/** Obtengo la lista de caras. */
		list* faces = voronoi->diagram->face;
		
		face* face = NULL;
		item *tmp;

		/** Itero las caras hasta dar con la que contiene al punto vertex. */
		for (tmp = faces->head; tmp != NULL; tmp = tmp->right) {
			
			/** Descarto la cara exterior de la caja. */
			if ( ((struct face*)tmp->element)->outer_component !=  NULL) {

				if (contain_vertex(tmp->element, vertex)) {
					face = tmp->element;
					break;
				}
				
			}
		}

		if (face == NULL) {
			exit(EXIT_FAILURE);
		}

		vertex->distinct_color = TRUE;				
		
		push_back(voronoi->processing, face);
		push_back(voronoi->seeds, vertex);		

		return;
		
	} 

	/**
	 * 2. Si la lista voronoi->processing no esta vacia.
	 */
	
	if (steps_voronoi(voronoi)) {
		
		struct point* seed = voronoi->seeds->tail->element;
		
		face* face = pop_front(voronoi->processing);

		printf("\nSeed nuevo: (%f,%f)\n", seed->x, seed->y);

		printf("CARA QUE SE ESTA PROCESANDO:\n");

		list* half_edges = incident_he_to_f(face);
		
		item* tmp_list;
		
		for(tmp_list = half_edges->head; tmp_list != NULL; 
			tmp_list = tmp_list->right) {
		

			half_edge* he = tmp_list->element;
			
			printf("(%f,%f),(%f,%f)\n",he->first->x, he->first->y, he->last->x,
				   he->last->y);
			
		}

		printf("TERMINE\n");
		
		/**
		 * 2.1 Si el cojunto de semillas tiene tamaño 1.
		 */
		if (voronoi->seeds->size == 1) {
			
			/**
			 * 2.1.1 A la única semilla la pongo como centro de la
			 *       cara interior de la DCEL.
			 */
			
			face->center = seed;			
			seed->face = face;
			seed->distinct_color = FALSE;

		} else {

			/**
			 * 2.2 Proceso la cara de la siguiente forma:
			 */
			list* incident_half_edge = incident_he_to_f(face);
			line* bisector = perpendicular_bisector(face->center, seed);
			
			/**
			 * 2.2.1 Obtengo las dos aristas de intersección, junto con su
			 *       interseción.
			 */
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

						he_incident_2 = tmp->element;
						intersection_2 = intersection;
						
					}
				}
			}
			
			/**
			 * 2.2.2 Si la cara que estoy procesando contiene a
			 *       la semilla que estoy procesando, entonces las
			 *       dos interseciones van a ser nuevas.
			 */

			up_data* data = init_upgrade_data();
			
			data->center = seed;
			
			data->a = he_incident_1;
			data->intersection_a = intersection_1;
			
			data->b = he_incident_2;
			data->intersection_b = intersection_2;

			data->voronoi = voronoi;


			/**
			 * 2.2.3 Si no la contiene, entonces al menos una
			 *       interseción se repite.
			 */
			
			if (!contain_vertex(face, seed)) { 
				data->intersection_repeat = TRUE;
				
				printf("LA CARA DEBE TENER REPETICIONES:\n");
				
			}
			
			
			
			/**
			 * 2.2.4 Actualizo la DCEL.
			 */
			add_half_edge_voronoi(data);
			
			destroy_upgrade_data(data);
		}
		
		/**
		 * 2.3 Si la lista voronoi->processing queda vacía, entonces 
		 *     marco al ultimo punto de voronoi->seeds como color no
		 *     distinto.
		 */
		if (!steps_voronoi(voronoi)) {
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

	/**
	 * 1. Si no contiene intersecciónes que se repitan.
	 *    (Es la primera cara que se procesa.)
	 */

	if (!data->intersection_repeat) {
		/**
		 * 1.1 Si el punto de intersecion no empieza
		 *     o termina en a.
		 */
		
		if (!(point_equals(a->first, intersection_a) ||
			  point_equals(a->last, intersection_a))) {
			

			/**
			 * 1.1 Parto la arista a en dos aristas.
			 *     Ahora el punto de intersecion sera
			 *     a->last
			 */
			
			cut_half_edge(a,intersection_a, diagram);
			a = a->prev;

		} else {
			
			/**
			 * 1.2 Si el punto de intersecion empieza
			 *     o termina en a, destruyo el punto de intersecion_a.
			 */
			
			if (point_equals(a->first, intersection_a))
				a = a->prev;
			
			//destroy_point(intersection_a);
			
		}

		if (!(point_equals(b->first, intersection_b) ||
			  point_equals(b->last, intersection_b))) {

			/**
			 * 1.3 Parto la arista b en dos aristas. Ahora el punto de
			 * intersecion sera b->last (código identico 
			 * al de arriba)
			 */
			
			cut_half_edge(b,intersection_b, diagram);
			b = b->prev;
			
		} else {
			if (point_equals(b->first, intersection_b))
				b = b->prev;
			
			//destroy_point(intersection_b);

		}

		
		/**
		 * 1.4 Agrego una nueva cara junto con sus dos nuevas aristas.
		 */
		
		face* current_face = b->incident_face;
		vertex* seed = voronoi->seeds->tail->element;
		face* new_face = add_new_face(a,b,seed,current_face->center,diagram);

		/**
		 * 1.5 Si la nueva cara contiene al menos una arista disinta a la que
		 *     cree y que no sea adyacente a la cara exterior.
		 *		 
		 *    Tambien la nueva arista debe de ser adyacente por ambos lados
		 *    con la cara exterior.
		 */
		
		face *tmp1, *tmp2;
		tmp1 = new_face->outer_component->prev->twin->incident_face;
		tmp2 = new_face->outer_component->next->twin->incident_face;

		int count = 0;
		half_edge* non_adyacent_to_fe;

		if (tmp1->outer_component != NULL && tmp2->outer_component != NULL) {
		
		
			list* half_edges = incident_he_to_f(new_face);
			
			item* tmp;
			for (tmp = half_edges->head; tmp != NULL; tmp = tmp->right) {
				half_edge* tmp_he;
				tmp_he = tmp->element;
				
				tmp_he = tmp_he->twin;
				
				if (((face*) tmp_he->incident_face)->outer_component != NULL &&
					new_face->outer_component != tmp_he->twin ) {
					
					count++;
					non_adyacent_to_fe = tmp_he;
				}
			}
		}

		/**
		 * 1.5.1 Elimino a la arista y encolo a la cara.
		 */

		if (count == 1 && !voronoi->degenerate_case) {
			push_back(voronoi->processing, 
					  non_adyacent_to_fe->incident_face);

			vertex* ncenter = ((face*)non_adyacent_to_fe->incident_face)->center;

			erase_half_edge(non_adyacent_to_fe, diagram);
			
			new_face->center = ncenter;
			ncenter->face = new_face;
			
			voronoi->degenerate_case = TRUE;
		} else {
			
			/**
			 * 1.6 En otro caso, encolo a las caras que no sean las exteriores
			 * y son adyacentes a la nueva arista que cree.
			 */
			
			voronoi->degenerate_case = FALSE;

			if (tmp1->outer_component != NULL) 
				push_back(voronoi->processing,tmp1);

			if (tmp2->outer_component != NULL) 
				push_back(voronoi->processing,tmp2);
		}
		
		return;
			
	} else {
		
		/**
		 * 2. Al menos una interseción se repite.
		 **/

		/**
		 * 2. (Primero debo saber que puntos se intersectan, y buscarlos en la
		 *    lista de vertices.
		 */

		int count = 0;
		int a_repeat = FALSE;
		int b_repeat = FALSE;

		vertex* tmp_v = search_vertex(diagram,intersection_a);

		if (tmp_v != NULL) {
			
			//destroy_point(intersection_a);
			intersection_a = tmp_v;
			a_repeat = TRUE;
			count++;
		}

		tmp_v = search_vertex(diagram,intersection_b);
		
		if (tmp_v != NULL) {
			//destroy_point(intersection_b);
			intersection_b = tmp_v;
			b_repeat = TRUE;
			count++;
		}

		if (count == 0) {
			printf("No hubo intersecciones repetidas, eso no debio ser.\n");
			
			printf("Intersection a: (%f,%f)\n", intersection_a->x,
				   intersection_a->y);

			printf("Intersection b: (%f,%f)\n", intersection_b->x,
				   intersection_b->y);


			printf("LISTA DE VERTICES:\n");
			item* temp;

			for (temp = diagram->vertex->head; temp != NULL; 
				 temp = temp->right) {
				
				vertex* tmp_ve = temp->element;
				
				printf("(%f,%f)\n", tmp_ve->x, tmp_ve->y);
				
			}
			
				   
			exit(EXIT_FAILURE);
		}
		
		printf("a: (%f,%f),(%f,%f)\n", a->first->x, a->first->y, a->last->x,
			   a->last->y);

		printf("b: (%f,%f),(%f,%f)\n", b->first->x, b->first->y, b->last->x,
			   b->last->y);

		/**
		 * 2. (Parto en dos las aristas que contiene la intersecion que no
		 *     se repite).
		 */

		face* next_face_to_process = NULL;

		if (!a_repeat) {
			next_face_to_process = a->twin->incident_face;
			
			cut_half_edge(a,intersection_a, diagram);
			a = a->prev;
			
		} else {
			if (point_equals(a->first, intersection_a)) {
				a = a->prev;

			}
			
			//destroy_point(intersection_a);
			
		}

		if (!b_repeat) {

			next_face_to_process = b->twin->incident_face;
			
			cut_half_edge(b,intersection_b, diagram);
			b = b->prev;
		} else {
			if (point_equals(b->first, intersection_b))
				b = b->prev;
			
			//destroy_point(intersection_b);

		}

		printf("a: (%f,%f),(%f,%f)\n", a->first->x, a->first->y, a->last->x,
			   a->last->y);

		printf("b: (%f,%f),(%f,%f)\n", b->first->x, b->first->y, b->last->x,
			   b->last->y);

		
		/**
		 * 2.1 Creo una nueva cara y se las asigno a las nuevas aristas.
		 */
		vertex* face_center = ((face*)a->incident_face)->center;
		
		face* new_face = add_new_face(a,b,NULL,face_center, diagram);

		/**
		 * 2.2 Elimino las aristas que tengan las dos caras adyacentes.
		 *     center, new_face.
		 */

		merge_faces(center->face, new_face, diagram);
		

		list* cara_resultante = incident_he_to_f(center->face);
		
		item* temporal;

		printf("Merge_Face:\n");

		for (temporal = cara_resultante->head; temporal != NULL; 
			 temporal = temporal->right) {
			half_edge* tmp_he = temporal->element;
			
			printf("(%f,%f) (%f,%f)\n", tmp_he->first->x, tmp_he->first->y,
				   tmp_he->last->x, tmp_he->last->y);
		}

		/**
		 * 2.3 Encolo las caras adyacentes a la interseccion que no se
		 *     repita.
		 */

		if (count == 1) {

			printf("Me preparo para agregar nueva cara:\n");
			
			if (next_face_to_process == NULL) {
				printf("La cara nueva que se iba a procesar es nula\n.");
				exit(EXIT_FAILURE);
			}
			
			
			if (next_face_to_process->outer_component == NULL) {
				
				list* faces = voronoi->diagram->face;

				item* tmps;

				/**printf("\n\nCaras finales\n");

				for(tmps = faces->head; tmps != NULL; tmps = tmps->right) {
					printf("\nCara:\n");
					
					item* tmp_a;
					
					list* aristas = incident_he_to_f(tmps->element);
					
					for (tmp_a = aristas->head; tmp_a != NULL; 
						 tmp_a = tmp_a->right) {
						
						half_edge* tmp_he = tmp_a->element;
						
						printf("(%f,%f) (%f,%f)\n", tmp_he->first->x,
							   tmp_he->first->y, tmp_he->last->x, 
							   tmp_he->last->y);
					}
					
				}
				*/
				
				/**printf("\n\nAristas Finales\n");
				
				for(tmps = voronoi->diagram->half_edge->head;
					tmps != NULL; tmps = tmps->right) {
					
					half_edge* tmp_he = tmps->element;
					
					printf("(%f,%f) (%f,%f)\n", tmp_he->first->x,
						   tmp_he->first->y, tmp_he->last->x, 
						   tmp_he->last->y);
					
					
				}
				*/
			}
			
			printf("Lista de Caras por procesar: %d\n"
				   ,voronoi->processing->size);

			if (next_face_to_process->outer_component != NULL) {
				
				half_edge* tmp_he = next_face_to_process->outer_component;

				printf("Outer_component: (%f,%f), (%f,%f)\n", tmp_he->first->x,
					   tmp_he->first->y, tmp_he->last->x, tmp_he->last->y);

				push_back(voronoi->processing,next_face_to_process);
			}
		}
		
		return;	
	}
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
		return;


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
