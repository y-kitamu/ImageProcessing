/*
 * point.cpp
 *
 * Create Date : 2020-03-14 14:55:32
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */
#include "base.hpp"
#include "point.hpp"

namespace gl {

void Point::updateGLPt() {
    gl_pt = BaseGL::plugin->imageCoord2GLCoord(img_pt);
    gl_pt.x() = 2 * gl_pt.x() * BaseGL::width_inv - 1;
    gl_pt.y() = 1 - 2 * gl_pt.y() * BaseGL::height_inv;
}

void Point::load(float half_point_width, float half_point_height) {
    updateGLPt();
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

void Point::draw() {
    glUseProgram(Points::program_id);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices);
}


void Points::setShader() {
    program_id = gl::setShader(
        (BaseGL::shader_dir / fs::path(shader_basename + ".vert")).generic_string(),
        (BaseGL::shader_dir / fs::path(shader_basename + ".frag")).generic_string());
}

std::shared_ptr<Point> Points::addPoint(Eigen::Vector2d pt, int frame_idx, Eigen::Vector4f color) {
    pts.emplace_back(std::make_shared<Point>(pt, frame_idx, color));
    return pts.back();
}

void Points::load() {
    norm_point_size = Eigen::Vector2d(
        point_size.x() * BaseGL::width_inv, point_size.y() * BaseGL::height_inv);
    for (auto && pt : pts) {
        pt->load(norm_point_size.x(), norm_point_size.y());
    }
}

void Points::draw() {
    glUseProgram(program_id);
    for (auto && pt : pts) {
        pt->draw();
    }
    glUseProgram(0);
}

} // namespace gl
