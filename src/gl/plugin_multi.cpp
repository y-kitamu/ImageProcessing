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
        (shader_dir / fs::path(shader_basename + ".vert")).generic_string(),
        (shader_dir / fs::path(shader_basename + ".frag")).generic_string());
}

void PluginMulti::setTexture() {
    BaseGL::frames[first_frame_idx]->setTexture();
    BaseGL::frames[second_frame_idx]->setTexture();
}

void PluginMulti::loadGLObjects() {
    setViewport(1);
    BaseGL::frames[first_frame_idx]->load();
    setViewport(2);
    BaseGL::frames[second_frame_idx]->load();
    setViewport(0);
}

void PluginMulti::drawGL() {
    setViewport(1);
    BaseGL::frames[first_frame_idx]->draw();
    setViewport(2);
    BaseGL::frames[second_frame_idx]->draw();
    setViewport(0);
}

void PluginMulti::setViewport(int view_idx) {
    switch (view_idx) {
    case 0:
        setViewport(0, 0, BaseGL::width, BaseGL::height);
        break;
    case 1:
        setViewport(0, 0, BaseGL::width / 2, BaseGL::height);
        frame_idx = first_frame_idx;
        break;
    case 2:
        setViewport(BaseGL::width / 2, 0, BaseGL::width / 2, BaseGL::height);
        frame_idx = second_frame_idx;
        break;
    }
    imageCoord2GLCoord = [](Eigen::Vector2d img_pt) {
        return imageCoord2GLCoordImpl(img_pt, frame_idx);
    };
    glCoord2ImageCoord = [](Eigen::Vector2d gl_pt) {
        return glCoord2ImageCoordImpl(gl_pt, frame_idx);
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

void PluginMulti::calcCursorPointView() {
    /*
     * cursor がある位置の viewport をsetして、画像のframe_idx を返す。
     */
    double xpos, ypos;
    glfwGetCursorPos(BaseGL::img_window, &xpos, &ypos);
    if (xpos * 2 < BaseGL::width) {
        setViewport(1);
        frame_idx = first_frame_idx;
    } else {
        setViewport(2);
        frame_idx =  second_frame_idx;
    }
}

void PluginMulti::scrollCallback(GLFWwindow * window, double xoffset, double yoffset) {
    calcCursorPointView();
    float scale = BaseGL::frames[frame_idx]->getScale();
    scale += scale * mouse_scroll_scale * yoffset;
    if (scale < 0.3) {
        scale = 0.3;
    }
    BaseGL::frames[frame_idx]->setScale(scale);
}

void PluginMulti::mouseCallback(GLFWwindow *window, int button, int action, int mods) {
}

void PluginMulti::cursorCallback(GLFWwindow * window, double xpos, double ypos) {
    
}

int PluginMulti::isPointInImage(double x, double y) {
    /*
     * Return frame index if cursor is on the frame else -1
     */
    
    return -1;
}

} // namespace gl
