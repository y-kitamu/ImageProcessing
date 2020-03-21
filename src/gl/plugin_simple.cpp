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
    glfwSetWindowSizeCallback(BaseGL::img_window, windowSizeCallback);
    glfwSetFramebufferSizeCallback(BaseGL::img_window, framebufferSizeCallback);

    shader_program_id = setShader(
        (shader_dir / fs::path(shader_basename + ".vert")).generic_string(),
        (shader_dir / fs::path(shader_basename + ".frag")).generic_string());

    imageCoord2GLCoord = [](Eigen::Vector2d img_pt) {
        return imageCoord2GLCoordImpl(img_pt, frame_idx);
    };
    glCoord2ImageCoord = [](Eigen::Vector2d gl_pt) {
        return glCoord2ImageCoordImpl(gl_pt, frame_idx);
    };
    BaseGL::view_width = BaseGL::width;
    BaseGL::view_height = BaseGL::height;
    BaseGL::width_inv = 1.f / BaseGL::width;
    BaseGL::height_inv = 1.f / BaseGL::height;
    glViewport(0, 0, BaseGL::view_width, BaseGL::view_height);
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
    scale += scale * mouse_scroll_scale * yoffset;
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

void PluginSimple::windowSizeCallback(GLFWwindow* window, int w, int h) {
    BaseGL::width = w; BaseGL::height = h;
    BaseGL::width_inv = 1.0f / w, BaseGL::height_inv = 1.0f / h;
}

void PluginSimple::framebufferSizeCallback(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
    BaseGL::view_width = w;
    BaseGL::view_height = h;
}

bool PluginSimple::isPointInImage(double x, double y) {
    int image_width = BaseGL::frames[frame_idx]->getImageWidth();
    int image_height = BaseGL::frames[frame_idx]->getImageHeight();
    auto ipt = glCoord2ImageCoord(Eigen::Vector2d(x, y));
    return 0 < ipt.x() && ipt.x() < image_width && 0 < ipt.y() && ipt.y() < image_height;
}

} // namespace gl
