#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cmdline/cmdline.h>
#include <opencv2/opencv.hpp>
#include <fmt/format.h>
#include <boost/filesystem.hpp>
#include <glog/logging.h>
#include <gperftools/profiler.h>

#include "gl/simple.hpp"

namespace fs = boost::filesystem;

int main(int argc, char ** argv) {
    // 環境によってはうまく動作しない？
    // 正方形の画像はうまく表示されたけど、長方形だと変になる
    // 画像サイズが大きいときに遅くなる
    // gpu driver によって違う？
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();

    fs::path prof_dir("./prof/");
    if (!fs::exists(prof_dir)) {
        fs::create_directory(prof_dir);
    }
    
    ProfilerStart(
        (prof_dir / fs::path(fs::basename(fs::path(__FILE__)) + ".prof")).generic_string().c_str());
    
    std::string filename =
        (fs::path(__FILE__).parent_path() / fs::path("../../data/img/Lenna.png")).generic_string();
    std::string shader_dir =
        (fs::path(__FILE__).parent_path() / fs::path("../src/gl/shader")).generic_string();
    
    cmdline::parser parser;
    parser.add<std::string>("src", 's', "src image filename", false, filename);
    parser.add<std::string>("shader", 'd', "shader directory", false, shader_dir);
    parser.parse_check(argc, argv);

    filename = parser.get<std::string>("src");
    shader_dir = parser.get<std::string>("shader");
    
    cv::Mat img = cv::imread(filename);
    
    fmt::print("filename : {}, shader_dir : {}\n", filename, shader_dir);
    fmt::print("image (width x height) : {} x {}\n", img.cols, img.rows);
    
    gl::SimpleGL window = gl::SimpleGL();

    window.set_frame(img);
    window.set_frame(img);
    window.draw();

    ProfilerStop();
}
