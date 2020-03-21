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

    std::string getName() override;

    static void setViewport(int view_idx);
    static void setViewport(int minx, int miny, int width, int height);
    static void calcCursorPointView();
    static void calcCursorPointFrame(double x, double y);

  public:
    inline static int first_frame_idx = 0, second_frame_idx = 1;  // 表示する画像の index
    
  private:
    const std::string shader_basename = "simple_texture";
    
    inline static bool is_left_button_pressed = false;
    inline static int drawing_frame_idx = 0;
    inline static int focused_frame_idx = -1; // -1: none, used in cursor callback
};

}

#endif // PLUGIN_MULTI_HPP__
