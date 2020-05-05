/*
 * test_stdlib.cpp
 *
 * Create Date : 2020-05-04 14:51:07
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */
#include <cstdio>
#include <iostream>
#include <glog/logging.h>
#include <gperftools/profiler.h>
#include <boost/filesystem.hpp>

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

#ifdef _LIBCPP_VERSION
    std::printf("libc++: %d\n", _LIBCPP_VERSION);
#endif

#ifdef __GLIBCXX__
    std::printf("GLIBCXX: %d\n", __GLIBCXX__);
#endif

    ProfilerStop();
}
