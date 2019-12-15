#ifndef GL_SIMPLE_HPP__
#define GL_SIMPLE_HPP__

#include <stdio.h>

#include <Eigen/Eigen>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>

#include "base.hpp"
#include "image.hpp"
#include "line.hpp"

namespace fs = boost::filesystem;

namespace gl {
class SimpleGL : public SingletonBaseGL<SimpleGL> {
    /*
     * 画像を一枚表示する class
     *
     * TODO : Interface を整理する。わかりやすくする (他の oss などを参考にして関数名など refactor する)
     */
    friend class SingletonBaseGL<SimpleGL>; // getInstance が private の SimpleGL にアクセスするため
  private:
    SimpleGL() {
        // BaseGL (基底クラス)のコンストラクタのあとに呼び出し
        glfwSetScrollCallback(img_window, scrollCallback);
        glfwSetMouseButtonCallback(img_window, mouseCallback);
        glfwSetCursorPosCallback(img_window, cursorCallback);
    }
    
  public:
    std::shared_ptr<Image> addFrame(const cv::Mat &mat) {
        auto frame = std::make_shared<Image>(mat);
        frames.emplace_back(frame);
        return frame;
    }
    
    static int getImageWidth() { return image_width; }
    static int getImageHeight() { return image_height; }
    static float getScale() { return scale; }
    static float getOffsetX() { return offset_x; }
    static float getOffsetY() { return offset_y; }
    
    static Eigen::Vector2d imageCoord2GLCoord(Eigen::Vector2d img_pt);
    static Eigen::Vector2d glCoord2ImageCoord(Eigen::Vector2d gl_pt);
    static bool isPointInImage(double x, double y);

  private:
    // draw() の最初に呼び出される
    void initDraw() override {
        setTexture();
    }
    // load_gl_object, draw_gl, draw_imgui, check_keyboard_and_mouse_input は draw の中で呼び出される
    void loadGLObjects() override;
    void drawGL() override;
    void drawImgui() override;
    void checkKeyboardAndMouseInput() override;

    void setTexture();

    static void scrollCallback(GLFWwindow * window, double xoffset, double yoffset);
    static void mouseCallback(GLFWwindow *window, int button, int action, int mods);
    static void cursorCallback(GLFWwindow * window, double xpos, double ypos);

  private:
    static constexpr float mouse_scroll_scale = 0.10;
    inline static double prev_xpos = 0.0, prev_ypos = 0.0, xpos = 0.0, ypos = 0.0; // mouse position
    inline static bool is_left_button_pressed = false, is_pressed_in_image = false;

    // variable about selected image
    inline static int image_width = 0, image_height = 0;
    inline static float scale = 1.0f;
    inline static float offset_x = 0.0f, offset_y = 0.0f;
    
    std::vector<std::shared_ptr<Image>> frames;
    int frame_idx = 0;  // 表示する画像の index
    Eigen::Vector2d cursor_img_pt;
};

} // namespace gl


#endif //GL_SIMPLE_HPP__
