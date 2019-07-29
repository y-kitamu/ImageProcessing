cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

project(ImageProcessing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/utility.cmake)

set(LIBRARIES "" CACHE INTERNAL "")
set(EXTERNAL_LIBS "" CACHE INTERNAL "")

### External Libraries
set(EXTERNAL_LIBS ${EXTERNAL_LIBS} ${CMAKE_DL_LIBS} CACHE INTERNAL "")

# opencv, glew, glfw
find_package(OpenCV REQUIRED)
set(OpenGL_GL_PREFERENCE GLVND) # legacy と glvnd の2種類の opengl が見つかったとき glvnd を選択する
find_package(OpenGL REQUIRED)
# find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
set(EXTERNAL_LIBS
  ${EXTERNAL_LIBS}
  ${OPENGL_LIBRARY}
  # ${GLEW_LIBRARIES}
  glfw
  ${OpenCV_LIBS}
  CACHE INTERNAL "")

include_directories(${OpenCV_INCLUDE_DIRS})

# boost  (参考 : https://sgry.jp/blog/2017/06/06/4624/)
# target_link_libraries(<executable_name> Boost::filesystem Boost::system) みたいな感じで link する
if (EXISTS "/home/kitamura/lib/boost_1_67_0")
  set(BOOST_ROOT "/home/kitamura/lib/boost_1_67_0")
elseif (EXISTS "/home/kitamura/lib/boost/")
  set(BOOST_ROOT "/home/kitamura/lib/boost")
endif()
# message("BOOST_ROOT : ${BOOST_ROOT}")
find_package(Boost REQUIRED COMPONENTS system filesystem)
set(EXTERNAL_LIBS ${EXTERNAL_LIBS} Boost::system Boost::filesystem CACHE INTERNAL "")

# ceres solver
find_package(Ceres REQUIRED)
set(EXTERNAL_LIBS ${EXTERNAL_LIBS} ${CERES_LIBRARIES} CACHE INTERNAL "")

# another submodules
include_directories(AFTER ext ext/imgui ext/fmt/include ext/cereal/include)
include_directories(AFTER ext/imgui/examples ext/imgui/examples/libs/gl3w)  # TODO : refactor 
add_subdirectory(ext)
# message(${EXTERNAL_LIBS})

### Add Subdirectory Libraries
include_directories(AFTER src src/TGV src/cereal)
add_subdirectory(src/gl)
add_subdirectory(src/TGV)

### Add Executable
add_subdirectory(snipets)

# get_directory_property(INCLUDE_PATH INCLUDE_DIRECTORIES)
# foreach(SINGLE_PATH ${INCLUDE_PATH})
#   message("${SINGLE_PATH}")
# endforeach()
