MACRO(CREATE_HALIDE)
  IF (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/halide)
    set(header_gen_path ${CMAKE_CURRENT_SOURCE_DIR}/halide)
    set(binary_gen_path ${CMAKE_CURRENT_BINARY_DIR}/halide) 
    
    FILE(GLOB file_list ${CMAKE_CURRENT_SOURCE_DIR}/halide/[a-z]*.cpp)
    FOREACH(fname IN LISTS file_list)
      get_filename_component(basename ${fname} NAME_WE)
      set(tmp_generator ${fname}_tmp)
      set(output_header ${fname}.h)
      set(output_binary ${fname}.a)
      
      add_custom_command(
        OUTPUT ${tmp_generator}
        COMMAND ${CMAKE_CXX_COMPILER} ${fname} --std=c++1z -lHalide -ldl -fno-rtti ${tmp_generator}
        DEPENDS ${fname}
        )

      add_custom_command(
        OUTPUT ${ouput_header} ${output_binary}
        COMMAND ./${tmp_generator} -g ${basename} -o ${header_gen_path} target=host -e h
        COMMAND ./${tmp_generator} -g ${basename} -o ${binary_gen_path} target=host -e static_library
        DEPENDS ${tmp_generator_path}
        )
      set_source_files_properties(${output_binary} PROPERTIES
        EXTERNAL_OBJECT TRUE
        GENERATED TRUE)
      set(generated_binaries ${generated_binaries} ${binary_gen_path}/${output_binary})
    ENDFOREACH()
    add_custom_target(${module_name}_halide ALL SOURCES ${halide_objects})
    set(HALIDE_OBJECTS ${HALIDE_OBJECTS} ${generated_binaries} CACHE INTERNAL "")
  ENDIF()
MACRO()
