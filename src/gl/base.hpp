#ifndef __GL_BASE_HPP__
#define __GL_BASE_HPP__

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <thread>
#include <chrono>

#include <Eigen/Eigen>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>

#include "base_include.hpp"
#include "plugin_base.hpp"

namespace fs = boost::filesystem;

namespace gl {

class BaseGL {
    /*
     * BaseGL は window の表示、menu bar の表示を行い、image object を保持する。
     * Plugin に object を描画させる。
     * BaseGL は singleton で getInstance から instance を取得する。
     * 
     * auto window = gl::BaseGL::getInstance();
     * window->addFrame(cv::Mat mat);
     * window->draw();  // show image (stop by Esc)
     * window->destroy(); // destroy window
     */
  protected:
    BaseGL();

  public:
    static BaseGL& getInstance() {
        // 返り値はpointerじゃなくていい?
        auto func = []() { create(); };
        std::call_once(init_flag, func);
        return *singleton;
    }
    static void destroy() {
        singleton.release();
    }
    ~BaseGL();
    void draw(); // main loop function
    
    std::shared_ptr<Image> addFrame(const cv::Mat &mat) {
        auto frame = std::make_shared<Image>(mat);
        frames.emplace_back(frame);
        return frame;
    }
    
  private:
    static void create() {
        singleton = std::unique_ptr<BaseGL>(new BaseGL());
    }
    // functions called in the constructor
    void initGL();
    void initImgui();

    void drawImguiMenu();
    void checkKeyboardAndMouseInput();
    
    // callbacks
    static void windowSizeCallback(GLFWwindow* window, int w, int h);
    static void framebufferSizeCallback(GLFWwindow * window, int w, int h);
    
    // debug call back function
    static void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, 
                                       GLsizei length, const GLchar *message, void *userParam);

  public:
    static constexpr const char* glsl_version = "#version 330";
    
    inline static int width = 1024, height = 768;
    inline static float width_inv = 1.0f / width, height_inv = 1.0f / height;
    inline static GLFWwindow *img_window;
    // const char* glsl_version = "#version 460 core";

    const std::vector<std::string> plugin_names = {"Simple"};
    inline static std::shared_ptr<PluginBase> plugin;
    inline static std::vector<std::shared_ptr<Image>> frames = std::vector<std::shared_ptr<Image>>();
    
  private:
    ImGuiStyleColor imgui_theme;
    static inline std::once_flag init_flag;
    static inline std::unique_ptr<BaseGL> singleton;
};

} // namespace gl

#endif // __GL_BASE_HPP__
