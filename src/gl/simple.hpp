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
        glfwSetMouseButtonCallback(img_window, mouse_callback);
        glfwSetCursorPosCallback(img_window, cursor_callback);
    }
    
    void set_frame(const cv::Mat &mat) {
        frame = mat;
    }

  private:
    // load_gl_object, draw_gl, draw_imgui, check_keyboard_and_mouse_input は draw の中で呼び出される
    void load_gl_objects() override;
    void draw_gl() override;
    void draw_imgui() override;
    void check_keyboard_and_mouse_input() override;
    
    static void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
    static void mouse_callback(GLFWwindow *window, int button, int action, int mods);
    static void cursor_callback(GLFWwindow * window, double xpos, double ypos);
    
  private:
    static float scale, offset_x, offset_y;  // 頂点の位置に関する変数
    static constexpr float mouse_scroll_scale = 0.05;
    static double prev_xpos, prev_ypos, xpos, ypos;
    static bool is_left_button_pressed;
    
    GLuint image;
    GLuint vao, vbo;
    
    cv::Mat frame;
    int vertices;  // 頂点の数

    bool demo_window, another_window;
};

} // namespace gl


#endif //__GL_SIMPLE_HPP__
