#include "simple.hpp"


namespace gl {

// static 変数のためここで代入
float SimpleGL::scale = 1.0, SimpleGL::offset_x = 0.0, SimpleGL::offset_y = 0.0;
double SimpleGL::prev_xpos = 0.0, SimpleGL::prev_ypos = 0.0;
double SimpleGL::xpos = 0.0, SimpleGL::ypos = 0.0;
bool SimpleGL::is_left_button_pressed = false;


void SimpleGL::load_gl_objects() {
    // 頂点データを準備
    // 頂点バッファオブジェクト (VBO, cpu 側のオブジェクト) を直接描画に指定することはできません.
    // 描画に指定できるのは, 頂点バッファオブジェクトを組み込んだ頂点配列オブジェクト (VAO, gpu側のオブジェクト) だけです.
    float rows = frames[frame_idx].rows, cols = frames[frame_idx].cols;
    float aspect_ratio = rows / cols * width / height;
    // GL_TEXTURE_RECTANGLE の場合、uv 座標は Textureの ピクセルの座標の値と一致する。
    // x, y, u, v
    GLfloat position[4][4] = {
        {-0.5f * scale + offset_x, -0.5f * aspect_ratio * scale + offset_y, 0.0f, rows},
        { 0.5f * scale + offset_x, -0.5f * aspect_ratio * scale + offset_y, cols, rows},
        { 0.5f * scale + offset_x,  0.5f * aspect_ratio * scale + offset_y, cols, 0.0f},
        {-0.5f * scale + offset_x,  0.5f * aspect_ratio * scale + offset_y, 0.0f, 0.0f},
    };
    vertices = sizeof(position) / sizeof(position[0]);

    // VAO (gpu 側のオブジェクト)を作成、有効にする
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VBO (cpu 側のオブジェクト)に頂点情報を格納 
    glGenBuffers(1, &vbo);  // バッファ作成
    glBindBuffer(GL_ARRAY_BUFFER, vbo);  // GL コンテキストに vbo をBL_ARRAY_BUFFER でバインド
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);  // 実データを格納

    // 位置属性を 「0」にバインド（シェーダー側で後で参照）
    int index = 0, size = 2, stride = sizeof(position[0]);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, 0);  // shader の attrib 属性に渡すデータの指定
    glEnableVertexAttribArray(index);
    index = 1;
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (GLvoid *)(size * sizeof(GLfloat)));
    glEnableVertexAttribArray(index);

    glBindBuffer(GL_ARRAY_BUFFER, 0);  // vbo の bind を解除
    glBindVertexArray(0);  // vao の bind を解除

    // テクスチャ
    glGenTextures(1, &image);
    glBindTexture(GL_TEXTURE_RECTANGLE, image);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteriv(GL_TEXTURE_RECTANGLE, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);
}

void SimpleGL::draw_gl() {
    // 切り出した画像をテクスチャに転送する
    // TODO: FBO を使って高速化 https://stackoverflow.com/questions/3887636/how-to-manipulate-texture-content-on-the-fly/10702468#10702468
    glBindTexture(GL_TEXTURE_RECTANGLE, image);
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB, frames[frame_idx].cols, frames[frame_idx].rows, 0,
                 texture_format, GL_UNSIGNED_BYTE, frames[frame_idx].ptr());

    // シェーダプログラムの使用開始
    glUseProgram(program_id);

    // テクスチャユニットとテクスチャの指定
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_RECTANGLE, image);

    // 描画に使う頂点配列オブジェクトの指定
    glBindVertexArray(vao);

    // 図形の描画
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices);

    // bind を解除
    glBindVertexArray(0);
    glUseProgram(0);
}

void SimpleGL::draw_imgui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

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
            set_texture_format();
        }
    }
    
    ImGui::Render();
}

void SimpleGL::check_keyboard_and_mouse_input() {
    BaseGL::check_keyboard_and_mouse_input();
}

void SimpleGL::scroll_callback(GLFWwindow * window, double xoffset, double yoffset) {
    scale += mouse_scroll_scale * yoffset;
}

void SimpleGL::mouse_callback(GLFWwindow * window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        is_left_button_pressed = true;
        glfwGetCursorPos(window, &prev_xpos, &prev_ypos);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        is_left_button_pressed = false;
    }
}

void SimpleGL::cursor_callback(GLFWwindow * window, double x, double y) {
    if (is_left_button_pressed) {
        xpos = x; ypos = y;
        offset_x += 2 * (xpos - prev_xpos) / width;
        offset_y -= 2 * (ypos - prev_ypos) / height;
        
        prev_xpos = xpos; prev_ypos = ypos;
    }
}


} // namespace gl
