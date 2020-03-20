/*
 * test_halide.cpp
 *
 * Create Date : 2019-11-28 22:32:39
 * Copyright (c) 2019 Yusuke Kitamura <ymyk6602@gmail.com>
 */
#include <map>

#include <opencv2/opencv.hpp>
#include <fmt/format.h>
#include <cmdline/cmdline.h>
#include <halide/HalideBuffer.h>  // #include <halide/Halide.h> だとコンパイルエラーになった ???
#include <Eigen/Eigen>

#include "gl/base.hpp"
#include "halide/halide_sobel.h"
#include "halide/halide_normalize.h"


std::pair<cv::Mat, cv::Mat> sobelFilter(cv::Mat &src) {
    auto& window = gl::BaseGL::getInstance();
    Halide::Runtime::Buffer<uint8_t> input =
        Halide::Runtime::Buffer<uint8_t>::make_interleaved(src.ptr(), src.cols, src.rows, src.channels());
    Halide::Runtime::Buffer<float> output_gray(src.cols, src.rows);
    Halide::Runtime::Buffer<float> output_x(src.cols, src.rows);
    Halide::Runtime::Buffer<float> output_y(src.cols, src.rows);
    halide_sobel(input, output_gray, output_x, output_y);

    cv::Mat gray(src.size(), CV_32FC1);
    cv::Mat sobel_x(src.size(), CV_32FC1);
    cv::Mat sobel_y(src.size(), CV_32FC1);
    memcpy(gray.ptr(), output_gray.data(), src.cols * src.rows * sizeof(float));
    memcpy(sobel_x.ptr(), output_x.data(), src.cols * src.rows * sizeof(float));
    memcpy(sobel_y.ptr(), output_y.data(), src.cols * src.rows * sizeof(float));

    window.addFrame(gray);
    
    return std::make_pair(sobel_x, sobel_y);
}

cv::Mat normalize(cv::Mat &src) {
    float * ptr = src.ptr<float>();
    int width = src.cols, height = src.rows;
    float maximum = ptr[0], minimum = ptr[0];

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++, ptr++) {
            maximum = std::max(maximum, ptr[0]);
            minimum = std::min(minimum, ptr[0]);
        }
    }
    
    Halide::Runtime::Buffer<float> input(src.ptr<float>(), width, height);
    Halide::Runtime::Buffer<float> output(width, height);
    halide_normalize(input, maximum, minimum, output);

    cv::Mat normalized(src.size(), CV_32FC1);
    memcpy(normalized.ptr(), output.data(), width * height * sizeof(float));
    return normalized;
}

int main(int argc, char ** argv) {
    std::string filename =
        (fs::path(__FILE__).parent_path() / fs::path("../../data/img/Lenna.png")).generic_string();
    
    cmdline::parser parser;
    parser.add<std::string>("src", 's', "src image filename", false, filename);

    filename = parser.get<std::string>("src");

    cv::Mat mat = cv::imread(filename);
    std::pair<cv::Mat, cv::Mat> sobel = sobelFilter(mat);
    cv::Mat normalized_x = normalize(sobel.first);
    cv::Mat normalized_y = normalize(sobel.second);

    gl::BaseGL& window = gl::BaseGL::getInstance(); //gl::SimpleGL();

    window.addFrame(mat);
    window.addFrame(sobel.first);
    window.addFrame(sobel.second);
    window.addFrame(normalized_x);
    window.addFrame(normalized_y);
    window.draw();
    window.destroy();

}
