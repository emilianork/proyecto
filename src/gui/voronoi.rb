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

  rand_size = 7
  random_vertex = []

  rand_size.times.each do
    random_vertex << [rand(width).to_f, rand(height).to_f]
  end
  
  #p random_vertex

  @points = [[26.0, 96.0], [347.0, 234.0], [136.0, 362.0]].map {|x| Points.init_point(x[0], x[1], "")}
  
  @index = 0
  
  @voronoi = Voronoi.init_voronoi_diagram(width,height)
  
  @up = false
 
end

def draw
  if key_pressed?
    if @up
      next_step
      translate(0, height)
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

  c_half_edges = List.create_copy_list(@voronoi[:diagram][:half_edge])

  half_edge = []

  c_half_edges[:size].times.each do
    
    edge = HalfEdge.cast_half_edge(List.pop_front(c_half_edges))

    ruby_edge = []

    ruby_edge << [edge[:first][:x], edge[:first][:y]]
    ruby_edge << [edge[:last][:x], edge[:last][:y]]
    
    half_edge << ruby_edge
  end

  puts "Ruby Aristas"

  half_edge.each do
    |half_edge|
    a = half_edge[0]
    b = half_edge[1]

    puts "#{a}  #{b}"

    line(a[0].abs,-a[1].abs,b[0].abs,-b[1].abs)
  end
  c_seeds = List.create_copy_list(@voronoi[:seeds])
  
  array_seeds = []

  c_seeds[:size].times.each do
    array_seeds << List.pop_front(c_seeds)
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
    ellipse(vertex[0], -vertex[1], 5.0,5.0)
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
