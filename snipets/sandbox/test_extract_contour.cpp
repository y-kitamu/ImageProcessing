#include <opencv2/opencv.hpp>
#include <cmdline/cmdline.h>
#include <cereal/cereal.hpp>
#include <fmt/format.h>

#include "gl/simple.hpp"
#include "cereal/opencv.hpp"

int main(int argc, char ** argv) {
    // ネットで拾ってきた洋服のサンプル画像の輪郭を抽出
    cmdline::parser parser;

    parser.add<std::string>("src_image", 's', "cloth sample image", false,
                            "/home/kitamura/work/opencv/data/img/sample_pants.jpg");
    parser.parse_check(argc, argv);
    
    cv::Mat img = cv::imread(parser.get<std::string>("src_image"), cv::IMREAD_UNCHANGED);
    fmt::print("{} x {}\n", img.cols, img.rows);
    
    gl::SimpleGL window = gl::SimpleGL();
    window.set_frame(img);
    window.draw();
}
