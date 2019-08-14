#ifndef __GL_SIMPLE_HPP__
#define __GL_SIMPLE_HPP__

#include "base.hpp"
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>

#include <fmt/format.h>

namespace fs = boost::filesystem;

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
        set_shader((fs::path(__FILE__).parent_path() / fs::path("./shader/simple_texture.vert")).generic_string(),
                   (fs::path(__FILE__).parent_path() / fs::path("./shader/simple_texture.frag")).generic_string());
    }
    
    void set_frame(const cv::Mat &mat) {
        frames.emplace_back(mat);
        set_texture_format();
    }

    void set_texture_format() {
        if (frames[frame_idx].type() == CV_8UC1) {
            texture_format = GL_RED;
            swizzle_mask[0] = GL_RED;
            swizzle_mask[1] = GL_RED;
            swizzle_mask[2] = GL_RED;
            swizzle_mask[3] = GL_ZERO;
        } else if (frames[frame_idx].type() == CV_8UC3) {
            texture_format = GL_BGR;
            swizzle_mask[0] = GL_RED;
            swizzle_mask[1] = GL_GREEN;
            swizzle_mask[2] = GL_BLUE;
            swizzle_mask[3] = GL_ZERO;
        }
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
    
    std::vector<cv::Mat> frames;
    int frame_idx = 0;  // 表示する画像の index
    int vertices;  // 頂点の数
    GLenum texture_format = GL_BGR;
    GLint swizzle_mask[4] = {GL_RED, GL_GREEN, GL_BLUE, GL_ZERO};

    bool demo_window, another_window;
};

} // namespace gl


#endif //__GL_SIMPLE_HPP__
