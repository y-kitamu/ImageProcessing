#include <opencv2/opencv.hpp>
#include <sstream>
#include <vector>
#include <string>

#include <cmdline/cmdline.h>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>


int main(int argc, char ** argv) {
    /*
     * ファイルを保存する
     *
     */
    const int HIGH_RESO = 10000;
    const std::string directory = "/home/kitamura/work/engine2/sandbox/kitamura/image/";

    cmdline::parser parser;

    cv::VideoCapture cap(0);

    if (!cap.isOpened()) {
        std::cout << "failed to open camera" << std::edl;
        return -1;
    }
    
    cap.set(cv::CAP_PROP_FRAME_WIDTH, HIGH_RESO);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, HIGH_RESO);


    cv::Mat frame;
    
    while (cap.read(frame)) {

        cv::imshow("window", frame);

        const int key = cv::waitKey(1);

        if (key == 'c') {
            cv::imwrite()
        }
    }
}

