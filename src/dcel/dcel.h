/**
 * Author: José Emiliano Cabrera Blancas (jemiliano.cabrera@gmail.com)
 *
 * Description:
 * Implementación de la DCEL, con la diferencia de que en lugar de usar una 
 * lista doublemente ligada para las caras, otra para los vertices y otro más
 * para las aristas, se utilizan arboles rojo negro.
 * 
 */

#ifndef DCEL_H_
#define DCEL_H_

#include "double_linked_list/double_linked_list.h"

#include "face/face.h"
#include "half_edge/half_edge.h"
#include "points/2d_points.h"

#define STR_LENGTH 7

/**
 * Estructura DCEL, para guardar las caras, vertices y aristas
 */
struct dcel {
	/** 
	 * Registro de las caras, vertices y aristas guardadas en listas.
	*/
	list* face;
	list* vertex;
	list* half_edge;
};

/** Renombramos la DCEL para usarla de forma más sencilla. */
typedef struct dcel dcel;

/** Funcion para inicializar para alojar la memoria necesaria para la DCEL. */
dcel* init_dcel(void);

/**
 * Funcion para destruir la dcel sin eliminar los elementos dentro de ella.
 * Solo destruye los arboles que utiliza.
*/
void destroy_dcel(dcel* dcel);

/**
 * Inserta una cara a la DCEL, esta funcion maneja por dentro los arboles rojo
 * negro.
 */
void dcel_insert_face(dcel* dcel, face* face);

/**
 * Inserta una arista a la DCEL, esta funcion maneja por dentro los arboles rojo
 * negro.
 */
void dcel_insert_half_edge(dcel* dcel, half_edge* half_edge);

/**
 * Inserta un vertice a la DCEL, esta funcion maneja por dentro los arboles rojo
 * negro.
 */
void dcel_insert_vertex(dcel* dcel, vertex* vertex);

/**
 * Borra un vertice de la DCEL, esta funcion maneja por dentro los arboles rojo
 * negro.
 */
void dcel_pop_vertex(dcel* dcel, vertex* vertex);

/**
 * Borra una arista de la DCEL, esta funcion maneja por dentro los arboles rojo
 * negro.
 */
void dcel_pop_half_edge(dcel* dcel,half_edge* half_edge);

/**
 * Borra una cara de la DCEL, esta funcion maneja por dentro los arboles rojo
 * negro.
 */
void dcel_pop_face(dcel* dcel, face* face);


/**
 * Funciones que debes de implementar.
 *
 * incident_he_to_v() debe regresar una lista de half_edge que tengan como origen * el vertice dado.
 */
list* incident_he_to_v(vertex* vertex);

/**
 * incident_f_to_f() debe regresar una lista de face que tengan como origen la
 * cara que se le pase. Puedes utilizar la funcion rb_tree_to_list(), 
 * recuerda que el arbol rojo negro te puede ayudar para saber si una cara ya
 * esta dentro del del arbol rojo negro.
 */
list* incident_f_to_f(face* face);

/**
 * incident_he_to_f() debe de regresa todos las aristas que son incidentes a 
 * la cara dada, recuerda que debes iterar la lista inner_components.
 */
list* incident_he_to_f(face* face);

/**
 * Dado que una cara del diagrama de voronoi es convexa, podemos decir en tiempo
 * lineal si el punto que se le pase esta contenido en la cara que se le pase.
 */
int contain_vertex(face* face, vertex* vertex);

/**
 * Buscamos en la DCEL si un vertice dado esta contenido o no en la DCEL.
 */
vertex* search_vertex(dcel* dcel, vertex* vertex);

/**
 * Funcion que genera nombres aleatorio para las caras, puesto que su unico
 * unico identificador es este.
 */
void rand_str(char *dest, int length);

#endif
