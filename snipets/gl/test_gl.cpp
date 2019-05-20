#include <unistd.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "gl/simple.hpp"


int main() {

    std::string vert_shader = "/home/kitamura/work/opencv/src/gl/shader/simple_texture.vert";
    std::string frag_shader = "/home/kitamura/work/opencv/src/gl/shader/simple_texture.frag";
    
    gl::SimpleGL window = gl::SimpleGL();
    std::cout << "end init" << std::endl;
    window.set_shader(vert_shader, frag_shader);
    std::cout << "end init" << std::endl;

    cv::Mat img;
    // cv::Mat img = cv::imread("/home/kitamura/work/opencv/data/img/Lenna.png");
    // std::cout << img.size << std::endl;
    // cv::flip(img, img, 0);

    // for (int i = 0; i < 100; i++) {
    while (true) {
        // std::cout << i << std::endl;
        window.draw(img);
    }
    
}
