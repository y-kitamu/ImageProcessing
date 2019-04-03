#include <opencv2/opencv.hpp>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>


int main() {
    cv::VideoCapture cap0, cap1;
    std::string save_dir = "/home/kitamura/work/opencv/snipets/uvc_video/image/";

    int idx = 0;
    for (;idx < 100; idx++) {
        std::cout << idx << std::endl;
        cap0.open(idx);
        if (cap0.isOpened()) {
            idx++;
            break;
        }
    }

    for (;idx < 100 ;idx++) {
        std::cout << idx << std::endl;
        cap1.open(idx);
        if (cap1.isOpened()) {
            break;
        }
    }

    if (!cap0.isOpened() || !cap1.isOpened()) {
        std::cout << "failed to open camera" << std::endl;
        return -1;
    }
    
    int HIGH_RESO = 10000;

    cap0.set(cv::CAP_PROP_FRAME_WIDTH, HIGH_RESO);
    cap0.set(cv::CAP_PROP_FRAME_HEIGHT, HIGH_RESO);
    cap1.set(cv::CAP_PROP_FRAME_WIDTH, HIGH_RESO);
    cap1.set(cv::CAP_PROP_FRAME_HEIGHT, HIGH_RESO);
    int width0 = cap0.get(cv::CAP_PROP_FRAME_WIDTH);
    int height0 = cap0.get(cv::CAP_PROP_FRAME_HEIGHT);
    int width1 = cap1.get(cv::CAP_PROP_FRAME_WIDTH);
    int height1 = cap1.get(cv::CAP_PROP_FRAME_HEIGHT);

    std::cout << "width : " << width0 << ", height : " << height0 << std::endl;
    std::cout << "width : " << width1 << ", height : " << height1 << std::endl;

    cv::Mat frame0, frame1;

    while (cap0.read(frame0) && cap1.read(frame1)) {
        
        cv::imshow("window0", frame0);
        cv::imshow("window1", frame1);

        const int key = cv::waitKey(1);

        if (key == 'c') {
            auto tp = std::chrono::system_clock::now();
            std::time_t t = std::chrono::system_clock::to_time_t(tp);
            const tm *lt = std::localtime(&t);

            std::stringstream ss;
            ss << std::put_time(lt, "%Y_%m%d_%H%M%S");

            std::string fname_head = save_dir + ss.str();
            
            cv::imwrite(fname_head + "_0.png", frame0);
            cv::imwrite(fname_head + "_1.png", frame1);
        }

        if (key == 'd') {
            cv::Mat frame0_with_kps, frame1_with_kps;
            cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create();
            std::vector<cv::KeyPoint> keypoints0, keypoints1;
            detector->detect(frame0, keypoints0);
            detector->detect(frame1, keypoints1);
            
            std::cout << "kps : " << keypoints0.size() << " , "<< keypoints1.size() << std::endl;
            cv::drawKeypoints(frame0, keypoints0, frame0_with_kps, cv::Scalar(0,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
            cv::drawKeypoints(frame1, keypoints1, frame1_with_kps, cv::Scalar(0,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

            cv::imshow("window with kps0", frame0_with_kps);
            cv::imshow("window with kps1", frame1_with_kps);
        }
        
        if (key == 'q') {
            break;
        }
    }

    cv::destroyAllWindows();
    return 0;
}
