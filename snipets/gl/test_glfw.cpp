/*
 * test_glfw.cpp
 *
 * Create Date : 2019-11-23 06:44:04
 * Copyright (c) 2019 Yusuke Kitamura <ymyk6602@gmail.com>
 */

#include <iostream>
#include <cstdlib>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>

#include "gl/shader.hpp"

namespace fs = boost::filesystem;

int main() {
    /*
     * (1) GLFW を初期化する (glfwInit())
     * (2) ウィンドウを作成する (glfwCreateWindow())
     * (3) ウィンドウが開いている間繰り返し描画する (glfwWindowShouldClose())
     * (4) ダブルバッファリングのバッファの入れ替えを行う (glfwSwapBuffers())
     * (5) ウィンドウが閉じたら終了処理を行う (glfwTerminate())
     */

    // (1) GLFW を初期化する (glfwInit())
    if (glfwInit() == GL_FALSE) {
        std::cerr << "Can't initialize GLFW" << std::endl;
        return 1;
    }

    // プログラム終了時の処理を登録する
    std::atexit(glfwTerminate);
    
    // OpenGL Version 3.2 Core Profile を選択する
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
    // ウィンドウを作成する (glfwCreateWindow())
    GLFWwindow *const window(glfwCreateWindow(640, 480, "Hello", nullptr, nullptr));
    if (window == nullptr) {
        std::cerr << "Can't create GLFW window" << std::endl;
        return 1;
    }

    // 作成したウィンドウを OpenGL の処理対象にする
    glfwMakeContextCurrent(window);

    // GL3W を初期化する
    if (gl3wInit()) {
        std::cerr << "Can't initialize GL3W"  << std::endl;
        return 1;
    }
    
    // ダブルバッファリングにおける、カラーバッファの入れ替えのタイミングを指定
    glfwSwapInterval(1);
 
     // 背景色を指定する
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);


    // texture の処理
    float texCoords[] = {
        0.0f, 0.0f,  // lower-left corner  
        1.0f, 0.0f,  // lower-right corner
        0.5f, 1.0f   // top-center corner
    };

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    fs::path file_path = fs::path(__FILE__).parent_path() / fs::path("../../data/IMG_20190630_100108_BURST002.jpg");
    cv::Mat mat = cv::imread(file_path.generic_string());

    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGR, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, mat.ptr());
    glGenerateMipmap(GL_TEXTURE_2D);

    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint program_id = gl::LoadShaders(
        (fs::path(__FILE__).parent_path() / fs::path("../../src/gl/shader/simple_texture.vert")).generic_string().c_str(),
        (fs::path(__FILE__).parent_path() / fs::path("../../src/gl/shader/simple_texture.frag")).generic_string().c_str());

    while (glfwWindowShouldClose(window) == GL_FALSE) {
        // ウィンドウを消去する
        glClear(GL_COLOR_BUFFER_BIT);
        
        // ここで描画処理を行う
        glUseProgram(program_id);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // カラーバッファを入れ替える
        glfwSwapBuffers(window);
        // イベントを取り出す
        glfwWaitEvents();
    }
}
