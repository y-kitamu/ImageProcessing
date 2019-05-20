# imgui
add_library(imgui
  imgui/imgui.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_widgets.cpp
  imgui/examples/imgui_impl_opengl3.cpp
  imgui/examples/imgui_impl_glfw.cpp
  imgui/examples/libs/gl3w/GL/gl3w.c)
target_link_libraries(imgui ${CMAKE_DL_LIBS} ${OPENGL_LIBRARY})

set(EXTERNAL_LIBS ${EXTERNAL_LIBS} imgui CACHE INTERNAL "")
# message("external libs : ${EXTERNAL_LIBS}")
