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

struct upgrade_data {
	
	/**Cambiar nombre a seed_processing*/
	
	vertex* center;
	
	half_edge* a;
	vertex* intersection_a;
	
	half_edge* b;
	vertex* intersection_b;
	
	voronoi* voronoi;
	
	int intersection_repeat;
};

typedef struct upgrade_data up_data;

up_data* init_upgrade_data(void);
void destroy_upgrade_data(up_data* data);

int steps_voronoi(voronoi* voronoi);

void voronoi_incremental(voronoi* voronoi, vertex* vertex);

void upgrade_voronoi_diagram(up_data* data);

voronoi* process_incremental(double width, double height, list* vertices);
#endif
