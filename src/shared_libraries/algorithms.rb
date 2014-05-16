# -*- coding: utf-8 -*-
# 
# Author: José Emiliano Cabrera Blancas (jemiliano.cabrera@gmail.com)
#
require 'ffi'

require File.expand_path(File.join(File.dirname(__FILE__),"points"))
require File.expand_path(File.join(File.dirname(__FILE__),"half_edge"))
require File.expand_path(File.join(File.dirname(__FILE__),"face"))

require File.expand_path(File.join(File.dirname(__FILE__),"double_linked_list"))
require File.expand_path(File.join(File.dirname(__FILE__), "dcel"))
require File.expand_path(File.join(File.dirname(__FILE__), "voronoi"))
#
# Modulo para mapear una DCEL y sus funciones de C a Ruby.
#
module Algorithms
  extend FFI::Library
  ffi_lib "lib/libalgorithms.so"
  
  attach_function :steps_voronoi, [Voronoi::Voronoi.by_ref], :int
  attach_function :voronoi_incremental, [Voronoi::Voronoi.by_ref, Points::Point.by_ref], :void

  attach_function :process_incremental, [:double, :double, List::List.by_ref], :void
end
