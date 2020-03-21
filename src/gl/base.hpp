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
#include "plugin_simple.hpp"
#include "plugin_multi.hpp"
#include "line.hpp"

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
  public:
    class Viewport {
      public:
        constexpr static int LEFT = 1;
        constexpr static int RIGHT = 2;
        constexpr static int ALL = 0;
    };
    class PluginNames {
      public:
        const inline static std::string SIMPLE = "Simple";
        const inline static std::string MULTI = "Multi";
    };
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

    void addLine(std::shared_ptr<Point> pt0, std::shared_ptr<Point> pt1,
                 Eigen::Vector4f color=Eigen::Vector4f(1.0, 1.0, 1.0, 0.5)) {
        lines.addLine(pt0, pt1, color);
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
    inline static const fs::path shader_dir = fs::path(__FILE__).parent_path() / fs::path("shader");
    
    inline static int width = 1024, height = 768; // window width and height
    inline static int view_width = width, view_height = height;
    inline static float width_inv = 1.0f / view_width, height_inv = 1.0f / view_height;
    inline static GLFWwindow *img_window;
    // const char* glsl_version = "#version 460 core";

    const std::vector<std::pair<std::string, std::function<std::shared_ptr<PluginBase>()>>> plugins {
        {PluginNames::SIMPLE, std::make_shared<PluginSimple>},
        {PluginNames::MULTI, std::make_shared<PluginMulti>}
    };
    inline static std::shared_ptr<PluginBase> plugin;
    inline static std::vector<std::shared_ptr<Image>> frames = std::vector<std::shared_ptr<Image>>();
    inline static Lines lines;
    
  private:
    ImGuiStyleColor imgui_theme;
    static inline std::once_flag init_flag;
    static inline std::unique_ptr<BaseGL> singleton;
};

} // namespace gl

#endif // __GL_BASE_HPP__
