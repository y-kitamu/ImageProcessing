# imgui
add_library(imgui imgui/examples/imgui_impl_opengl3.cpp imgui/examples/imgui_impl_glfw.cpp)

set(EXTERNAL_LIBS ${EXTERNAL_LIBS} imgui CACHE INTERNAL "")
# message("external libs : ${EXTERNAL_LIBS}")
