#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cmdline/cmdline.h>
#include <opencv2/opencv.hpp>
#include <fmt/format.h>
#include <boost/filesystem.hpp>

#include "gl/simple.hpp"

namespace fs = boost::filesystem;

int main(int argc, char ** argv) {

    cmdline::parser parser;
    parser.add<std::string>("src", 's', "src image filename", false, "/home/kitamura/work/ImageProcessing/data/lena.jpg");
    parser.add<std::string>("shader", 'd', "shader directory", false, "/home/kitamura/work/ImageProcessing/src/gl/shader/");
    parser.parse_check(argc, argv);
    
    // std::string vert_shader = "/home/kitamura/work/opencv/src/gl/shader/simple_texture.vert";
    // std::string frag_shader = "/home/kitamura/work/opencv/src/gl/shader/simple_texture.frag";
    
    // std::string vert_shader = parser.get<std::string>("shader") + "simple_texture.vert";
    // std::string frag_shader = parser.get<std::string>("shader") + "simple_texture.frag";

    // auto check_shader_exists = [] (std::string shader_filename) {
    //     std::ifstream ifs(shader_filename);
    //     return ifs.is_open();
    // };

    // if (!check_shader_exists(vert_shader) || !check_shader_exists(frag_shader)) {
    //     fmt::print("shader file does not exist\n");
    //     return -1;
    // }
        
    gl::SimpleGL window = gl::SimpleGL();
    // window.set_shader(vert_shader, frag_shader);

    // cv::Mat img = cv::imread("/home/kitamura/work/opencv/data/img/Lenna.png");
    // cv::Mat img = cv::imread(parser.get<std::string>("src"));
    std::string filename = (fs::path(__FILE__).parent_path() / fs::path("../../data/img/Lenna.png")).generic_string();
    fmt::print("filename : {}\n", filename);
    cv::Mat img = cv::imread(filename);
    // std::cout << img.size << std::endl;
    // cv::flip(img, img, 0);

    window.set_frame(img);
    window.draw();
}
