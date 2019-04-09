# file(GLOB flist "^[a-z].*.cpp")
file(GLOB flist [a-z]*.cpp)

# message("${flist}")

foreach(fname IN LISTS flist)
  get_filename_component(basename ${fname} NAME_WE)
  add_executable(${module_name}_${basename} ${fname})
  target_link_libraries(${module_name}_${basename} ${EXTERNAL_LIBS})
endforeach()
