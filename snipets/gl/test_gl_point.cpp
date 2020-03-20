/*
 * test_gl_point.cpp
 *
 * Create Date : 2020-03-20 19:58:12
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */
#include <iostream>
#include <glog/logging.h>
#include <gperftools/profiler.h>
#include <boost/filesystem.hpp>

#include "gl/base.hpp"
#include "fmt/format.h"

namespace fs = boost::filesystem;

int main(int argc, char ** argv) {
    // logging codes
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();

    fs::path save_dir("./prof");
    if (!fs::exists(save_dir)) {
        fs::create_directories(save_dir);
    }
    auto save_path = (save_dir / fs::path(fs::basename(fs::path(argv[0])) + ".prof")).generic_string();
    ProfilerStart(save_path.c_str());
    std::cout << "Profiling to " << save_path << std::endl;
    // end logging codes

    std::string filename = "/home/kitamura/work/ImageProcessing/data/img/sample01/sample01_0.png";
    cv::Mat mat = cv::imread(filename);
    fmt::print("image size (rows x cols) = {} x {}\n", mat.rows, mat.cols);
    
    auto window = gl::BaseGL::getInstance();
    auto debug_image = window.addFrame(mat);
    Eigen::Vector2d pt0(mat.cols, mat.rows);
    std::cout << "pt0 : " << pt0.transpose() << std::endl;;
    debug_image->addPoint(pt0);
    debug_image->addPoint(Eigen::Vector2d(0, 100), Eigen::Vector4f(1.0, 0.0, 0.0, 0.5));
    debug_image->addPoint(Eigen::Vector2d(100, 100), Eigen::Vector4f(1.0, 1.0, 0.0, 0.5));

    auto gl_pt = window.plugin->imageCoord2GLCoord(pt0);
    auto img_pt = window.plugin->glCoord2ImageCoord(gl_pt);

    std::cout << gl_pt.transpose() << std::endl;
    std::cout << img_pt.transpose() << std::endl;

    window.draw();
    ProfilerStop();
}
