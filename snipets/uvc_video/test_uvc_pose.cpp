#include <opencv2/opencv.hpp>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>

#include <cmdline/cmdline.h>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

namespace fs = boost::filesystem;

int main(int argc, char ** argv) {
    /*
     * 姿勢推定用の画像を撮影する。
     * カメラの位置を固定して撮影。apriltag を複数枚含む画像。
     * sandbox/kitamura/image/<カメラ名>/pose_sample_<カメラ名>_????.png
     */
    const int HIGH_RESO = 10000;

    cmdline::parser parser;
    parser.add<std::string>("directory", 'd', "image directory", false, "/home/kitamura/work/engine2/sandbox/kitamura/image/");
    parser.add<std::string>("camera", 'c', "camera name (SBC010 or SBC011)", false, "SBC011");
    parser.parse_check(argc, argv);
    
    const std::string camera = parser.get<std::string>("camera");
    const std::string directory = parser.get<std::string>("directory") + camera + "/";
    fs::path path(directory);
    boost::system::error_code error;
    if (!fs::exists(path, error)) {
        bool result = fs::create_directory(path, error);
        if (!result || error) {
            std::cout << "failed to create directory : " << directory << std::endl;
            return -1;
        }
    }

    std::string base_head = "pose_sample_" + boost::algorithm::to_lower_copy(camera) + "_";
    int max_used_idx = 0;
    BOOST_FOREACH(const fs::path &p, std::make_pair(fs::directory_iterator(path), fs::directory_iterator())) {
        if (!fs::is_directory(p)) {
            std::string fname = p.filename().generic_string();
            if (fname.size() >= base_head.size() && std::equal(base_head.begin(), base_head.end(), fname.begin())) {
                int idx = std::atoi(fname.substr(base_head.length(), 4).c_str());
                // std::cout << idx << std::endl;
                max_used_idx = idx > max_used_idx ? idx : max_used_idx;
            }
        }
    }
    std::cout << "max_used_idx : " << max_used_idx << std::endl;
    int idx = max_used_idx + 1;

    cv::VideoCapture cap(0);

    if (!cap.isOpened()) {
        std::cout << "failed to open camera" << std::endl;
        return -1;
    }
    
    cap.set(cv::CAP_PROP_FRAME_WIDTH, HIGH_RESO);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, HIGH_RESO);

    cv::Mat frame;
    
    while (cap.read(frame)) {
        cv::imshow("window", frame);
        
        const int key = cv::waitKey(1);

        if (key == 'c') {
            auto format = boost::format("%s%04d.jpg") % base_head % idx;
            std::string filename = format.str();
            std::cout << filename << std::endl;
            idx++;
            cv::imwrite(directory + filename, frame);
        }

        if (key == 'q') {
            break;
        }
    }

    cv::destroyAllWindows();
    return 0;
}
