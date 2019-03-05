cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)

include(cmake/utility.cmake)

find_package(OpenCV REQUIRED)

set(LIBRARIES "")
set(EXTERNAL_LIBS "")

if (OpenCV_FOUND)
  include_directories(${OpenCV_INCLUDE_DIRS})
  set(EXTERNAL_LIBS ${EXTERNAL_LIBS} ${OpenCV_LIBS})
endif(OpenCV_FOUND)

include_directories(AFTER TGV)
add_subdirectory(TGV)

add_subdirectory(snipets)

