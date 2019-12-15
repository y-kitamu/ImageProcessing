/*
 * gl_object.hpp
 * 
 * 
 * Create Date : 2019-12-15 10:01:36
 * Copyright (c) 2019 Yusuke Kitamura <ymyk6602@gmail.com>
 */

#ifndef GL_OBJECT_HPP__
#define GL_OBJECT_HPP__

namespace gl {
class GLObject {
  public:
    virtual ~GLObject() = default;
    virtual void load() = 0;
    virtual void draw() = 0;
};
}

#endif // GL_OBJECT_HPP__
