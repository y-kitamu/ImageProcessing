#include "base.hpp"
#include "plugin_base.hpp"
#include <fmt/format.h>


namespace gl {

PluginBase::PluginBase(std::string shader_basename, fs::path shader_dir) {
    glfwSetScrollCallback(BaseGL::img_window, scrollCallback);
    glfwSetMouseButtonCallback(BaseGL::img_window, mouseCallback);
    glfwSetCursorPosCallback(BaseGL::img_window, cursorCallback);

    shader_program_id = setShader(
        (shader_dir / fs::path(shader_basename + ".vert")).generic_string(),
        (shader_dir / fs::path(shader_basename + ".frag")).generic_string());
}

void PluginBase::loadGLObjects() {
    BaseGL::frames[frame_idx]->updateParams<PluginBase>();
    BaseGL::frames[frame_idx]->load();
}

void PluginBase::drawGL() {
    BaseGL::frames[frame_idx]->draw();
}

void PluginBase::drawImgui() {
    {
        ImGui::Begin("frames");
        bool is_changed = false;
        for (int i = 0; i < (int)BaseGL::frames.size(); i++) {
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

void PluginBase::scrollCallback(GLFWwindow * window, double xoffset, double yoffset) {
    scale += mouse_scroll_scale * yoffset;
    if (scale < 0.3) {
        scale = 0.3;
    }
}

void PluginBase::mouseCallback(GLFWwindow * window, int button, int action, int mods) {
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

void PluginBase::cursorCallback(GLFWwindow * window, double x, double y) {
    xpos = x; ypos = y;
    if (is_left_button_pressed && is_pressed_in_image && !ImGui::IsAnyItemActive()) {
        offset_x += 2 * (xpos - prev_xpos) * BaseGL::width_inv;
        offset_y -= 2 * (ypos - prev_ypos) * BaseGL::height_inv;
        prev_xpos = xpos; prev_ypos = ypos;
    }
}

void PluginBase::setTexture() {
    BaseGL::frames[frame_idx]->setTexture();
    image_width = BaseGL::frames[frame_idx]->getImageWidth();
    image_height = BaseGL::frames[frame_idx]->getImageHeight();
    scale = BaseGL::frames[frame_idx]->getScale();
    offset_x = BaseGL::frames[frame_idx]->getOffsetX();
    offset_y = BaseGL::frames[frame_idx]->getOffsetY();
}


Eigen::Vector2d PluginBase::imageCoord2GLCoord(Eigen::Vector2d img_pt) {
    float aspect_ratio = (float)image_height / image_width * BaseGL::width / BaseGL::height;
    Eigen::Vector2d gl_pt;

    gl_pt.x() = (img_pt.x() * scale / image_width + 1.0f + offset_x - scale * 0.5)
        / (2 * BaseGL::width_inv);
    gl_pt.y() = (img_pt.y() * scale * BaseGL::width / (image_width * BaseGL::height)
                 + 1.0f + offset_y - scale * 0.5 * aspect_ratio) / (2 * BaseGL::height_inv);
    return gl_pt;
}

Eigen::Vector2d PluginBase::glCoord2ImageCoord(Eigen::Vector2d gl_pt) {
    /*
     * window の coordinate (左上(0, 0)、右下(window_width, window_height)) から
     * Image coordinate (画像左上(0, 0), 画像右下(image_width, image_height))
     */
    double aspect_ratio = 1.0 * image_height  * BaseGL::width / (image_width * BaseGL::height);
    Eigen::Vector2d img_pt;
    
    img_pt.x() = (2.0 * gl_pt.x() * BaseGL::width_inv - 1.0 - offset_x + scale * 0.5)
        * image_width / scale;
    img_pt.y() = (2.0 * gl_pt.y() * BaseGL::height_inv - 1.0 + offset_y + scale * 0.5 * aspect_ratio)
        * BaseGL::height / BaseGL::width * image_width / scale;

    return img_pt;
}

bool PluginBase::isPointInImage(double x, double y) {
    auto ipt = glCoord2ImageCoord(Eigen::Vector2d(x, y));
    return 0 < ipt.x() && ipt.x() < image_width && 0 < ipt.y() && ipt.y() < image_height;
}

} // namespace gl
