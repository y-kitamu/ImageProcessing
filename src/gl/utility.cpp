/*
 * utility.cpp
 *
 * Create Date : 2019-12-15 12:55:33
 * Copyright (c) 2019 Yusuke Kitamura <ymyk6602@gmail.com>
 */
#include "utility.hpp"

namespace gl {
GLuint setShader(std::string vertex_shader_name, std::string fragment_shader_name) {
    return LoadShaders(vertex_shader_name.c_str(), fragment_shader_name.c_str());
}

}
