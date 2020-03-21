/*
 * plugin_simple.hpp
 * 
 * 
 * Create Date : 2020-03-14 18:31:04
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */

#ifndef PLUGIN_SIMPLE_HPP__
#define PLUGIN_SIMPLE_HPP__

#include <boost/filesystem.hpp>

#include "plugin_base.hpp"

namespace fs = boost::filesystem;

namespace gl {

class PluginSimple: public PluginBase {
  public:
    PluginSimple();

  private:
    static void scrollCallback(GLFWwindow * window, double xoffset, double yoffset);
    static void mouseCallback(GLFWwindow *window, int button, int action, int mods);
    static void cursorCallback(GLFWwindow * window, double xpos, double ypos);
    static void windowSizeCallback(GLFWwindow* window, int w, int h);
    static void framebufferSizeCallback(GLFWwindow* window, int w, int h);

    void setTexture() override;
    void loadGLObjects() override;
    void drawGL() override;
    void drawImgui() override;
    
    static bool isPointInImage(double x, double y);
    static void setViewport();

  private:
    const std::string shader_basename = "simple_texture";
    const fs::path shader_dir = fs::path(__FILE__).parent_path() / fs::path("shader");

    inline static int frame_idx = 0;  // 表示する画像の index
};

} // namespace gl


#endif // PLUGIN_SIMPLE_HPP__
