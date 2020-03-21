/*
 * point.hpp
 * 
 * 
 * Create Date : 2019-12-15 09:28:23
 * Copyright (c) 2019 Yusuke Kitamura <ymyk6602@gmail.com>
 */

#ifndef POINT_HPP__
#define POINT_HPP__

#include <Eigen/Eigen>
#include <boost/filesystem.hpp>

#include "base_include.hpp"
#include "utility.hpp"

namespace fs = boost::filesystem;

namespace gl {

class Point {
    /*
     * TODO : 高速化
     */
  public:
    Point(Eigen::Vector2d ipt, int frame_idx, Eigen::Vector4f color=Eigen::Vector4f(1.0, 1.0, 1.0, 0.5))
        : img_pt(ipt), frame_idx(frame_idx), color(color) {}

    void load(float half_point_width, float half_point_height);
    void draw();

    Eigen::Vector2d getGLPt() { return gl_pt; }
    int getFrameIndex() { return frame_idx; }

  private:
    void updateGLPt();
    
  private:
    const Eigen::Vector2d img_pt; // point location (image coordinate)
    Eigen::Vector2d gl_pt; // point location (gl window coordinate)
    int frame_idx;
    Eigen::Vector4f color;
    GLuint vao, vbo;
    int vertices = 4;
};

class Points {
    /*
     * 点の情報を保持する
     */
  public:
    static void setShader();
    std::shared_ptr<Point> addPoint(Eigen::Vector2d pt, int frame_idx,
                    Eigen::Vector4f color=Eigen::Vector4f(1.0, 1.0, 1.0, 0.5));
    void load();
    void draw();
    
  private:
    inline static const std::string shader_basename = "point";
    std::vector<std::shared_ptr<Point>> pts;
    Eigen::Vector2d point_size = Eigen::Vector2d(10, 10), norm_point_size;
  public:
    inline static GLuint program_id;
};

}

#endif // POINT_HPP__
