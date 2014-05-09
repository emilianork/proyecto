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

struct voronoi_diagram {
	struct dcel* diagram;
	struct double_linked_list* processing;
	struct double_linked_list* seeds;
	int degenerate_case;
};

typedef struct voronoi_diagram voronoi;

voronoi* init_voronoi_diagram(double width, double height);

/** Falta componer esta funcion. */
void destroy_voronoi_diagram(voronoi* voronoi_diagram);

dcel* construct_bounded_box(double width, double height);

#endif
