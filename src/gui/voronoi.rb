# -*- coding: utf-8 -*-


#
# Author: José Emiliano Cabrera Blancas (jemiliano.cabrera@gmail.com)
#

# Public: Inicializa la ventana y variables de processing
#
require File.expand_path(File.join(File.dirname(__FILE__), "../shared_libraries/algorithms"))

def setup
  background 255, 255, 255
  size(400, 400,P2D)

  # Genera un conjunto random de puntos dentro de la caja.

  rand_size = 7
  random_vertex = []

  rand_size.times.each do
    random_vertex << [rand(width).to_f, rand(height).to_f]
  end

  puts "Puntos"
  p random_vertex
  puts ""

  points = random_vertex.map do
    |x| 
    Points.init_point(x[0], x[1], "")
  end
  
  
  list = List.init_double_linked_list(:point)

  points.each do
    |point|
    List.push_back(list, point)
  end

  Algorithms.process_incremental(width, height, list)

  read

  @index = 0
  
  @up = false
 
end

def draw
  if key_pressed?
    if @up
      translate(0, height)
      draw_vertex_and_half_edges
      @index += 1
      @up = false
    else
      @up = true
    end
  end
end

# Public: Metodo que dibuja las aristas y los vertices guardados en el arreglo
#         @process
#
# Esta funcion la debes de modificar para que dibuje los puntos y aristas
def draw_vertex_and_half_edges
  
end

# Public: Metodo que lee el archivo salida.txt y la guarda en una variable 
#         de instancia. (Les recomiendo usar las clases Array y Hash.)
#
def read()
  
  
end
