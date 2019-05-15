cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/utility.cmake)

set(LIBRARIES "")
set(EXTERNAL_LIBS "")

# opencv 
find_package(OpenCV REQUIRED)

if (OpenCV_FOUND)
  include_directories(${OpenCV_INCLUDE_DIRS})
  set(EXTERNAL_LIBS ${EXTERNAL_LIBS} ${OpenCV_LIBS})
endif(OpenCV_FOUND)

# boost  (参考 : https://sgry.jp/blog/2017/06/06/4624/)
# target_link_libraries(<executable_name> Boost::filesystem Boost::system) みたいな感じで link する
set(BOOST_ROOT "/home/kitamura/lib/boost_1_67_0")
find_package(Boost REQUIRED COMPONENTS system filesystem)

include_directories(SYSTEM AFTER ext ext/eigen)

include_directories(AFTER TGV)
add_subdirectory(TGV)

add_subdirectory(snipets)
