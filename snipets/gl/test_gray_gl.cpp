/*
 * test_gray_gl.cpp
 *
 * Create Date : 2019-12-09 23:29:16
 * Copyright (c) 2019 Yusuke Kitamura <ymyk6602@gmail.com>
 */
#include <opencv2/opencv.hpp>

#include "gl/base.hpp"

int main(int argc, char ** argv) {
    int img_size = 16;
    cv::Mat mat(img_size, img_size, CV_32FC1);
    cv::Mat mat_uint(img_size, img_size, CV_8UC1);
    
    float * ptr = mat.ptr<float>();
    uint8_t * u_ptr = mat_uint.ptr();
    for (int y = 0; y < img_size; y++) {
        for (int x = 0; x < img_size; x++, ptr++, u_ptr++) {
            ptr[0] = 1.0f * x * y / (img_size * img_size);
            u_ptr[0] = x * y * 255 / (img_size * img_size);
        }
    }
    
    auto& window = gl::BaseGL::getInstance();
    window.addFrame(mat);
    window.addFrame(mat_uint);
    window.draw();
    gl::BaseGL::destroy();
}

