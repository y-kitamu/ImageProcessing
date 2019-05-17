#include "simple.hpp"


namespace gl {

void SimpleGL::init_gl() {
    // initialize glfw
    if (!glfwInit()) {
        std::cout << "failed to initialize GLFW" << std::endl;
        std::exit(EXIT_FAILURE);
    }
        
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x アンチエイリアス
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL3.3を使います。
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MacOS用：必ずしも必要ではありません。
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 古いOpenGLは使いません。

    // Open a window and create its OpenGL context
    img_window = glfwCreateWindow(width, height, "SimpleGL", NULL, NULL);
    if (img_window == NULL) {
        std::cout << "failed to open window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(img_window);
        
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cout << "failed to initialize GLEW" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(img_window, GLFW_STICKY_KEYS, GL_TRUE);
}


void SimpleGL::init_imgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO & io = ImGui::GetIO(); (void) io;
    ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    ImGuiImplGlfw_InitForOpenGL(img_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
}

void SimpleGL::load_gl_objects() {
    // 頂点データを準備
    // 頂点バッファオブジェクト (VBO) を直接描画に指定することはできません.
    // 描画に指定できるのは, 頂点バッファオブジェクトを組み込んだ頂点配列オブジェクト (VAO) だけです.
    // GL_TEXTURE_RECTANGLE の場合、uv 座標は Textureの ピクセルの座標の値と一致する。
    // x, y, u, v
    GLfloat position[4][4] = {
        {-0.5f, -0.5f, 0.0f, 512}, 
        { 0.5f, -0.5f, 512, 512}, 
        { 0.5f,  0.5f, 512, 0.0f}, 
        {-0.5f,  0.5f, 0.0f, 0.0f}, 
    };
    vertices = sizeof(position) / sizeof(position[0]);

    // VAO を作成、有効にする
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VBO に頂点情報を格納
    glGenBuffers(1, &vbo); // バッファ作成
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // GL コンテキストに vbo をBL_ARRAY_BUFFER でバインド
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW); // 実データを格納

    // 位置属性を 「0」にバインド（シェーダー側で後で参照）
    int index = 0, size = 2, stride = sizeof(position[0]);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, 0); // shader の attrib 属性に渡すデータの指定
    glEnableVertexAttribArray(index);
    index = 1;
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void *)(stride / 2));
    glEnableVertexAttribArray(index);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // vbo の bind を解除

    glBindVertexArray(0); // vao の bind を解除
        
    // テクスチャ
    glGenTextures(1, &image);
    glBindTexture(GL_TEXTURE_RECTANGLE, image);
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
}

void SimpleGL::draw_gl(const cv::Mat &frame) {
    // 切り出した画像をテクスチャに転送する
    glBindTexture(GL_TEXTURE_RECTANGLE, image);
    glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, 0, 0, frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.data);

    // シェーダプログラムの使用開始
    glUseProgram(program_id);

    // uniform サンプラの指定
    // glUniform1i(image_loc, 0);

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

    ImGui::ShowDemoWindow(&show_demo_window);

}

}
