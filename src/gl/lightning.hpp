#ifndef __GL_LIGHTNING_HPP__
#define __GL_LIGHTNING_HPP__

#include "base.hpp"


namespace gl {

class LightningGL : public BaseGL {
    /*
     * 光の量、位置を変化させる class
     */
  public:
    void loadGLObjects() override;
    void drawGL() override;
    void drawImgui() override;

  public:
    GLuint vao;
};

} // namespace gl


#endif //__GL_LIGHTNING_HPP__
