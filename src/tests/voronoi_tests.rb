require File.expand_path(File.join(File.dirname(__FILE__), "../shared_libraries/algorithms"))

#
# Reabre el modulo de Test para agregar la clase TestPoints, la cual
# tiene los test de las funciones de la estructura de C.
#

module TestC
  require 'ffi'
  module TestVoronoi
    extend FFI::Library
    ffi_lib "lib/libvoronoi_tests.so"
    
    attach_function :simple_case, [], :int
    attach_function :degenerate_case, [], :int
    attach_function :general_case, [], :int

  end
end


module Test
  class TestVoronoi
    def self.simple_case
      TestC::TestVoronoi.simple_case == 1
    end

    def self.degenerate_case
      TestC::TestVoronoi.degenerate_case == 1
    end

    def self.general_case
      TestC::TestVoronoi.general_case == 1
    end
  end
end
