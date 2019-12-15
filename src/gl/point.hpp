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
#include "gl_object.hpp"

namespace fs = boost::filesystem;

namespace gl {

class Point {
    /*
     * TODO : 高速化
     */
  public:
    Point(Eigen::Vector2d ipt) : img_pt(ipt) {}
    template<class T>
    void updateGLPt() {
        gl_pt = T::imageCoord2GLCoord(img_pt);
        gl_pt.x() = 2 * gl_pt.x() * T::width_inv - 1;
        gl_pt.y() = 2 * gl_pt.y() * T::height_inv - 1;
    }

    void load(float half_point_width, float half_point_height) {
        float left = gl_pt.x() - half_point_width;
        float right = gl_pt.x() + half_point_width;
        float top = gl_pt.y() + half_point_height;
        float bottom = gl_pt.y() - half_point_height;
        GLfloat position[4][6] = {
            {left, top, color.x(), color.y(), color.z()},
            {left, bottom, color.x(), color.y(), color.z()},
            {right, bottom, color.x(), color.y(), color.z()},
            {right, top, color.x(), color.y(), color.z()},
        };

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // VBO (cpu 側のオブジェクト)に頂点情報を格納 
        glGenBuffers(1, &vbo);  // バッファ作成
        glBindBuffer(GL_ARRAY_BUFFER, vbo);  // GL コンテキストに vbo をBL_ARRAY_BUFFER でバインド
        glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);  // 実データを格納

        // 位置属性を 「0」にバインド（シェーダー側で後で参照）
        int pos_index = 0, pos_size = 2, stride = sizeof(position[0]);
        glVertexAttribPointer(pos_index, pos_size, GL_FLOAT, GL_FALSE, stride, 0);
        glEnableVertexAttribArray(pos_index);
        int color_index = 1, color_size = 4;
        glVertexAttribPointer(color_index, color_size, GL_FLOAT, GL_FALSE,
                              stride, (GLvoid *)(pos_size * sizeof(GLfloat)));
        glEnableVertexAttribArray(color_index);
    
        glBindBuffer(GL_ARRAY_BUFFER, 0);  // vbo の bind を解除
        glBindVertexArray(0);  // vao の bind を解除
    }

    void draw() {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertices);
    }

  private:
    const Eigen::Vector2d img_pt;
    Eigen::Vector2d gl_pt;
    Eigen::Vector4f color = Eigen::Vector4f(1.0, 1.0, 1.0, 0.5);
    GLuint vao, vbo;
    int vertices = 4;
};

class Points {
    /*
     * 点の情報を保持する
     */
  public:
    Points(std::string shader_basename = "point",
           fs::path shader_dir = fs::path(__FILE__).parent_path() / fs::path("shader")) {
        program_id = setShader(
            (shader_dir / fs::path(shader_basename + ".vert")).generic_string(),
            (shader_dir / fs::path(shader_basename + ".frag")).generic_string());
    }
    
    void addPoint(Eigen::Vector2d pt) {
        pts.emplace_back(Point(pt));
    }

    void load() {
        for (auto && pt : pts) {
            pt.load(norm_point_size.x(), norm_point_size.y());
        }
    }
    
    void draw() {
        glUseProgram(program_id);
        for (auto && pt : pts) {
            pt.draw();
        }
        glUseProgram(0);
    }

    template<class T>
    void updateGLPts() {
        for (auto && pt : pts) {
            pt.updateGLPt<T>();
        }
        norm_point_size = Eigen::Vector2d(point_size.x() * T::width_inv, point_size.y() * T::height_inv);
    }
    
  private:
    std::vector<Point> pts;
    GLuint program_id;
    Eigen::Vector2d point_size = Eigen::Vector2d(10, 10), norm_point_size;
};

}

#endif // POINT_HPP__
