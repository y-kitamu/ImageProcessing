#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace gl {
class SimpleGL {
  public:
    SimpleGL() {
        if (!glfwInit()) {
            std::cout << "failed to initialize GLFW" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        
        glfwWindowHint(GLFW_SAMPLES, 4); // 4x アンチエイリアス
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL3.3を使います。
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MacOS用：必ずしも必要ではありません。
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 古いOpenGLは使いません。

        img_window = glfwCreateWindow(1024, 768, "SimpleGL", NULL, NULL);
        if (img_window == NULL) {
            std::cout << "failed to open window" << std::endl;
            glfwTerminate();
            std::exit(EXIT_FAILURE);
        }
    }

    void init() {
        int width, height;
        
        glfwMakeContextCurrent(img_window);
        glfwGetWindowSize(img_window, &width, &height);
        glewExperimental = true;

        if (glewInit() != GLEW_OK) {
            std::cout << "failed to initialize GLEW" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // 頂点データを準備
        GLfloat vertices[4][2] = {
            {-1.0f, -1.0f},
            { 1.0f, -1.0f},
            { 1.0f,  1.0f},
            {-1.0f,  1.0f},
        };

        // VBO に頂点情報を格納
        glGenBuffers(1, &vbo); // バッファ作成
        glBindBuffer(GL_ARRAY_BUFFER, vbo); // GL コンテキストに vbo をBL_ARRAY_BUFFER でバインド
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 実データを格納

        // VAO に VBO をまとめる
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // 位置属性を 「0」にバインド（シェーダー側で後で参照）
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        
        // テクスチャ
        glGenTextures(1, &image);
        glBindTexture(GL_TEXTURE_RECTANGLE, image);
        glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    }
    
  private:
    GLFWwindow *img_window;
    GLuint vao, vbo;
    GLuint image, image_loc;
    // Shader s;
    int vertices;
};

} // namespace gl

