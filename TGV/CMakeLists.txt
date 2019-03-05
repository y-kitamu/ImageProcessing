set(module_name TGV)

file(GLOB file_list *.cpp)

add_library(${module_name} ${file_list})

set(LIBRARIES ${LIBRARIES} ${module_name} PARENT_SCOPE)
