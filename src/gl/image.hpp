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
  public:
    Image(const cv::Mat &img,
          std::string shader_basename = "simple_texture",
          fs::path shader_dir = fs::path(__FILE__).parent_path() / fs::path("shader"));
    void setTexture();
    void updateParams(int image_width, int image_height, float scale, float offset_x, float offset_y);
    void load(int window_width, int window_height);
    void draw();

    Eigen::Vector2d imageCoord2GLCoord(Eigen::Vector2d img_pt);
    Eigen::Vector2d glCoord2ImageCoord(Eigen::Vector2d gl_pt);

    int getImageWidth() { return image_width; }
    int getImageHeight() { return image_height; }
    float getScale() { return scale; }
    float getOffsetX() { return offset_x; }
    float getOffsetY() { return offset_y; }
    
  private:
    cv::Mat image;
    GLuint texture_id;
    GLuint vao, vbo;
    GLuint program_id; // shader program id 
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
