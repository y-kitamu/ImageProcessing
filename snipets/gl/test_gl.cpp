#include <unistd.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "gl/simple.hpp"


int main() {

    // std::string vert_shader = "/home/kitamura/work/opencv/src/gl/shader/simple_texture.vert";
    // std::string frag_shader = "/home/kitamura/work/opencv/src/gl/shader/simple_texture.frag";
    std::string vert_shader = "/home/kitamura/work/ImageProcessing/src/gl/shader/simple_texture.vert";
    std::string frag_shader = "/home/kitamura/work/ImageProcessing/src/gl/shader/simple_texture.frag";
    
    gl::SimpleGL window = gl::SimpleGL();
    window.set_shader(vert_shader, frag_shader);

    // cv::Mat img = cv::imread("/home/kitamura/work/opencv/data/img/Lenna.png");
    cv::Mat img = cv::imread("/home/kitamura/work/ImageProcessing/data/lena.jpg");
    // std::cout << img.size << std::endl;
    // cv::flip(img, img, 0);

    window.set_frame(img);

    window.draw();
    
}
