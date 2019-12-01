MACRO(CREATE_HALIDE_LIB)
  CREATE_HALIDE_IMPL()
  IF (DEFINED halide_generated_binaries)
    add_dependencies(${module_name} ${module_name}_halide)
    set(HALIDE_OBJECTS ${HALIDE_OBJECTS} ${halide_generated_binaries} CACHE INTERNAL "")
  ENDIF()
ENDMACRO()

MACRO(CREATE_HALIDE_IMPL)
  IF (DEFINED halide_generated_binaries)
    unset(halide_generated_binaries)
  ENDIF()
  
  IF (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/halide)
    set(header_gen_path ${CMAKE_CURRENT_SOURCE_DIR}/halide)
    set(binary_gen_path ${CMAKE_CURRENT_BINARY_DIR})
    
    FILE(GLOB flist ${CMAKE_CURRENT_SOURCE_DIR}/halide/[a-z]*.cpp)
    FOREACH(fname IN LISTS flist)
      set(target host)
      IF (DEFINED HALIDE_RUNTIME_GENERATED)
        set(target ${target}-no_runtime)
      ENDIF()
      message("target : ${target}")
      
      get_filename_component(bname ${fname} NAME_WE)
      set(tmp_generator ${bname}_tmp)
      set(output_header ${bname}.h)
      set(output_binary ${bname}.a)
      
      add_custom_command(
        OUTPUT ${tmp_generator}
        COMMAND ${CMAKE_CXX_COMPILER} ${fname} --std=c++1z -lHalide -ldl -lz -lpthread -fno-rtti -o ${tmp_generator}
        DEPENDS ${fname}
        )

      add_custom_command(
        OUTPUT ${ouput_header} ${output_binary}
        COMMAND ./${tmp_generator} -g ${bname} -o ${header_gen_path} target=${target} -e h
        COMMAND ./${tmp_generator} -g ${bname} -o ${binary_gen_path} target=${target} -e static_library
        DEPENDS ${tmp_generator}
        )
      set_source_files_properties(${output_binary} PROPERTIES
        EXTERNAL_OBJECT TRUE
        GENERATED TRUE)
      
      set(halide_generated_binaries ${halide_generated_binaries} ${binary_gen_path}/${output_binary})
      set(HALIDE_RUNTIME_GENERATED "")
    ENDFOREACH()
    IF (DEFINED halide_generated_binaries)
      add_custom_target(${module_name}_halide ALL SOURCES ${halide_generated_binaries})
      # message(${halide_generated_binaries})
    ENDIF()
  ENDIF()
ENDMACRO()
