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


void BaseGL::draw() {
    int framecount = 0;
    double current, previous = glfwGetTime();

    while (!glfwWindowShouldClose(img_window)) {
        // Clear the screen
        glfwMakeContextCurrent(img_window);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        load_gl_objects();

        draw_gl();  // virtual : open gl objects の描画処理
        draw_imgui();  // virtual : imgui の描画処理
        check_keyboard_and_mouse_input();

        // frame rate to 30 fps
        current = glfwGetTime();
        framecount++;
        if (current - previous > 1) {
            std::stringstream ss;
            ss << "frame rate : " <<  framecount << " fps";
            glfwSetWindowTitle(img_window, ss.str().c_str());
            framecount = 0;
            previous = current;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
            
        glClear(GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(img_window);
    }
}


void BaseGL::check_keyboard_and_mouse_input() {
    if (glfwGetKey(img_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(img_window, GLFW_TRUE);
    }
}

}
