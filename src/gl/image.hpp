/*
 * image.hpp
 * 
 * 
 * Create Date : 2019-12-15 10:10:33
 * Copyright (c) 2019 Yusuke Kitamura <ymyk6602@gmail.com>
 */

#ifndef IMAGE_HPP__
#define IMAGE_HPP__

#include <opencv2/opencv.hpp>
#include <Eigen/Eigen>
#include <boost/filesystem.hpp>

#include "base_include.hpp"
#include "utility.hpp"
#include "point.hpp"

namespace fs = boost::filesystem;

namespace gl {

class Image {
    /*
     * debug 画面上に表示する画像に関するクラス
     */
  public:
    Image(const cv::Mat &img);
    void setTexture();
    void load();
    void draw();

    template<class T>
    void updateParams() {
        // reflect and retain changes made by callbacks 
        image_width = T::getImageWidth();
        image_height = T::getImageHeight();
        scale = T::getScale();
        offset_x = T::getOffsetX();
        offset_y = T::getOffsetY();
        
        points.updateGLPts();
    }

    int getImageWidth() { return image_width; }
    int getImageHeight() { return image_height; }
    float getScale() { return scale; }
    float getOffsetX() { return offset_x; }
    float getOffsetY() { return offset_y; }

  public:
    Points points;
    
  private:
    cv::Mat image;
    GLuint texture_id;
    GLuint vao, vbo;
    int vertices;  // 頂点の数

    GLint swizzle_mask[4];
    GLenum texture_format;  // pixel 上の画像の形式を指定
    GLenum texture_internal_format; //テクスチャを opengl 内部でどう保持するかを指定
    GLenum pixel_format; // pixel のフォーマットを指定
    int image_width, image_height;
    float scale = 1.0f;
    float offset_x = 0.0f, offset_y = 0.0f;
};

}

#endif // IMAGE_HPP__
