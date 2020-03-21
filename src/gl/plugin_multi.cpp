/*
 * plugin_multi.cpp
 *
 * Create Date : 2020-03-21 10:08:11
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */
#include "plugin_multi.hpp"
#include "base.hpp"

namespace gl {

PluginMulti::PluginMulti() {
    glfwSetScrollCallback(BaseGL::img_window, scrollCallback);
    glfwSetMouseButtonCallback(BaseGL::img_window, mouseCallback);
    glfwSetCursorPosCallback(BaseGL::img_window, cursorCallback);

    shader_program_id = setShader(
        (BaseGL::shader_dir / fs::path(shader_basename + ".vert")).generic_string(),
        (BaseGL::shader_dir / fs::path(shader_basename + ".frag")).generic_string());
}

std::string PluginMulti::getName() {
    return BaseGL::PluginNames::MULTI;
}

void PluginMulti::setTexture() {
    BaseGL::frames[first_frame_idx]->setTexture();
    BaseGL::frames[second_frame_idx]->setTexture();
}

void PluginMulti::loadGLObjects() {
    setViewport(BaseGL::Viewport::LEFT);
    BaseGL::frames[first_frame_idx]->load();
    setViewport(BaseGL::Viewport::RIGHT);
    BaseGL::frames[second_frame_idx]->load();
    setViewport(BaseGL::Viewport::ALL);
    BaseGL::lines.load();
}

void PluginMulti::drawGL() {
    setViewport(BaseGL::Viewport::LEFT);
    BaseGL::frames[first_frame_idx]->draw();
    setViewport(BaseGL::Viewport::RIGHT);
    BaseGL::frames[second_frame_idx]->draw();
    setViewport(BaseGL::Viewport::ALL);
    BaseGL::lines.draw();
}

void PluginMulti::setViewport(int view_idx) {
    switch (view_idx) {
    case BaseGL::Viewport::ALL:
        setViewport(0, 0, BaseGL::width, BaseGL::height);
        drawing_frame_idx = -1;
        break;
    case BaseGL::Viewport::LEFT:
        setViewport(0, 0, BaseGL::width / 2, BaseGL::height);
        drawing_frame_idx = first_frame_idx;
        break;
    case BaseGL::Viewport::RIGHT:
        setViewport(BaseGL::width / 2, 0, BaseGL::width / 2, BaseGL::height);
        drawing_frame_idx = second_frame_idx;
        break;
    }
    imageCoord2GLCoord = [](Eigen::Vector2d img_pt) {
        return imageCoord2GLCoordImpl(img_pt, drawing_frame_idx);
    };
    glCoord2ImageCoord = [](Eigen::Vector2d gl_pt) {
        return glCoord2ImageCoordImpl(gl_pt, drawing_frame_idx);
    };
    isPointInImage = [](double x, double y) {
        return isPointInImageImpl(x, y, drawing_frame_idx);
    };
}

void PluginMulti::setViewport(int minx, int miny, int width, int height) {
    glViewport(minx, miny, width, height);
    BaseGL::view_width = width;
    BaseGL::view_height = height;
    BaseGL::width_inv = 1.0 / width;
    BaseGL::height_inv = 1.0 / height;
}

void PluginMulti::drawImgui() {
    {
        ImGui::Begin("frames");
        bool is_changed = false;
        for (int i = 0; i < (int)BaseGL::frames.size(); i++) {
            std::stringstream ss;
            ss << i + 1;
            is_changed |= ImGui::RadioButton(ss.str().c_str(), &first_frame_idx, i);
        }
        ImGui::End();
        if (is_changed) {
            setTexture();
        }
    }
    ImGui::Render();
}

void PluginMulti::scrollCallback(GLFWwindow * window, double xoffset, double yoffset) {
    calcCursorPointView();
    float scale = BaseGL::frames[focused_frame_idx]->getScale();
    scale += scale * mouse_scroll_scale * yoffset;
    if (scale < 0.3) {
        scale = 0.3;
    }
    BaseGL::frames[focused_frame_idx]->setScale(scale);
}

void PluginMulti::mouseCallback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        is_left_button_pressed = true;
        calcCursorPointFrame(xpos, ypos);
        glfwGetCursorPos(window, &prev_xpos, &prev_ypos);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        is_left_button_pressed = false;
        focused_frame_idx = -1;
    }
}

void PluginMulti::cursorCallback(GLFWwindow * window, double x, double y) {
    xpos = x; ypos = y;
    if (is_left_button_pressed && focused_frame_idx != -1 && !ImGui::IsAnyItemActive()) {
        int view_idx = focused_frame_idx == first_frame_idx ? 1 : 2;
        setViewport(view_idx);
        float offset_x = BaseGL::frames[focused_frame_idx]->getOffsetX();
        float offset_y = BaseGL::frames[focused_frame_idx]->getOffsetY();
        offset_x += 2 * (xpos - prev_xpos) * BaseGL::width_inv;
        offset_y -= 2 * (ypos - prev_ypos) * BaseGL::height_inv;
        prev_xpos = xpos; prev_ypos = ypos;
        BaseGL::frames[focused_frame_idx]->setOffsetX(offset_x);
        BaseGL::frames[focused_frame_idx]->setOffsetY(offset_y);
    }
}

void PluginMulti::calcCursorPointView() {
    /*
     * cursor がある位置の viewport をsetして、画像のframe_idx を返す。
     */
    double xpos, ypos;
    glfwGetCursorPos(BaseGL::img_window, &xpos, &ypos);
    if (xpos * 2 < BaseGL::width) {
        setViewport(BaseGL::Viewport::LEFT);
        focused_frame_idx = first_frame_idx;
    } else {
        setViewport(BaseGL::Viewport::RIGHT);
        focused_frame_idx = second_frame_idx;
    }
}

void PluginMulti::calcCursorPointFrame(double x, double y) {
    /*
     * Return frame index if cursor is on the frame else -1
     */
    setViewport(BaseGL::Viewport::LEFT);
    if (isPointInImage(x, y)) {
        focused_frame_idx = first_frame_idx;
        return;
    }
    x = x - BaseGL::view_width;
    setViewport(BaseGL::Viewport::RIGHT);
    if (isPointInImage(x, y)) {
        focused_frame_idx = second_frame_idx;
        return;
    }
    focused_frame_idx = -1;
}

} // namespace gl
