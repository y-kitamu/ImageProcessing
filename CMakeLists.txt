cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)

include(cmake/utility.cmake)

set(LIBRARIES "")
set(EXTERNAL_LIBS "")

# opencv 
find_package(OpenCV REQUIRED)

if (OpenCV_FOUND)
  include_directories(${OpenCV_INCLUDE_DIRS})
  set(EXTERNAL_LIBS ${EXTERNAL_LIBS} ${OpenCV_LIBS})
endif(OpenCV_FOUND)

include_directories(AFTER ext)

include_directories(AFTER TGV)
add_subdirectory(TGV)

add_subdirectory(snipets)

