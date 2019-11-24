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

#include <opencv2/opencv.hpp>

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

    // ウィンドウが開いている間繰り返す
    while (glfwWindowShouldClose(window) == GL_FALSE) {
        // ウィンドウを消去する
        glClear(GL_COLOR_BUFFER_BIT);
        
        // ここで描画処理を行う
        
        // カラーバッファを入れ替える
        glfwSwapBuffers(window);
        // イベントを取り出す
        glfwWaitEvents();
    }
}
