/*
 * halide_sobel.cpp
 *
 * Create Date : 2019-11-28 22:33:52
 * Copyright (c) 2019 Yusuke Kitamura <ymyk6602@gmail.com>
 */

#include <halide/Halide.h>

class SobelFilter : public Halide::Generator<SobelFilter> {
  public:
    Input<Buffer<uint8_t>> input {"input", 3}; // BGR image (from cv::Mat)
    Output<Buffer<float>> sobel_x {"sobel_x", 2};
    Output<Buffer<float>> sobel_y {"sobel_y", 2};

    void generate() {
        clamped = Halide::BoundaryConditions::repeat_edge(input);
        gray(x, y) = 0.114f * clamped(x, y, 0) + 0.587f * clamped(x, y, 1) + 0.299f * clamped(x, y, 2);
        sobel_x(x, y) = 2.0f * (gray(x + 1, y) - gray(x - 1, y))
            + gray(x + 1, y - 1) - gray(x - 1, y - 1)
            + gray(x + 1, y + 1) - gray(x - 1, y + 1);
        sobel_y(x, y) = 2.0f * (gray(x, y + 1) - gray(x, y - 1))
            + gray(x - 1, y + 1) - gray(x - 1, y - 1)
            + gray(x + 1, y + 1) - gray(x + 1, y - 1);
    }


  private:
    Var x {"x"}, y {"y"};
    Func clamped {"clamped"}, gray {"gray"};
};

HALIDE_REGISTER_GENERATOR(SobelFilter, halide_sobel)

int main(int argc, char **argv) {
  return Halide::Internal::generate_filter_main(argc, argv, std::cerr);
}
