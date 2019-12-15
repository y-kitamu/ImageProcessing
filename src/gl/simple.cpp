#include "simple.hpp"
#include <fmt/format.h>


namespace gl {

void SimpleGL::loadGLObjects() {
    frames[frame_idx]->updateParams(image_width, image_height, scale, offset_x, offset_y);
    frames[frame_idx]->load(width, height);
}

void SimpleGL::drawGL() {
    frames[frame_idx]->draw();
}

void SimpleGL::drawImgui() {
    // TODO : base.cpp に移す
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        // menu bar (一番上のバー)
        // ファイルのセーブ(未実装)・ロードなど
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Save")) {
                    
                }
                if (ImGui::MenuItem("Load")) {
                    
                }
                if (ImGui::MenuItem("Open file")) {
                    char c_filename[1024];
                    FILE *fp = popen("zenity --file-selection", "r");
                    fgets(c_filename, 1024, fp);
                    pclose(fp);
                    std::string filename(c_filename);
                    filename.erase(std::remove(filename.begin(), filename.end(), '\n'), filename.end());
                    cv::Mat img = cv::imread(filename);
                    if (img.data == nullptr) {
                        fmt::print("{} is not a valid image file.\n", filename);
                    } else {
                        addFrame(img);
                    }
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Style")) {
                auto themes = imgui_theme.getAllThemeNames();
                for (auto && theme : themes) {
                    if (ImGui::MenuItem(theme.c_str())) {
                        imgui_theme.setCurrentTheme(theme);
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
    
    {
        ImGui::Begin("frames");
        bool is_changed = false;
        for (int i = 0; i < frames.size(); i++) {
            std::stringstream ss;
            ss << i + 1;
            is_changed |= ImGui::RadioButton(ss.str().c_str(), &frame_idx, i);
        }
        ImGui::End();
        if (is_changed) {
            setTexture();
        }
    }

    {
        cursor_img_pt = glCoord2ImageCoord(Eigen::Vector2d(xpos, ypos));
        ImGui::SetNextWindowPos(
            ImVec2(0, ImGui::GetIO().DisplaySize.y - ImGui::GetTextLineHeightWithSpacing() * 2), 0);
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 0), 0);
        if (ImGui::Begin("", nullptr, (ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                       ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar))) {
            ImGui::Text("(x, y) = (%.1f, %.1f)", cursor_img_pt.x(), cursor_img_pt.y());
        }
    }
    
    ImGui::Render();
}

void SimpleGL::checkKeyboardAndMouseInput() {
    BaseGL::checkKeyboardAndMouseInput();
}

void SimpleGL::scrollCallback(GLFWwindow * window, double xoffset, double yoffset) {
    scale += mouse_scroll_scale * yoffset;
    if (scale < 0.3) {
        scale = 0.3;
    }
}

void SimpleGL::mouseCallback(GLFWwindow * window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        is_left_button_pressed = true;
        is_pressed_in_image = isPointInImage(xpos, ypos);
        glfwGetCursorPos(window, &prev_xpos, &prev_ypos);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        is_left_button_pressed = false;
        is_pressed_in_image = false;
    }
}

void SimpleGL::cursorCallback(GLFWwindow * window, double x, double y) {
    xpos = x; ypos = y;
    if (is_left_button_pressed && is_pressed_in_image && !ImGui::IsAnyItemActive()) {
        offset_x += 2 * (xpos - prev_xpos) * width_inv;
        offset_y -= 2 * (ypos - prev_ypos) * height_inv;
        prev_xpos = xpos; prev_ypos = ypos;
    }
}

void SimpleGL::setTexture() {
    frames[frame_idx]->setTexture();
    image_width = frames[frame_idx]->getImageWidth();
    image_height = frames[frame_idx]->getImageHeight();
    scale = frames[frame_idx]->getScale();
    offset_x = frames[frame_idx]->getOffsetX();
    offset_y = frames[frame_idx]->getOffsetY();
}


Eigen::Vector2d SimpleGL::imageCoord2GLCoord(Eigen::Vector2d img_pt) {
    float aspect_ratio = 1.0f * image_height / image_width * width / height;
    Eigen::Vector2d gl_pt;

    gl_pt.x() = (img_pt.x() * scale / image_width + 1.0f + offset_x - scale * 0.5) / (2 * width_inv);
    gl_pt.y() = (img_pt.y() * scale * width / (image_width * height)
                 + 1.0f - offset_y - scale * 0.5 * aspect_ratio) / (2 * height_inv);
    return gl_pt;
}

Eigen::Vector2d SimpleGL::glCoord2ImageCoord(Eigen::Vector2d gl_pt) {
    /*
     * window の coordinate (左上(0, 0)、右下(window_width, window_height)) から
     * Image coordinate (画像左上(0, 0), 画像右下(image_width, image_height))
     */
    double aspect_ratio = 1.0 * image_height  * width / (image_width * height);
    Eigen::Vector2d img_pt;
    
    img_pt.x() = (2.0 * gl_pt.x() * width_inv - 1.0 - offset_x + scale * 0.5) * image_width / scale;
    img_pt.y() = (2.0 * gl_pt.y() * height_inv - 1.0 + offset_y + scale * 0.5 * aspect_ratio)
        * height / width * image_width / scale;

    return img_pt;
}

bool SimpleGL::isPointInImage(double x, double y) {
    auto ipt = glCoord2ImageCoord(Eigen::Vector2d(x, y));
    return 0 < ipt.x() && ipt.x() < image_width && 0 < ipt.y() && ipt.y() < image_height;
}

} // namespace gl
