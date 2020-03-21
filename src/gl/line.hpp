#ifndef __GL_LINE_HPP__
#define __GL_LINE_HPP__

#include <memory>

#include <boost/filesystem.hpp>

#include "point.hpp"

namespace fs = boost::filesystem;

namespace gl {

class Line {
    /*
     * debug 画面に直線を描画する class
     */
  public:
    Line(std::shared_ptr<Point> pt1, std::shared_ptr<Point> pt2, Eigen::Vector4f color)
        : line_segment({pt1, pt2}), color(color) {}
    void load(int frame_idx);
    void load(int frame_idx0, int frame_idx1);
    void load(Eigen::Vector2d gl_pt0, Eigen::Vector2d gl_pt1);
    void draw();

    std::pair<std::shared_ptr<Point>, std::shared_ptr<Point>> getLineSegment() { return line_segment; }
    
  private:
    std::pair<std::shared_ptr<Point>, std::shared_ptr<Point>> line_segment;
    Eigen::Vector4f color;
    GLuint vao, vbo;
    int vertices = 2;
    bool is_active = false;
};


class Lines {
  public:
    static void setShader();
    
    void addLine(std::shared_ptr<Point> pt1, std::shared_ptr<Point> pt2,
                 Eigen::Vector4f color=Eigen::Vector4f(1.0, 1.0, 1.0, 0.5));
    void load();
    void draw();

  private:
    inline static const std::string shader_basename = "line";
    std::vector<std::shared_ptr<Line>> lines;
  public:
    inline static GLuint program_id;
};

} // namespace gl

#endif  // __GL_LINE_HPP__
