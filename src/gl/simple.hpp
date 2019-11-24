#ifndef __GL_SIMPLE_HPP__
#define __GL_SIMPLE_HPP__

#include "base.hpp"
#include <Eigen/Eigen>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>

#include "line.hpp"

namespace fs = boost::filesystem;

namespace gl {
class SimpleGL : public BaseGL {
    /*
     * 画像を一枚表示する class
     */
    // TODO : refactor function name. From snake_case to camelCase
  public:
    SimpleGL() {
        // BaseGL (基底クラス)のコンストラクタのあとに呼び出し
        glfwSetScrollCallback(img_window, scrollCallback);
        glfwSetMouseButtonCallback(img_window, mouseCallback);
        glfwSetCursorPosCallback(img_window, cursorCallback);
        setShader((fs::path(__FILE__).parent_path() / fs::path("./shader/simple_texture.vert")).generic_string(),
                   (fs::path(__FILE__).parent_path() / fs::path("./shader/simple_texture.frag")).generic_string());
    }
    
    void set_frame(const cv::Mat &mat) {
        frames.emplace_back(mat);
        setTextureFormat();
    }

  private:
    // load_gl_object, draw_gl, draw_imgui, check_keyboard_and_mouse_input は draw の中で呼び出される
    void loadGLObjects() override;
    void drawGL() override;
    void drawImgui() override;
    void checkKeyboardAndMouseInput() override;

    void setTextureFormat();

    Eigen::Vector2d imageCoord2GLCoord(Eigen::Vector2d img_pt);
    Eigen::Vector2d glCoord2ImageCoord(Eigen::Vector2d gl_pt);
    
    static void scrollCallback(GLFWwindow * window, double xoffset, double yoffset);
    static void mouseCallback(GLFWwindow *window, int button, int action, int mods);
    static void cursorCallback(GLFWwindow * window, double xpos, double ypos);
    
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
};

} // namespace gl


#endif //__GL_SIMPLE_HPP__
