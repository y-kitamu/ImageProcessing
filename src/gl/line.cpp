#include "line.hpp"
#include "base.hpp"

namespace gl {


void Line::load(int frame_idx) {
    is_active = false;
    auto pt0 = line_segment.first;
    auto pt1 = line_segment.second;

    if (pt0->getFrameIndex() != frame_idx || pt1->getFrameIndex() != frame_idx) {
        return;
    }
    auto gl_pt0 = pt0->getGLPt();
    auto gl_pt1 = pt1->getGLPt();
    load(gl_pt0, gl_pt1);
}

void Line::load(int frame_idx0, int frame_idx1) {
    // TODO : refactor
    is_active = false;
    auto pt0 = line_segment.first;
    auto pt1 = line_segment.second;

    auto idx0 = pt0->getFrameIndex();
    auto idx1 = pt1->getFrameIndex();
    if (idx0 != frame_idx0 && idx0 != frame_idx1) {
        return;
    }
    if (idx1 != frame_idx0 && idx1 != frame_idx1) {
        return;
    }
    auto gl_pt0 = pt0->getGLPt();
    auto gl_pt1 = pt1->getGLPt();

    auto convert_coord = [&frame_idx0, &frame_idx1](Eigen::Vector2d pt, int idx) {
        if (idx == frame_idx0) {
            pt.x() = (pt.x() - 1) * 0.5;
        } else if (idx == frame_idx1) {
            pt.x() = (pt.x() + 1) * 0.5;
        }
        return pt;
    };
    gl_pt0 = convert_coord(gl_pt0, idx0);
    gl_pt1 = convert_coord(gl_pt1, idx1);
    load(gl_pt0, gl_pt1);
}


void Line::load(Eigen::Vector2d gl_pt0, Eigen::Vector2d gl_pt1) {
    is_active = true;
    GLfloat position[2][6] = {
        {(float)gl_pt0.x(), (float)gl_pt0.y(), color.x(), color.y(), color.z(), color.w()},
        {(float)gl_pt1.x(), (float)gl_pt1.y(), color.x(), color.y(), color.z(), color.w()},
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VBO (cpu 側のオブジェクト)に頂点情報を格納 
    glGenBuffers(1, &vbo);  // バッファ作成
    glBindBuffer(GL_ARRAY_BUFFER, vbo);  // GL コンテキストに vbo をBL_ARRAY_BUFFER でバインド
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);  // 実データを格納

    // 位置属性を 「0」にバインド（シェーダー側で後で参照）
    int pos_index = 0, pos_size = 2, stride = sizeof(position[0]);
    glVertexAttribPointer(pos_index, pos_size, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(pos_index);
    int color_index = 1, color_size = 4;
    glVertexAttribPointer(color_index, color_size, GL_FLOAT, GL_FALSE,
                          stride, (GLvoid *)(pos_size * sizeof(GLfloat)));
    glEnableVertexAttribArray(color_index);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // vbo の bind を解除
    glBindVertexArray(0);  // vao の bind を解除
}

void Line::draw() {
    if (!is_active) {
        return;
    }
    glViewport(0, 0, BaseGL::width, BaseGL::height);
    glUseProgram(Lines::program_id);
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, vertices);
}

void Lines::setShader() {
    program_id = gl::setShader(
        (BaseGL::shader_dir / fs::path(shader_basename + ".vert")).generic_string(),
        (BaseGL::shader_dir / fs::path(shader_basename + ".frag")).generic_string());
}

void Lines::addLine(std::shared_ptr<Point> pt1, std::shared_ptr<Point> pt2, Eigen::Vector4f color) {
    lines.emplace_back(std::make_shared<Line>(pt1, pt2, color));
}

void Lines::load() {
    std::string plugin_name = BaseGL::plugin->getName();
    if (plugin_name == BaseGL::PluginNames::SIMPLE) {
        int frame_idx = PluginSimple::frame_idx;
        for (auto && line : lines) {
            line->load(frame_idx);
        }
    } else if (plugin_name == BaseGL::PluginNames::MULTI) {
        int frame_idx0 = PluginMulti::first_frame_idx;
        int frame_idx1 = PluginMulti::second_frame_idx;
        for (auto && line : lines) {
            line->load(frame_idx0, frame_idx1);
        }
    }
}

void Lines::draw() {
    for (auto && line : lines) {
        line->draw();
    }
}

} // namespace gl 
