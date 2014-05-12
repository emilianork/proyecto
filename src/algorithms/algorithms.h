/**
 * Author: José Emiliano Cabrera Blancas (jemiliano.cabrera@gmail.com)
 *
 * Description:
 * Funciones básicas para contruir un diagrama de Voronoi y después construir
 * la triangulación de Delauney.
 */

#ifndef ALGORITHMS_H_
#define ALGORITHMS_H_

#include "voronoi/voronoi.h"

#include <stdio.h>

/**
 * upgrade_data contiene toda la información que necesita la 
 * funcion upgrade_voronoi_diagram() para actualizar la DCEL.
 */
struct upgrade_data {
	
	/**
	 * center: Es la última semilla que se agregado a la lista seeds de
	 *         la estructura voronoi, dicho de otra forma, 
	 *         center = voronoi->seeds->tail->element;
	 */
	vertex* center;
	
	/**
	 * Cuando trazamos el bisector perpendicular de la cara que estamos procesando
	 * ocurrent 2 posibles casos.
	 *
	 *  1. Si la nueva semilla no pertenece a ningun centroide, entonces las
	 *     dos intersecciones del bisector perpendicular de la semilla y del
	 *     centro de la cara donde cae son puntos que no estan en la DCEL.
	 *  
	 *  2. Si la nueva semilla pertenece a una cara, entonces las dos 
	 *     intersecciones del bisector perpendicular del centroide de la cara
	 *     que se esta procesando y la nueva semilla se repiten una o dos veces
	 *     en la lista de vertices de la DCEL.
	 *
	 *      
	 * NOTA: intersection_a es un punto contenido en la arista a, lo mismo con 
	 *       la arista b y la intersecion_b.
	 */
	half_edge* a;
	vertex* intersection_a;
	
	half_edge* b;
	vertex* intersection_b;
	
	/**
	 * voronoi es el diagrama que se esta procesando.
	 */
	voronoi* voronoi;
	
	/**
	 * Es un booleano que nos indica si alguna de las dos interseciones de arriba
	 * ya existen en la DCEL.
	 *
	 * NOTA: Al menos una intersercion se repite si center ya tiene una
	 *       cara asignada
	 */
	int intersection_repeat;
};

/** Renombramos la estructura anterior para manejarla de forma mas facil. */
typedef struct upgrade_data up_data;

/** Funciones para inicalizar y destruir la estructura up_data. */
up_data* init_upgrade_data(void);
void destroy_upgrade_data(up_data* data);


/** Nos indica si existen pasos pendientes en el diagrama de voronoi. */
int steps_voronoi(voronoi* voronoi);

/**
 * Funcion principal que se llama cada vez que haya algun paso por procesarse.
 * Si se require seguir procesando las aristas encoladas en voronoi->processing,
 * entonces vertex es nulo. (Esta funcion la deben de implementar)
 **/
void voronoi_incremental(voronoi* voronoi, vertex* vertex);

/**
 * Cuando se hace merge de dos caras, puede que se tengan que unir
 * dos aristas en una sola.
 * half_edge->last es el vertice que se va a borrar en la dcel.
 */
void merge_half_edge(half_edge* half_edge, dcel* diagram);

/**
 * Cuando se tiene que crear una nueva cara apartir de dos interseciones.
 * Si alguna intersecion no se existe en la DCEL, tenemos que cortar
 * la nueva arista en dos aristas, dejando a 'a->next' como la nueva arista
 *
 */
void cut_half_edge(half_edge* a, vertex* intersection_a, dcel* diagram);

/**
 * Esta funcion agrega una nueva cara a la DCEL, creando una nueva arista
 * desde a->last a b->last.
 */
face* add_new_face(half_edge* a, half_edge* b, vertex* seed,
				   vertex* face_center, dcel* diagram);

/**
 * Esta funcion une dos caras, elminando a las aristas que son adyacentes
 * a ambas caras, les recomiendo eliminar new_face. (esta funcion la deben
 * de implementar. )
 */
void merge_faces(face* original_face, face* new_face, dcel* diagram);

/**
 * Funcion que agrega los cambios que se re requiran a la cara que se esta 
 * procesando.
 */
void upgrade_voronoi_diagram(up_data* data);

/**
 * Estas funciones procesan los puntos y escriben el resultado en el archivo
 * salida.txt
 */
void write_voronoi(voronoi* voronoi, FILE* fp);
voronoi* process_incremental(double width, double height, list* vertices);
#endif
