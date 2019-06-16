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
    SimpleGL() {
        // BaseGL (基底クラス)のコンストラクタのあとに呼び出し
        glfwSetScrollCallback(img_window, scroll_callback);
    }
    
    void set_frame(const cv::Mat &mat) {
        frame = mat;
    }

  private:
    void draw_gl() override;
    void draw_imgui() override;
    void load_gl_objects() override;
    
    void check_keyboard_and_mouse_input() override;
    static void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
    
  private:
    static float scale, offset_x, offset_y;  // 頂点の位置に関する変数
    static constexpr float mouse_scroll_scale = 0.05;
    
    GLuint image;
    GLuint vao, vbo;
    
    cv::Mat frame;
    int vertices;  // 頂点の数

    bool demo_window, another_window;
};

} // namespace gl


#endif //__GL_SIMPLE_HPP__
