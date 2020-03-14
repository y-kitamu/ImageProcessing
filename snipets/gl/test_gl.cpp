#include <unistd.h>
#include <random>
#include <iostream>
#include <fstream>
#include <cmdline/cmdline.h>
#include <opencv2/opencv.hpp>
#include <fmt/format.h>
#include <boost/filesystem.hpp>
#include <Eigen/Eigen>

#include "gl/base.hpp"
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
    
    // gl::SimpleGL window = gl::SimpleGL();
    gl::BaseGL window = gl::BaseGL::getInstance();

    auto debug_image0 = window.addFrame(img);
    auto debug_image1 = window.addFrame(img);

    std::random_device seed;
    std::mt19937 engine(seed());
    std::uniform_real_distribution<> dist(0.0, 512.0);
    for (int i = 0; i < 512; i++) {
        double x = dist(engine), y = dist(engine);
        debug_image0->points.addPoint(Eigen::Vector2d(x, y));
    }
    
    window.draw();
    gl::BaseGL::destroy();

    util::stopLogging();
}
