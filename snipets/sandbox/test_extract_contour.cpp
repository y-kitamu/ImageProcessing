#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
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
    
    cv::Mat raw = cv::imread(parser.get<std::string>("src_image"));
    fmt::print("col x row x channel = {} x {} x {}\n", raw.cols, raw.rows, raw.channels());
    fmt::print("elemsize : {}, step : {}\n", raw.elemSize(), raw.step);

    cv::Mat img = raw;
    if (img.type() == CV_8UC3) {
        cv::cvtColor(raw, img, cv::COLOR_BGR2GRAY);
    }
    
    cv::Mat thresh;
    cv::threshold(img, thresh, 0, 0, cv::THRESH_OTSU);

    gl::SimpleGL window = gl::SimpleGL();
    window.set_frame(raw);
    window.set_frame(img);
    window.set_frame(thresh);
    window.draw();
    // gl::SimpleGL window = gl::SimpleGL();
    // window.set_frame(img);
    // window.draw();
    
    // cv::namedWindow("window");
    // cv::imshow("window", img);
    // cv::waitKey();
}
