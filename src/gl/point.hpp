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

#include "base_include.hpp"
#include "gl_object.hpp"

namespace gl {
class Point : GLObject {
    /*
     * 点の情報を保持する
     */
  public:
    Point(double x, double y) : pt(x, y) {}
    ~Point() override = default;
    void load() override;
    void draw() override;
    
  private:
    Eigen::Vector2d pt;
};

}

#endif // POINT_HPP__
