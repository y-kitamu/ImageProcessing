set(module_name "ceres")

file(GLOB file_list ^[a-zA-Z]*.cpp)

foreach(fname IN LISTS file_list)
  get_filename_component(basename ${fname} NAME_WE)
  add_executable(${module_name}_${basename} ${fname})
  target_link_libraries(${module_name}_${basename} ${EXTERNAL_LIBS})
endforeach()
