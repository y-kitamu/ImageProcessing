#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <cmdline/cmdline.h>


cv::Mat scanImageAndReduceC(cv::Mat & I, const uint8_t *table) {

}


cv::Mat scanImageAndReduceIterator(cv::Mat &I, const uint8_t *table) {
    
}


cv::Mat scanImageAndRedceRandomAccess(cv::Mat &I, const uint8_t *table) {
    
}


int main(int argc, char ** argv) {
    /* color space reduction */
    
    cmdline::parser parser;
    parser.add<std::string>("src", 's', "source filename", false, "~/");
    parser.parse_check(argc, argv);

    auto tp0 = std::chrono::system_clock::now();

    auto tp1 = std::chrono::system_clock::now();

    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(tp1 - tp0).count();


}
