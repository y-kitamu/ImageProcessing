set(module_name uvc)

file(GLOB file_list test_*.cpp)

find_package(realsense2 HINTS)
# message("FOUND : ${realsense2_FOUND}, ${realsense2_VERSION}")

# if (realsense2_FOUND)
#   message("realsense include dir : ${realsense2_INCLUDE_DIR}")
#   message("realsense lib : ${realsense2_LIBRARY}")
# endif()

foreach (fname IN LISTS file_list)
  get_filename_component(basename ${fname} NAME_WE)
  add_executable(${module_name}_${basename} ${fname})
  target_link_libraries(${module_name}_${basename} ${LIBRARIES} ${EXTERNAL_LIBS} ${realsense2_LIBRARY})
  target_link_libraries(${module_name}_${basename} Boost::filesystem Boost::system)
endforeach()
