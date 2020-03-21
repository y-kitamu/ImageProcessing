/*
 * plugin_multi.hpp
 * 
 * 
 * Create Date : 2020-03-21 10:05:26
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */

#ifndef PLUGIN_MULTI_HPP__
#define PLUGIN_MULTI_HPP__

#include "plugin_base.hpp"

namespace gl {

class PluginMulti : public PluginBase {
  public:
    PluginMulti();

  private:
    static void scrollCallback(GLFWwindow * window, double xoffset, double yoffset);
    static void mouseCallback(GLFWwindow *window, int button, int action, int mods);
    static void cursorCallback(GLFWwindow * window, double xpos, double ypos);
    
    void setTexture() override;
    void loadGLObjects() override;
    void drawGL() override;
    void drawImgui() override;

    static int isPointInImage(double x, double y);
    static void setViewport(int view_idx);
    static void setViewport(int minx, int miny, int width, int height);
    static void calcCursorPointView();

  private:
    const std::string shader_basename = "simple_texture";
    const fs::path shader_dir = fs::path(__FILE__).parent_path() / fs::path("shader");
    
    inline static int first_frame_idx = 0, second_frame_idx = 1;  // 表示する画像の index
    inline static int frame_idx = 0; 
};

}

#endif // PLUGIN_MULTI_HPP__
