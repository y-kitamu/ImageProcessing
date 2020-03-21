/*
 * plugin_base.hpp
 * 
 * 
 * Create Date : 2020-03-14 10:57:30
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */

#ifndef PLUGIN_BASE_HPP__
#define PLUGIN_BASE_HPP__

#include <Eigen/Eigen>
#include <opencv2/opencv.hpp>

#include "image.hpp"

namespace gl {

class PluginBase {
    /*
     * Plugin は window 上に表示する object を描画する責任がある
     */
  public:
    virtual ~PluginBase() {}

    static inline std::function<Eigen::Vector2d(Eigen::Vector2d)> imageCoord2GLCoord;
    static inline std::function<Eigen::Vector2d(Eigen::Vector2d)> glCoord2ImageCoord;
    static inline std::function<bool(double,double)> isPointInImage;
    
    static Eigen::Vector2d imageCoord2GLCoordImpl(Eigen::Vector2d img_pt, int frame_idx);
    static Eigen::Vector2d glCoord2ImageCoordImpl(Eigen::Vector2d gl_pt, int frame_idx);
    static bool isPointInImageImpl(double x, double y, int frame_idx);

    // BaseGL::draw() の最初に呼び出される
    void initDraw() {
        setTexture();
    }
    virtual void setTexture() = 0;
    virtual void loadGLObjects() = 0;
    virtual void drawGL() = 0;
    virtual void drawImgui() = 0;
    virtual void checkKeyboardAndMouseInput() {};

  public:
    inline static GLuint shader_program_id;

  protected:
    static constexpr float mouse_scroll_scale = 0.10;
    inline static double prev_xpos = 0.0, prev_ypos = 0.0, xpos = 0.0, ypos = 0.0; // mouse position
    
    Eigen::Vector2d cursor_img_pt;
};

} // namespace gl


#endif // PLUGIN_BASE_HPP__
