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

#include "gl/simple.hpp"
#include "debug_util/utility.hpp"
#include "halide/halide_sobel.h"


std::pair<cv::Mat, cv::Mat> sobelFilter(cv::Mat &src) {
    Halide::Runtime::Buffer<uint8_t> input(src.ptr(), src.cols, src.rows, src.channels());
    Halide::Runtime::Buffer<float> output_x(src.cols, src.rows);
    Halide::Runtime::Buffer<float> output_y(src.cols, src.rows);
    halide_sobel(input, output_x, output_y);

    cv::Mat sobel_x(src.size(), CV_32FC1);
    cv::Mat sobel_y(src.size(), CV_32FC1);
    memcpy(sobel_x.ptr(), output_x.data(), src.cols * src.rows * sizeof(float));
    memcpy(sobel_y.ptr(), output_y.data(), src.cols * src.rows * sizeof(float));
    
    return std::make_pair(sobel_x, sobel_y);
}

int main(int argc, char ** argv) {
    util::startLogging(argc, argv, __FILE__);

    std::string filename =
        (fs::path(__FILE__).parent_path() / fs::path("../../data/img/Lenna.png")).generic_string();
    
    cmdline::parser parser;
    parser.add<std::string>("src", 's', "src image filename", false, filename);

    filename = parser.get<std::string>("src");

    cv::Mat mat = cv::imread(filename);
    std::pair<cv::Mat, cv::Mat> sobel = sobelFilter(mat);

    gl::SimpleGL window = gl::SimpleGL();

    window.addFrame(mat);
    window.addFrame(sobel.first);
    window.addFrame(sobel.second);
    window.draw();

    util::stopLogging();
}
