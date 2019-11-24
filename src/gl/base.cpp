#include "base.hpp"


namespace gl {

int BaseGL::width = 1024, BaseGL::height = 768;
float BaseGL::width_inv = 1.0f / BaseGL::width, BaseGL::height_inv = 1.0f / BaseGL::height;

void BaseGL::initGL() {
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
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); //

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
    
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        // initialize debug output
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    } 
}


void BaseGL::initImgui() {
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

        loadGLObjects();

        drawGL();  // virtual : open gl objects の描画処理
        drawImgui();  // virtual : imgui の描画処理
        checkKeyboardAndMouseInput();

        // show Frame Per Seconds (fps) on window title
        current = glfwGetTime();
        framecount++;
        if (current - previous > 1) {
            std::stringstream ss;
            ss << "frame rate : " <<  framecount << " fps";
            glfwSetWindowTitle(img_window, ss.str().c_str());
            framecount = 0;
            previous = current;
        }
        
        // frame rate to 30 fps
        //std::this_thread::sleep_for(std::chrono::milliseconds(20));
            
        glClear(GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(img_window);
    }
}


void BaseGL::checkKeyboardAndMouseInput() {
    if (glfwGetKey(img_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(img_window, true);
    }
}

void BaseGL::windowSizeCallback(GLFWwindow* window, int w, int h) {
    width = w; height = h;
    width_inv = 1.0f / w, height_inv = 1.0f / h;
}

void APIENTRY BaseGL::glDebugOutput(GLenum source,  GLenum type, GLuint id, GLenum severity, 
                                    GLsizei length, const GLchar *message, void *userParam) {
    // https://learnopengl.com/In-Practice/Debugging
    
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source) {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

}
