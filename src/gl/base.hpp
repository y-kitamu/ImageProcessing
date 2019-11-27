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
#include "imgui_style_color.hpp"


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
        initGL();
        initImgui();

        glfwSetWindowSizeCallback(img_window, windowSizeCallback); // callback を基底クラスと派生クラスにバラバラにおいていい？
    }

    ~BaseGL() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(img_window);
        glfwTerminate();
        return;
    }
    
    void setShader(std::string vertex_shader_fname, std::string fragment_shader_fname) {
        program_id = LoadShaders(vertex_shader_fname.c_str(), fragment_shader_fname.c_str());
    }
    
    virtual void draw();
    virtual void initDraw() {};

    // draw のループの中で呼ばれる関数
    virtual void loadGLObjects() {};
    virtual void drawGL() {};
    virtual void drawImgui() {};
    virtual void checkKeyboardAndMouseInput();

    // callbacks
    static void windowSizeCallback(GLFWwindow* window, int w, int h);
    
  private:
    void initGL();
    void initImgui();
    // debug call back function
    static void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, 
                                       GLsizei length, const GLchar *message, void *userParam);

  public:
    static int width, height;
    static float width_inv, height_inv;
    GLFWwindow *img_window;
    GLuint program_id;
    // const char* glsl_version = "#version 460 core";
    const char* glsl_version="#version 330";
    
  protected:
    ImGuiStyleColor imgui_theme;
};

} // namespace gl

#endif // __GL_BASE_HPP__
