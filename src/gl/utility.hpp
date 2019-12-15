/*
 * utility.hpp
 * 
 * 
 * Create Date : 2019-12-15 12:47:17
 * Copyright (c) 2019 Yusuke Kitamura <ymyk6602@gmail.com>
 */

#ifndef GL_UTILITY_HPP__
#define GL_UTILITY_HPP__

#include <string>

#include "base_include.hpp"

namespace gl {

GLuint setShader(std::string vertex_shader_name, std::string fragment_shader_name);

}

#endif // GL_UTILITY_HPP__
