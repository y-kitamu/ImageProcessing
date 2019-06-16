#ifndef __GL_BASE_HPP__
#define __GL_BASE_HPP__

#include <cstdlib>
#include <iostream>

#include <sstream>
#include <thread>
#include <chrono>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// #include <GL/glew.h> // glew だとエラーになった
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"


namespace gl {

class BaseGL {
    /*
     * open gl の描画 class の一番基底の class
     *
     * TODO : 
     *   - 呼び出しプログラム側から opengl object を追加できるようにする。 (点、多角形？、画像、線)
     */
  public:
    BaseGL() {
        std::cout << "initialize Base" << std::endl;
        init_gl();
        init_imgui();
    }

    ~BaseGL() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(img_window);
        glfwTerminate();
        return;
    }
    
    void set_shader(std::string vertex_shader_fname, std::string fragment_shader_fname) {
        program_id = LoadShaders(vertex_shader_fname.c_str(), fragment_shader_fname.c_str());
    }
    
    virtual void load_gl_objects() {};
    
    virtual void draw();
    virtual void draw_gl() {};
    virtual void draw_imgui() {};
    
    virtual void check_keyboard_and_mouse_input();

  private:
    void init_gl();
    void init_imgui();

  public:
    GLFWwindow *img_window;
    int width = 1024, height = 768;
    GLuint program_id;
    // const char* glsl_version = "#version 460 core";
    const char* glsl_version="#version 330";
};

} // namespace gl

#endif // __GL_BASE_HPP__
