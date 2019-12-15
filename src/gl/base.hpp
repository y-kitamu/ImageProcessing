#ifndef __GL_BASE_HPP__
#define __GL_BASE_HPP__

#include <cstdlib>
#include <iostream>
#include <memory>
#include <mutex>

#include <sstream>
#include <thread>
#include <chrono>

#include "base_include.hpp"

namespace gl {

class BaseGL {
    /*
     * open gl の描画 class の一番基底の class
     *
     * TODO : 
     *   - 呼び出しプログラム側から opengl object を追加できるようにする。 (点、多角形？、画像、線)
     */
  protected:
    BaseGL() {
        initGL();
        initImgui();

        glfwSetWindowSizeCallback(img_window, windowSizeCallback); // callback を基底クラスと派生クラスにバラバラにおいていい？
    }

  public:
    ~BaseGL() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(img_window);
        glfwTerminate();
        return;
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
    inline static int width = 1024, height = 768;
    inline static float width_inv = 1.0f / width, height_inv = 1.0f / height;
    GLFWwindow *img_window;
    // const char* glsl_version = "#version 460 core";
    const char* glsl_version="#version 330";
    
  protected:
    ImGuiStyleColor imgui_theme;
};


template<class T>
class SingletonBaseGL : public BaseGL {
  public:
    
    template<class... Args>
    static T& getInstance(Args... args) {
        auto func = [&args...]() { create(args...); };
        std::call_once(init_flag, func);
        return *singleton;
    }

    template<class... Args>
    static void create(Args... args) {
        singleton = std::unique_ptr<T>(new T(args...));
    }
    
    static void destroy() {
        singleton.release();
    }

    static inline std::once_flag init_flag;
    static inline std::unique_ptr<T> singleton;
};

} // namespace gl

#endif // __GL_BASE_HPP__
