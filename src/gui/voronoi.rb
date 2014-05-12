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
  
 # random_vertex = [[50.0, 100.0], [350.0, 100.0], [201.0,100.0]]


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

# Public: Dibuja el conjunto de vertices y aristas @vertex y @half_edge
#
# Esta funcion la debes de modificar para que dibuje los puntos y aristas
def draw_vertex_and_half_edges
  
  if (@process[@index] != nil)
    background 255, 255, 255
    
    vertices = @process[@index][:vertices]
    half_edges = @process[@index][:half_edges]
    
    vertices.each do
      |vertex|
      ellipse(vertex[0], -vertex[1], 5.0,5.0)
    end

    half_edges.each do
      |edge|
      
      a = edge[0]
      b = edge[1]

      line(a[0].abs,-a[1].abs,b[0].abs,-b[1].abs)
    end
  end
end


def read()

  @process = []
  f = File.open("salida.txt", "r")

  if (f.eof?)
    puts "C todavía no termino de escribir el archivo"
    exit(0)
  end

  while (not(f.eof?))
    
    vertices = []
    size_vertex = f.gets.split.last.to_i
    
    size_vertex.times.each do
      line = f.gets.split
      
      vertex = [line[0].to_f, line[1].to_f]
      
      vertices << vertex
      
    end
    
    half_edges = []
    size_edges = f.gets.split.last.to_i
    
    size_edges.times.each do
      line = f.gets.split
      
      edge = [[line[0].to_f,line[1].to_f],[line[2].to_f, line[3].to_f]]
      
      half_edges << edge

    end

    @process << {vertices: vertices, half_edges: half_edges}

  end  

  f.close
  
end
