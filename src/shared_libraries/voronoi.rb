# -*- coding: utf-8 -*-
# 
# Author: José Emiliano Cabrera Blancas (jemiliano.cabrera@gmail.com)
#
require 'ffi'

require File.expand_path(File.join(File.dirname(__FILE__),"points"))
require File.expand_path(File.join(File.dirname(__FILE__),"half_edge"))
require File.expand_path(File.join(File.dirname(__FILE__),"face"))

require File.expand_path(File.join(File.dirname(__FILE__),"double_linked_list"))
require File.expand_path(File.join(File.dirname(__FILE__), "rb_trees"))
require File.expand_path(File.join(File.dirname(__FILE__), "dcel"))

#
# Modulo para mapear una DCEL y sus funciones de C a Ruby.
#
module Voronoi
  extend FFI::Library
  ffi_lib "lib/libvoronoi.so"
  
  class Voronoi < FFI::Struct
    layout :diagram, DCEL::DCEL.by_ref,
           :processing, List::List.by_ref,
           :seeds, List::List.by_ref,
           :degenerate_case, :int
  end

  attach_function :init_voronoi_diagram, [:double, :double], Voronoi.by_ref
end
