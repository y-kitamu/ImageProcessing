#ifndef __GL_SIMPLE_HPP__
#define __GL_SIMPLE_HPP__

#include "base.hpp"
#include <opencv2/opencv.hpp>

namespace gl {
class SimpleGL : public BaseGL {
    /*
     * 画像を一枚表示する class
     */
  public:
    void set_frame(const cv::Mat &mat) {
        frame = mat;
        load_gl_objects(); // virtual : open gl objects の作成
    }

  private:
    void draw_gl() override;
    void draw_imgui() override;
    void load_gl_objects() override;
    
  private:
    GLuint image;
    GLuint vao, vbo;
    
    cv::Mat frame;
    int vertices;
    
    bool demo_window, another_window;
};

} // namespace gl


#endif //__GL_SIMPLE_HPP__
