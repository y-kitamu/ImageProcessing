#include "base.hpp"
#include "plugin_simple.hpp"


namespace gl {

BaseGL::BaseGL() {
    initGL();
    initImgui();

    plugin = std::make_shared<PluginSimple>();
    Points::setShader();
    Lines::setShader();
}

BaseGL::~BaseGL() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(img_window);
    glfwTerminate();
    return;
}

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
    img_window = glfwCreateWindow(width, height, "Debug Window", NULL, NULL);
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

    imgui_theme.setCurrentTheme();

    ImGui_ImplGlfw_InitForOpenGL(img_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}


void BaseGL::draw() {
    plugin->initDraw();

    int framecount = 0;
    double current, previous = glfwGetTime();

    while (!glfwWindowShouldClose(img_window)) {
        // Clear the screen
        glfwMakeContextCurrent(img_window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (frames.size() == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            continue;
        }
        plugin->loadGLObjects();
        plugin->drawGL();
        drawImguiMenu();
        plugin->drawImgui();
        checkKeyboardAndMouseInput();
        plugin->checkKeyboardAndMouseInput();

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
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        glClear(GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(img_window);
        glfwPollEvents();
    }
}


void BaseGL::drawImguiMenu() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        // menu bar (一番上のバー)
        // ファイルのセーブ(未実装)・ロードなど
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
            if (ImGui::BeginMenu("Plugin")) {
                for (auto && plug : plugins) {
                    if (ImGui::MenuItem(plug.first.c_str())) {
                        plugin = plug.second();
                        plugin->initDraw();
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
}


void BaseGL::checkKeyboardAndMouseInput() {
    if (glfwGetKey(img_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(img_window, true);
    }
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
