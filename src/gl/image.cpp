/*
 * image.cpp
 *
 * Create Date : 2019-12-15 10:30:27
 * Copyright (c) 2019 Yusuke Kitamura <ymyk6602@gmail.com>
 */
#include "base.hpp"
#include "image.hpp"
#include "plugin_base.hpp"
#include <fmt/format.h>

namespace gl {

Image::Image(const cv::Mat &img) : image(img) {
    if (image.type() == CV_8UC1) {
        texture_format = GL_RED;
        texture_internal_format = GL_RGB;
        swizzle_mask[0] = GL_RED;
        swizzle_mask[1] = GL_RED;
        swizzle_mask[2] = GL_RED;
        swizzle_mask[3] = GL_ZERO;
        pixel_format = GL_UNSIGNED_BYTE;
    } else if (image.type() == CV_8UC3) {
        texture_format = GL_BGR;
        texture_internal_format = GL_RGB;
        swizzle_mask[0] = GL_RED;
        swizzle_mask[1] = GL_GREEN;
        swizzle_mask[2] = GL_BLUE;
        swizzle_mask[3] = GL_ZERO;
        pixel_format = GL_UNSIGNED_BYTE;
    } else if (image.type() == CV_32FC1) {
        texture_format = GL_RED;
        texture_internal_format = GL_R32F;
        swizzle_mask[0] = GL_RED;
        swizzle_mask[1] = GL_RED;
        swizzle_mask[2] = GL_RED;
        swizzle_mask[3] = GL_ZERO;
        pixel_format = GL_FLOAT;
    }

    image_width = image.cols;
    image_height = image.rows;
}

void Image::setShader() {
    points.setShader();
}

void Image::setTexture() {
    // constructor に統一できる? (その時のメモリサイズは大丈夫?)
    // テクスチャ
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // GL_TEXTURE_2D の場合 GL_LINEAR_MIPMAP_LINEAR だと真っ黒になった
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);

    glTexImage2D(GL_TEXTURE_2D, 0, texture_internal_format, image_width,
                 image_height, 0, texture_format, pixel_format, image.ptr());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Image::load() {
    /*
     * フレームごとに変化する可能性のある gl object をロードする関数
     */
    // 頂点データを準備
    // 頂点バッファオブジェクト (VBO, cpu 側のオブジェクト) を直接描画に指定することはできません.
    // 描画に指定できるのは, 頂点バッファオブジェクトを組み込んだ頂点配列オブジェクト (VAO, gpu側のオブジェクト) だけです.

    float aspect_ratio = (float)image_height / image_width * BaseGL::view_width / BaseGL::view_height;
    GLfloat position[4][4] = {
        // [x, y, u, v]
        {-0.5f * scale + offset_x, -0.5f * aspect_ratio * scale + offset_y, 0.0f, 1.0f},
        { 0.5f * scale + offset_x, -0.5f * aspect_ratio * scale + offset_y, 1.0f, 1.0f},
        { 0.5f * scale + offset_x,  0.5f * aspect_ratio * scale + offset_y, 1.0f, 0.0f},
        {-0.5f * scale + offset_x,  0.5f * aspect_ratio * scale + offset_y, 0.0f, 0.0f},
    };
    vertices = sizeof(position) / sizeof(position[0]);

    // VAO (gpu 側のオブジェクト)を作成、有効にする
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VBO (cpu 側のオブジェクト)に頂点情報を格納 
    glGenBuffers(1, &vbo);  // バッファ作成
    glBindBuffer(GL_ARRAY_BUFFER, vbo);  // GL コンテキストに vbo をBL_ARRAY_BUFFER でバインド
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);  // 実データを格納

    // 位置属性を 「0」にバインド（シェーダー側で後で参照）
    int index = 0, size = 2, stride = sizeof(position[0]);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, 0);  // shader の attrib 属性に渡すデータの指定
    glEnableVertexAttribArray(index);
    index = 1;
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (GLvoid *)(size * sizeof(GLfloat)));
    glEnableVertexAttribArray(index);

    points.load();
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // vbo の bind を解除
    glBindVertexArray(0);  // vao の bind を解除
}

void Image::draw() {
    // テクスチャユニットとテクスチャの指定
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // 描画に使う頂点配列オブジェクトの指定
    glBindVertexArray(vao);

    // シェーダプログラムの指定
    glUseProgram(PluginBase::shader_program_id);
    // 図形の描画
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices);
    
    points.draw();
    
    // bind を解除
    glBindVertexArray(0);
    glUseProgram(0);
}

std::shared_ptr<Point> Image::addPoint(Eigen::Vector2d pt, Eigen::Vector4f color) {
    int frame_idx = 0;
    for (auto && ptr : BaseGL::frames) {
        if (ptr.get() == this) {
            break;
        }
        frame_idx++;
    }
    return points.addPoint(pt, frame_idx, color);
}

}
