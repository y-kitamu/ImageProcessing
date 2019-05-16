#include <iostream>
#include <opencv2/opencv.hpp>
#include "gl/gl.hpp"


int main() {

    std::string vert_shader = "gl/shader/simple_texture.vert";
    std::string frag_shader = "gl/shader/simple_texture.frag";
    
    gl::SimpleGL window = gl::SimpleGL();
    window.set_shader(vert_shader, frag_shader);

    cv::Mat img = cv::imread("../../data/img/Lenna.png");

    // while (1) {
    for (int i = 0; i < 1000;i ++) {
        window.draw(img);
    }
    
}
