#include <cstdlib>
#include <iostream>
#include <opencv2/opencv.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "shader.hpp"

namespace gl {
class SimpleGL {
  public:
    SimpleGL() {
        // 初期化
        init_gl();
        init_imgui();
        load_gl_objects();
    }


    void draw(cv::Mat frame) {
		// Clear the screen
        glfwMakeContextCurrent(img_window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw_gl(frame);
        draw_imgui();

        glClear(GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(img_window);
    }

    void set_shader(std::string vert_shader, std::string frag_shader) {
        vertex_shader_fname = vert_shader;
        fragment_shader_fname = frag_shader;

        program_id = LoadShaders(vertex_shader_fname.c_str(), fragment_shader_fname.c_str());
    }

    ~SimpleGL() {
        std::cout << "destructor start" << std::endl;
        // ImGui_ImplOpenGL3_Shutdown();
        // ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(img_window);
        glfwTerminate();
        return;
    }

  private:
    void init_gl();
    void init_imgui();
    void load_gl_objects();

    void draw_gl(const cv::Mat &fram);
    void draw_imgui();
    
  private:
    int width = 1024, height = 768;
    
    GLFWwindow *img_window;
    GLuint vao, vbo;
    GLuint image;
    GLuint program_id;

    const char* glsl_version = "#version 330";
    std::string vertex_shader_fname, fragment_shader_fname;
    int vertices;
    
};

} // namespace gl
