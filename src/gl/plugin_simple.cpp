/*
 * plugin_simple.cpp
 *
 * Create Date : 2020-03-14 18:35:10
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */
#include "plugin_simple.hpp"
#include "base.hpp"

namespace gl {

PluginSimple::PluginSimple() {
    glfwSetScrollCallback(BaseGL::img_window, scrollCallback);
    glfwSetMouseButtonCallback(BaseGL::img_window, mouseCallback);
    glfwSetCursorPosCallback(BaseGL::img_window, cursorCallback);

    shader_program_id = setShader(
        (shader_dir / fs::path(shader_basename + ".vert")).generic_string(),
        (shader_dir / fs::path(shader_basename + ".frag")).generic_string());

    imageCoord2GLCoord = imageCoord2GLCoordImpl;
    glCoord2ImageCoord = glCoord2ImageCoordImpl;
    isPointInImage = isPointInImageImpl;
}

void PluginSimple::setTexture() {
    BaseGL::frames[frame_idx]->setTexture();
}

void PluginSimple::loadGLObjects() {
    BaseGL::frames[frame_idx]->load();
}

void PluginSimple::drawGL() {
    BaseGL::frames[frame_idx]->draw();
}

void PluginSimple::drawImgui() {
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
            ImGui::Text("(x, y) = (%.1f, %.1f), (%.2f, %.2f)",
                        cursor_img_pt.x(), cursor_img_pt.y(), xpos, ypos);
        }
    }
    ImGui::Render();
}


void PluginSimple::scrollCallback(GLFWwindow * window, double xoffset, double yoffset) {
    float scale = BaseGL::frames[frame_idx]->getScale();
    scale += mouse_scroll_scale * yoffset;
    if (scale < 0.3) {
        scale = 0.3;
    }
    BaseGL::frames[frame_idx]->setScale(scale);
}

void PluginSimple::mouseCallback(GLFWwindow * window, int button, int action, int mods) {
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

void PluginSimple::cursorCallback(GLFWwindow * window, double x, double y) {
    float offset_x = BaseGL::frames[frame_idx]->getOffsetX();
    float offset_y = BaseGL::frames[frame_idx]->getOffsetY();
    xpos = x; ypos = y;
    if (is_left_button_pressed && is_pressed_in_image && !ImGui::IsAnyItemActive()) {
        offset_x += 2 * (xpos - prev_xpos) * BaseGL::width_inv;
        offset_y -= 2 * (ypos - prev_ypos) * BaseGL::height_inv;
        prev_xpos = xpos; prev_ypos = ypos;
        BaseGL::frames[frame_idx]->setOffsetX(offset_x);
        BaseGL::frames[frame_idx]->setOffsetY(offset_y);
    }
}

Eigen::Vector2d PluginSimple::imageCoord2GLCoordImpl(Eigen::Vector2d img_pt) {
    int image_width = BaseGL::frames[frame_idx]->getImageWidth();
    int image_height = BaseGL::frames[frame_idx]->getImageHeight();
    float scale = BaseGL::frames[frame_idx]->getScale();
    float offset_x = BaseGL::frames[frame_idx]->getOffsetX();
    float offset_y = BaseGL::frames[frame_idx]->getOffsetY();
    
    float aspect_ratio = (float)image_height / image_width * BaseGL::width / BaseGL::height;
    Eigen::Vector2d gl_pt;

    gl_pt.x() = (img_pt.x() * scale / image_width + offset_x - scale * 0.5 + 1.0f)
        * 0.5 * BaseGL::width;
    gl_pt.y() = (1 - ((1 - img_pt.y() / image_height) * scale * aspect_ratio + offset_y -
                      scale * 0.5 * aspect_ratio + 1.0f) * 0.5) * BaseGL::height;
    return gl_pt;
}


Eigen::Vector2d PluginSimple::glCoord2ImageCoordImpl(Eigen::Vector2d gl_pt) {
    /*
     * window の coordinate (左上(0, 0)、右下(window_width, window_height)) から
     * Image coordinate (画像左上(0, 0), 画像右下(image_width, image_height))
     */
    int image_width = BaseGL::frames[frame_idx]->getImageWidth();
    int image_height = BaseGL::frames[frame_idx]->getImageHeight();
    float scale = BaseGL::frames[frame_idx]->getScale();
    float offset_x = BaseGL::frames[frame_idx]->getOffsetX();
    float offset_y = BaseGL::frames[frame_idx]->getOffsetY();
    
    double aspect_ratio = 1.0 * image_height  * BaseGL::width / (image_width * BaseGL::height);
    Eigen::Vector2d img_pt;
    
    img_pt.x() = (2.0 * gl_pt.x() * BaseGL::width_inv - 1.0 - offset_x + scale * 0.5)
        * image_width / scale;
    img_pt.y() = (1 - ((1 - gl_pt.y() * BaseGL::height_inv) * 2 - 1.0 - offset_y +
                       scale * 0.5 * aspect_ratio) / (scale * aspect_ratio)) * image_height;
    
    return img_pt;
}


bool PluginSimple::isPointInImageImpl(double x, double y) {
    int image_width = BaseGL::frames[frame_idx]->getImageWidth();
    int image_height = BaseGL::frames[frame_idx]->getImageHeight();
    auto ipt = glCoord2ImageCoord(Eigen::Vector2d(x, y));
    return 0 < ipt.x() && ipt.x() < image_width && 0 < ipt.y() && ipt.y() < image_height;
}

} // namespace gl
