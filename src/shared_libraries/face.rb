# -*- coding: utf-8 -*-
# 
# Author: José Emiliano Cabrera Blancas (jemiliano.cabrera@gmail.com)
#
require 'ffi'

require File.expand_path(File.join(File.dirname(__FILE__),"half_edge"))

#
# Modulo para mapear las caras de C a Ruby, junto con sus funciones.
#
module Face
  extend FFI::Library
  ffi_lib "lib/libface.so"

  class Face < FFI::ManagedStruct
    layout :name, :string,
           :outer_componenet, HalfEdge::HalfEdge.by_ref,
           :inner_component, :pointer,
           :center, Points::Point.by_ref
   

    def self.release(ptr)
        Face.free_object(ptr)
    end
    
  end
  
  attach_function :init_face, [:string, :pointer, :pointer], Face.by_ref
  attach_function :cast_face, [:pointer], Face.by_ref
end
