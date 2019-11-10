set(module_name "profiler")

file(GLOB file_list "test_*.cpp")

foreach(fname IN LISTS file_list)
  get_filename_component(basename ${fname} NAME_WE)
  add_executable(${module_name}_${basename} ${fname})
  target_link_libraries(${module_name}_${basename} ${LIBRARIES} ${EXTERNAL_LIBS})
endforeach()
