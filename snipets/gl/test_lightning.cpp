#include <iostream>
#include "gl/lightning.hpp"


int main() {
    gl::LightningGL window = gl::LightningGL();

    // window.set_shader("/home/kitamura/work/opencv/src/gl/shader/lightning.vert",
    //                   "/home/kitamura/work/opencv/src/gl/shader/lightning.frag");
    window.setShader("/home/kitamura/work/ImageProcessing/src/gl/shader/lightning.vert",
                      "/home/kitamura/work/ImageProcessing/src/gl/shader/lightning.frag");
    
    for (int i = 0; i < 200; i++) {
        // std::cout << i << std::endl;
        window.draw();
    }
}
