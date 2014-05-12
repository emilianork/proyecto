/**
 * Author: José Emiliano Cabrera Blancas (jemiliano.cabrera@gmail.com)
 *
 * Description:
 * El diagrama de Voronoi se representa con una DCEL y ademas se le agregaron
 * algunos datos que son importantes durante su construcción.
 */

#ifndef VORONOI_H_
#define VORONOI_H_

#include "dcel/dcel.h"
#include "double_linked_list/double_linked_list.h"

/**
 * Estructura que contiene, los datos necesarios para representar el diagrama
 * de voronoi y el estado en donde se encuentra.
 */
struct voronoi_diagram {
	/**
	 * DCEL que encierra en un rectangulo el diagrama de voronoi.
	 */
	struct dcel* diagram;

	/**
	 * Cola de caras que faltan por processar.
	 */
	struct double_linked_list* processing;

	/**
	 * Cola de semillas que generan al diagrama de voronoi.
	 */
	struct double_linked_list* seeds;

	/**
	 * Entero que nos indica si estamos procesando o no el caso degenerado.
	 */
	int degenerate_case;
};

/** Renombramos a la estructura para trabajar de forma mas facil. */
typedef struct voronoi_diagram voronoi;

/** 
 * Funciones para generar el diagrama de voronoi sin ninguna semilla
 * (solo generan el rectangulo que encirran al diagrama de voronoi). 
 */
voronoi* init_voronoi_diagram(double width, double height);
dcel* construct_bounded_box(double width, double height);

#endif
