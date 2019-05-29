#ifndef __GL_LIGHTNING_HPP__
#define __GL_LIGHTNING_HPP__

#include "base.hpp"


namespace gl {

class LightningGL : public BaseGL {
    /*
     * 光の量、位置を変化させる class
     */
  public:
    void load_gl_objects() override;
    void draw_gl() override;
    void draw_imgui() override;

  public:
    GLuint vao;
};

} // namespace gl


#endif //__GL_LIGHTNING_HPP__
