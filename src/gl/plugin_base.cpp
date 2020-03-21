/*
 * plugin_base.cpp
 *
 * Create Date : 2020-03-21 17:07:14
 * Copyright (c) 2019- %name% <ymyk6602@gmail.com>
 */
#include "plugin_base.hpp"
#include "base.hpp"

namespace gl {

Eigen::Vector2d PluginBase::imageCoord2GLCoordImpl(Eigen::Vector2d img_pt, int idx) {
    int image_width = BaseGL::frames[idx]->getImageWidth();
    int image_height = BaseGL::frames[idx]->getImageHeight();
    float scale = BaseGL::frames[idx]->getScale();
    float offset_x = BaseGL::frames[idx]->getOffsetX();
    float offset_y = BaseGL::frames[idx]->getOffsetY();
    float aspect_ratio = (float)image_height / image_width * BaseGL::view_width / BaseGL::view_height;
    Eigen::Vector2d gl_pt;

    gl_pt.x() = (img_pt.x() * scale / image_width + offset_x - scale * 0.5 + 1.0f)
        * 0.5 * BaseGL::view_width;
    gl_pt.y() = (1 - ((1 - img_pt.y() / image_height) * scale * aspect_ratio + offset_y -
                      scale * 0.5 * aspect_ratio + 1.0f) * 0.5) * BaseGL::view_height;
    return gl_pt;
}


Eigen::Vector2d PluginBase::glCoord2ImageCoordImpl(Eigen::Vector2d gl_pt, int idx) {
    /*
     * window の coordinate (左上(0, 0)、右下(window_width, window_height)) から
     * Image coordinate (画像左上(0, 0), 画像右下(image_width, image_height))
     */
    int image_width = BaseGL::frames[idx]->getImageWidth();
    int image_height = BaseGL::frames[idx]->getImageHeight();
    float scale = BaseGL::frames[idx]->getScale();
    float offset_x = BaseGL::frames[idx]->getOffsetX();
    float offset_y = BaseGL::frames[idx]->getOffsetY();
    
    double aspect_ratio = 1.0 * image_height  * BaseGL::view_width / (image_width * BaseGL::view_height);
    Eigen::Vector2d img_pt;
    
    img_pt.x() = (2.0 * gl_pt.x() * BaseGL::width_inv - 1.0 - offset_x + scale * 0.5)
        * image_width / scale;
    img_pt.y() = (1 - ((1 - gl_pt.y() * BaseGL::height_inv) * 2 - 1.0 - offset_y +
                       scale * 0.5 * aspect_ratio) / (scale * aspect_ratio)) * image_height;
    
    return img_pt;
}

} // namespace gl
