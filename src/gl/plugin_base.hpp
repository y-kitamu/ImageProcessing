/*
 * plugin_base.hpp
 * 
 * 
 * Create Date : 2020-03-14 10:57:30
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */

#ifndef PLUGIN_BASE_HPP__
#define PLUGIN_BASE_HPP__

#include <opencv2/opencv.hpp>

#include "image.hpp"

namespace gl {

class PluginBase {
    /*
     * Plugin は window 上に表示する object を描画する責任がある
     */
  public:
    PluginBase(std::string shader_basename,
               fs::path shader_dir = fs::path(__FILE__).parent_path() / fs::path("shader"));
    virtual ~PluginBase() {}
    
    // draw() の最初に呼び出される
    void initDraw() {
        setTexture();
    }
    virtual void loadGLObjects();
    virtual void drawGL();
    virtual void drawImgui();
    virtual void checkKeyboardAndMouseInput() {};

    static Eigen::Vector2d imageCoord2GLCoord(Eigen::Vector2d img_pt);
    static Eigen::Vector2d glCoord2ImageCoord(Eigen::Vector2d gl_pt);
    static bool isPointInImage(double x, double y);

    // property getters
    static int getImageWidth() { return image_width; }
    static int getImageHeight() { return image_height; }
    static float getScale() { return scale; }
    static float getOffsetX() { return offset_x; }
    static float getOffsetY() { return offset_y; }
    
  private:
    void setTexture();

    static void scrollCallback(GLFWwindow * window, double xoffset, double yoffset);
    static void mouseCallback(GLFWwindow *window, int button, int action, int mods);
    static void cursorCallback(GLFWwindow * window, double xpos, double ypos);

  public:
    inline static GLuint shader_program_id;

  protected:
    static constexpr float mouse_scroll_scale = 0.10;
    inline static double prev_xpos = 0.0, prev_ypos = 0.0, xpos = 0.0, ypos = 0.0; // mouse position
    inline static bool is_left_button_pressed = false, is_pressed_in_image = false;
    
    inline static int image_width = 0, image_height = 0;
    inline static float scale = 1.0f;
    inline static float offset_x = 0.0f, offset_y = 0.0f;

    int frame_idx = 0;  // 表示する画像の index
    Eigen::Vector2d cursor_img_pt;
};

} // namespace gl


#endif // PLUGIN_BASE_HPP__
