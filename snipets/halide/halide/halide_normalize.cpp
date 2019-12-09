/*
 * halide_normalize.cpp
 *
 * Create Date : 2019-12-09 21:13:07
 * Copyright (c) 2019 Yusuke Kitamura <ymyk6602@gmail.com>
 */
#include <halide/Halide.h>

class Normalize : public Halide::Generator<Normalize> {
  public:
    Input<Buffer<float>> input {"input", 2};
    Input<float> maximum {"maximum"};
    Input<float> minimum {"minimum"};
    Output<Buffer<float>> output {"output", 2};

    void generate() {
        Expr scale = 1.f / (maximum - minimum);
        output(x, y) = (input(x, y) - minimum) * scale;
    }

  private:
    Var x {"x"}, y {"y"};
};

HALIDE_REGISTER_GENERATOR(Normalize, halide_normalize)

int main(int argc, char **argv) {
  return Halide::Internal::generate_filter_main(argc, argv, std::cerr);
}
    
