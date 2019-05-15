cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/utility.cmake)

set(LIBRARIES "")
set(EXTERNAL_LIBS "")

# opencv, glew, glfw
find_package(OpenCV REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
set(EXTERNAL_LIBS ${EXTERNAL_LIBS} ${GLEW_LIBRARIES} glfw ${OpenCV_LIBS})

include_directories(${OpenCV_INCLUDE_DIRS})

# boost  (参考 : https://sgry.jp/blog/2017/06/06/4624/)
# target_link_libraries(<executable_name> Boost::filesystem Boost::system) みたいな感じで link する
if (EXISTS "/home/kitamura/lib/boost_1_67_0")
  set(BOOST_ROOT "/home/kitamura/lib/boost_1_67_0")
elseif (EXISTS "/home/kitamura/lib/boost/")
  set(BOOST_ROOT "/home/kitamura/lib/boost")
endif()
message("BOOST_ROOT : ${BOOST_ROOT}")
find_package(Boost REQUIRED COMPONENTS system filesystem)

include_directories(SYSTEM AFTER ext ext/eigen)

include_directories(AFTER TGV)
add_subdirectory(TGV)

add_subdirectory(snipets)
