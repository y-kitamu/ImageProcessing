#include "simple.hpp"
#include <fmt/format.h>


namespace gl {

// static 変数のためここで代入
int SimpleGL::image_width = 0, SimpleGL::image_height = 0;
float SimpleGL::scale = 1.0f;
float SimpleGL::offset_x = 0.0, SimpleGL::offset_y = 0.0; // 画像の中心の window の中心に対する offset
double SimpleGL::prev_xpos = 0.0, SimpleGL::prev_ypos = 0.0;
double SimpleGL::xpos = 0.0, SimpleGL::ypos = 0.0;
bool SimpleGL::is_left_button_pressed = false, SimpleGL::is_pressed_in_image = false;


void SimpleGL::loadGLObjects() {
    // 頂点データを準備
    // 頂点バッファオブジェクト (VBO, cpu 側のオブジェクト) を直接描画に指定することはできません.
    // 描画に指定できるのは, 頂点バッファオブジェクトを組み込んだ頂点配列オブジェクト (VAO, gpu側のオブジェクト) だけです.
    float rows = frames[frame_idx].rows, cols = frames[frame_idx].cols;
    float aspect_ratio = rows / cols * width / height;
    GLfloat position[4][4] = {
        // [x, y, u, v]
        {-0.5f * scale + offset_x, -0.5f * aspect_ratio * scale + offset_y, 0.0f, 1.0f},
        { 0.5f * scale + offset_x, -0.5f * aspect_ratio * scale + offset_y, 1.0f, 1.0f},
        { 0.5f * scale + offset_x,  0.5f * aspect_ratio * scale + offset_y, 1.0f, 0.0f},
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
}

void SimpleGL::drawGL() {
    // 切り出した画像をテクスチャに転送する
    // TODO: FBO を使って高速化 https://stackoverflow.com/questions/3887636/how-to-manipulate-texture-content-on-the-fly/10702468#10702468

    // シェーダプログラムの使用開始
    glUseProgram(program_id);

    // テクスチャユニットとテクスチャの指定
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, image);

    // 描画に使う頂点配列オブジェクトの指定
    glBindVertexArray(vao);

    // 図形の描画
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices);

    // bind を解除
    glBindVertexArray(0);
    glUseProgram(0);
}

void SimpleGL::drawImgui() {
    // TODO : base.cpp に移す
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
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
    if (is_left_button_pressed && is_pressed_in_image) {
        offset_x += 2 * (xpos - prev_xpos) * width_inv;
        offset_y -= 2 * (ypos - prev_ypos) * height_inv;
        
        prev_xpos = xpos; prev_ypos = ypos;
    }
}

void SimpleGL::setTexture() {
    // gray scale の画像と color 画像を切り替え
    if (frames[frame_idx].type() == CV_8UC1) {
        texture_format = GL_RED;
        texture_internal_format = GL_RGB;
        swizzle_mask[0] = GL_RED;
        swizzle_mask[1] = GL_RED;
        swizzle_mask[2] = GL_RED;
        swizzle_mask[3] = GL_ZERO;
    } else if (frames[frame_idx].type() == CV_8UC3) {
        texture_format = GL_BGR;
        texture_internal_format = GL_RGB;
        swizzle_mask[0] = GL_RED;
        swizzle_mask[1] = GL_GREEN;
        swizzle_mask[2] = GL_BLUE;
        swizzle_mask[3] = GL_ZERO;
    } else if (frames[frame_idx].type() == CV_32FC1) {
        texture_format = GL_RED;
        texture_internal_format = GL_R32F;
        swizzle_mask[0] = GL_RED;
        swizzle_mask[1] = GL_RED;
        swizzle_mask[2] = GL_RED;
        swizzle_mask[3] = GL_ZERO;
    }

    image_width = frames[frame_idx].cols;
    image_height = frames[frame_idx].rows;
    
    // テクスチャ
    glGenTextures(1, &image);
    glBindTexture(GL_TEXTURE_2D, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // GL_TEXTURE_2D の場合 GL_LINEAR_MIPMAP_LINEAR だと真っ黒になった
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);

    glTexImage2D(GL_TEXTURE_2D, 0, texture_internal_format, frames[frame_idx].cols,
                 frames[frame_idx].rows, 0, texture_format, GL_UNSIGNED_BYTE, frames[frame_idx].ptr());
    glGenerateMipmap(GL_TEXTURE_2D);
}


Eigen::Vector2d SimpleGL::imageCoord2GLCoord(Eigen::Vector2d img_pt) {
    /*
     */
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
