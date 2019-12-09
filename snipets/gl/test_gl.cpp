#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cmdline/cmdline.h>
#include <opencv2/opencv.hpp>
#include <fmt/format.h>
#include <boost/filesystem.hpp>

#include "gl/simple.hpp"
#include "debug_util/utility.hpp"

namespace fs = boost::filesystem;

int main(int argc, char ** argv) {
    // 環境によってはうまく動作しない？
    // 正方形の画像はうまく表示されたけど、長方形だと変になる
    // gpu driver によって違う？

    util::startLogging(argc, argv, __FILE__);
    
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

    window.addFrame(img);
    window.addFrame(img);
    window.draw();

    util::stopLogging();
}
