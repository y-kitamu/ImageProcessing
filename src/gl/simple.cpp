#include "simple.hpp"


namespace gl {

void SimpleGL::load_gl_objects() {
    // 頂点データを準備
    // 頂点バッファオブジェクト (VBO, cpu 側のオブジェクト) を直接描画に指定することはできません.
    // 描画に指定できるのは, 頂点バッファオブジェクトを組み込んだ頂点配列オブジェクト (VAO, gpu側のオブジェクト) だけです.

    float aspect_ratio = 1.0f * frame.rows / frame.cols;
    // GL_TEXTURE_RECTANGLE の場合、uv 座標は Textureの ピクセルの座標の値と一致する。
    // x, y, u, v
    GLfloat position[4][4] = {
        {-0.5f, -0.5f * aspect_ratio, 0.0f, (float)frame.rows}, 
        { 0.5f, -0.5f * aspect_ratio, (float)frame.cols, (float)frame.rows},
        { 0.5f,  0.5f * aspect_ratio, (float)frame.cols, 0.0f},
        {-0.5f,  0.5f * aspect_ratio, 0.0f, 0.0f},
    };
    vertices = sizeof(position) / sizeof(position[0]);

    // VAO を作成、有効にする
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VBO に頂点情報を格納
    glGenBuffers(1, &vbo);  // バッファ作成
    glBindBuffer(GL_ARRAY_BUFFER, vbo);  // GL コンテキストに vbo をBL_ARRAY_BUFFER でバインド
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);  // 実データを格納

    // 位置属性を 「0」にバインド（シェーダー側で後で参照）
    int index = 0, size = 2, stride = sizeof(position[0]);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, 0);  // shader の attrib 属性に渡すデータの指定
    glEnableVertexAttribArray(index);
    index = 1;
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void *)(stride / 2));
    glEnableVertexAttribArray(index);

    glBindBuffer(GL_ARRAY_BUFFER, 0);  // vbo の bind を解除

    glBindVertexArray(0);  // vao の bind を解除
    
    // テクスチャ
    glGenTextures(1, &image);
    glBindTexture(GL_TEXTURE_RECTANGLE, image);
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
}

void SimpleGL::draw_gl() {
    // 切り出した画像をテクスチャに転送する
    glBindTexture(GL_TEXTURE_RECTANGLE, image);
    glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, 0, 0, frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.data);

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

    ImGui::Begin("Hello world!");
    ImGui::Checkbox("demo window", &demo_window);
    ImGui::Checkbox("another window", &another_window);
    ImGui::End();
    
    ImGui::Render();
}

}
