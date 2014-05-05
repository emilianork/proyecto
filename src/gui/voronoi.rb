# -*- coding: utf-8 -*-


#
# Author: José Emiliano Cabrera Blancas (jemiliano.cabrera@gmail.com)
#

# Public: Inicializa la ventana y variables de processing
#
require File.expand_path(File.join(File.dirname(__FILE__), "../shared_libraries/algorithms"))

def setup
  background 255, 255, 255
  size(400, 400,P3D)

  # Genera un conjunto random de puntos dentro de la caja.
  @points = [[100.0,100.0], [300.0,300.0], [201.0,10.0]].map {|x| Points.init_point(x[0], x[1], "")}
  
  @index = 0
  
  @voronoi = Voronoi.init_voronoi_diagram(width,height)
  
  @up = false
end

def draw
  if key_pressed?
    if @up
      next_step
      draw_vertex_and_half_edges
      @up = false
    else
      @up = true
    end
  end
end

# Public: Dibuja el conjunto de vertices y aristas @vertex y @half_edge
#
# Esta funcion la debes de modificar para que dibuje los puntos y aristas
def draw_vertex_and_half_edges

  background 255, 255, 255

  c_half_edges = RBTree.rb_tree_to_list(@voronoi[:diagram][:half_edge])

  half_edge = []

  c_half_edges[:size].times.each do
    
    edge = HalfEdge.cast_half_edge(List.pop_front(c_half_edges))

    ruby_edge = []

    ruby_edge << [edge[:first][:x], edge[:first][:y]]
    ruby_edge << [edge[:last][:x], edge[:last][:y]]
    
    half_edge << ruby_edge
  end
  
  half_edge.each do
    |half_edge|
    a = half_edge[0]
    b = half_edge[1]
    line(a[0],a[1],b[0],b[1])
  end
  c_seeds = @voronoi[:seeds]
  
  array_seeds = []

  c_seeds[:size].times.each do
    array_seeds << List.pop_front(c_seeds)
    List.push_back(c_seeds,array_seeds.last)
  end

  array_seeds.each do
    |point|
    
    casted_point = Points.cast_point(point)

    vertex = [casted_point[:x], casted_point[:y], casted_point[:distinct_color]]

    if (vertex[2] == 1)
      fill(255,0,0)
    else
      fill(0,0,0)
    end
    ellipse(vertex[0], vertex[1], 5.0,5.0)
  end
end

# Public: Manda llamar a la funcion para calcular voronoi y convierte los half_edges
#         a aristas en Ruby

def next_step
  if (Algorithms.steps_voronoi(@voronoi) == 0)
    Algorithms.voronoi_incremental(@voronoi,@points[@index])
    @index += 1
  else
    Algorithms.voronoi_incremental(@voronoi,nil)
  end
end
