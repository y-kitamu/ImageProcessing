/*
 * test_slam.cpp
 *
 * Create Date : 2020-05-04 12:19:37
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */
#include <iostream>
#include <functional>
#include <glog/logging.h>
#include <gperftools/profiler.h>
#include <boost/filesystem.hpp>

#include <slam/main_loop.hpp>
#include <gl/base.hpp>

// for type debugging
template<class T>
class TD;


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

    auto window = gl::BaseGL::getInstance();

    fs::path project_root = fs::path(__FILE__).parent_path() / fs::path("../../");

    fs::path video_path = project_root / fs::path("data/video/mov_hts-samp007.mp4");
    slam::MainLoop looper1(video_path.generic_string());
    looper1.run();

    window.draw();

    ProfilerStop();
}
