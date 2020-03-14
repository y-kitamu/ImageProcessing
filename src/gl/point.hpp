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
    Point(Eigen::Vector2d ipt) : img_pt(ipt) {}

    void updateGLPt();
    void load(float half_point_width, float half_point_height);
    void draw();

  private:
    const Eigen::Vector2d img_pt; // point location (image coordinate)
    Eigen::Vector2d gl_pt; // point location (gl window coordinate)
    Eigen::Vector4f color = Eigen::Vector4f(1.0, 1.0, 1.0, 0.5);
    GLuint vao, vbo;
    int vertices = 4;
};

class Points {
    /*
     * 点の情報を保持する
     */
  public:
    Points();
    void addPoint(Eigen::Vector2d pt);
    void load();
    void draw();
    void updateGLPts();
    
  private:
    const std::string shader_basename = "point";
    const fs::path shader_dir = fs::path(__FILE__).parent_path() / fs::path("shader");
    std::vector<Point> pts;
    GLuint program_id;
    Eigen::Vector2d point_size = Eigen::Vector2d(10, 10), norm_point_size;
};

}

#endif // POINT_HPP__
