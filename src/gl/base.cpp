#include "base.hpp"


namespace gl {

void BaseGL::init_gl() {
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

    // Initialize GL3W
    if (gl3wInit()) {
        std::cout << "failed to initialize gl3w" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(img_window, GLFW_STICKY_KEYS, GL_TRUE);
}


void BaseGL::init_imgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO & io = ImGui::GetIO(); (void) io;
    ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    ImGui_ImplGlfw_InitForOpenGL(img_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

}
