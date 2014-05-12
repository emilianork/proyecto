require File.expand_path(File.join(File.dirname(__FILE__), "../shared_libraries/algorithms"))

#
# Reabre el modulo de Test para agregar la clase TestPoints, la cual
# tiene los test de las funciones de la estructura de C.
#

module Test
  class TestVoronoi
    def self.simple
      
      voronoi = Voronoi.init_voronoi_diagram(400.0,400.0)

      points = [[26.0, 96.0], [347.0, 234.0], [136.0, 362.0]].map do
        |x| Points.init_point(x[0], x[1], "")
      end

      points.each do
        |point|
          Algorithms.voronoi_incremental(voronoi,point)

        while(Algorithms.steps_voronoi(voronoi) == 1) do
          Algorithms.voronoi_incremental(voronoi,nil)
        end
      end

      c_half_edges = List.create_copy_list(voronoi[:diagram][:half_edge])

      half_edge = []
      
      c_half_edges[:size].times.each do
        
        edge = HalfEdge.cast_half_edge(List.pop_front(c_half_edges))
        
        ruby_edge = []
        
        ruby_edge << [edge[:first][:x].round(6), edge[:first][:y].round(6)]
        ruby_edge << [edge[:last][:x].round(6), edge[:last][:y].round(6)]

        half_edge << ruby_edge
      end
      
      true

    end
  end
end
