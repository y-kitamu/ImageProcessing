/*
 * test_orb.cpp
 *
 * Create Date : 2020-03-20 11:46:16
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */
#include <iostream>
#include <glog/logging.h>
#include <gperftools/profiler.h>
#include <boost/filesystem.hpp>

#include <opencv2/opencv.hpp>
#include <Eigen/Eigen>
#include <fmt/format.h>

#include "gl/base.hpp"

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

    std::string filename;
    if (argc != 2) {
        fmt::print("Usage :  \n    ./<path>/<to>/feature_test_orb <image filename>\n");
        filename = (fs::absolute(fs::path(__FILE__).parent_path()) /
                    fs::path("../../data/img/Lenna.png")).generic_string();
    } else {
        filename = argv[1];
    }
    cv::Mat mat = cv::imread(filename);

    cv::Ptr<cv::Feature2D> ptr =
        cv::ORB::create(500, 1.2f, 8, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31, 20);
    std::vector<cv::KeyPoint> kpts;
    cv::Mat descriptors;
    ptr->detect(mat, kpts);
    ptr->compute(mat, kpts, descriptors);

    auto window = gl::BaseGL::getInstance();
    std::shared_ptr<gl::Image> debug_image = window.addFrame(mat);

    for (auto && kpt : kpts) {
        Eigen::Vector2d pt(kpt.pt.x, kpt.pt.y);
        debug_image->addPoint(pt);
    }

    window.draw();
    
    ProfilerStop();
}
