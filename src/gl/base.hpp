#ifndef __GL_BASE_HPP__
#define __GL_BASE_HPP__

#include <cstdlib>
#include <iostream>
#include <opencv2/opencv.hpp>

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
    
    void draw() {
		// Clear the screen
        glfwMakeContextCurrent(img_window);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw_gl();  // virtual : open gl objects の描画処理
        draw_imgui();  // virtual : imgui の描画処理

        glClear(GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(img_window);
    }

    void set_shader(std::string vertex_shader_fname, std::string fragment_shader_fname) {
        program_id = LoadShaders(vertex_shader_fname.c_str(), fragment_shader_fname.c_str());
    }
    
    virtual void load_gl_objects() {};
    virtual void draw_gl() {};
    virtual void draw_imgui() {};
    
  private:
    void init_gl();
    void init_imgui();

  public:
    GLFWwindow *img_window;
    int width = 1024, height = 768;
    GLuint program_id;
    const char* glsl_version = "#version 460 core";
};

} // namespace gl

#endif // __GL_BASE_HPP__
