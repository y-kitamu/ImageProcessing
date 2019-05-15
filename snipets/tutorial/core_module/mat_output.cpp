#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <Eigen/Eigen>


int main() {
    cv::Mat M(2, 2, CV_8UC3, cv::Scalar(0, 0, 255));
    std::cout << "M (default) = " << std::endl << " " <<  M << std::endl << std::endl;
    
    std::cout << "M (python) = " << std::endl << " " <<  cv::format(M, cv::Formatter::FMT_PYTHON) << std::endl << std::endl;
    std::cout << "M (numpy) = " << std::endl << " " << cv::format(M, cv::Formatter::FMT_NUMPY)  << std::endl << std::endl;
    std::cout << "M (csv) = " << std::endl << " " <<  cv::format(M, cv::Formatter::FMT_CSV) << std::endl  << std::endl;


    // ここの実装はどうやっているのか？？？
    int size = 20;
    std::vector<cv::Point2f> vpts(size);

    for (int i= 0; i < size; i++) {
        vpts[i] = cv::Point2f((float)(i * 5), (float)(i % 7));
    }

    std::cout << "A Vector of 2D Points = " << vpts << std::endl;


    return 0;
}
