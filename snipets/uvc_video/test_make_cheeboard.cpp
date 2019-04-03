#include <opencv2/opencv.hpp>

int main(int argc, char ** argv) {
    int A4_WIDTH = 195; //210; // mm
    int A4_HEIGHT = 276; //297; // mm
    
    double box_length = 10; // mm
    
    int scale = 10;
    int length = box_length * scale;
    cv::Size image_size(A4_WIDTH * scale, A4_HEIGHT * scale);
    
    int cols = 7;
    int rows = 10;
    
    int min_x = (image_size.width - length * cols) / 2;
    int min_y = (image_size.height - length * rows) / 2;

    int max_x = min_x + length * cols;
    int max_y = min_y + length * rows;

    cv::Mat cvimage(cv::Size(image_size), CV_8UC3, cv::Scalar(255, 255, 255));
    cvimage.forEach<cv::Point3_<uint8_t>>([&](cv::Point3_<uint8_t> &pixel, const int * position) {
        if (min_y <= position[0] && position[0] < max_y && min_x <= position[1] && position[1] < max_x) {
            int flag = ((position[0] - min_y) / length + (position[1] - min_x) / length) % 2;
            if (flag) {
                pixel = cv::Point3_<uint8_t>(0, 0, 0);
            }
        }
    });
    cv::imwrite("/home/kitamura/work/opencv/snipets/uvc_video/chessboard.png", cvimage);

    return 0;
}
