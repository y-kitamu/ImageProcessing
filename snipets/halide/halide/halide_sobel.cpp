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
    Output<Buffer<float>> output_gray {"output_gray", 2};
    Output<Buffer<float>> sobel_x {"sobel_x", 2};
    Output<Buffer<float>> sobel_y {"sobel_y", 2};

    void generate() {
        clamped = Halide::BoundaryConditions::repeat_edge(input);
        // gray(x, y) = 0.114f * clamped(x, y, 0) + 0.587f * clamped(x, y, 1) + 0.299f * clamped(x, y, 2);
        gray(x, y) = clamped(x, y, 0);
        output_gray(x, y) = gray(x, y) / 255.f;
        sobel_x(x, y) = 2.0f * (gray(x + 1, y) - gray(x - 1, y))
            + gray(x + 1, y - 1) - gray(x - 1, y - 1)
            + gray(x + 1, y + 1) - gray(x - 1, y + 1);
        sobel_y(x, y) = 2.0f * (gray(x, y + 1) - gray(x, y - 1))
            + gray(x - 1, y + 1) - gray(x - 1, y - 1)
            + gray(x + 1, y + 1) - gray(x + 1, y - 1);
    }

    void schedule() {
        input
            .dim(0).set_stride(3) // stride in dimension 0 (x) is three
            .dim(2).set_stride(1); // stride in dimension 2 (c) is one
        input.dim(2).set_bounds(0, 3); // Dimension 2 (c) starts at 0 and has extent 3.
        //input.dim(0).set_stride(Expr()); // Use an undefined Expr to
    }
    
  private:
    Var x {"x"}, y {"y"};
    Func clamped {"clamped"}, gray {"gray"};
};

HALIDE_REGISTER_GENERATOR(SobelFilter, halide_sobel)

int main(int argc, char **argv) {
  return Halide::Internal::generate_filter_main(argc, argv, std::cerr);
}
