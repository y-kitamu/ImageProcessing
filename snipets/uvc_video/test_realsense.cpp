#include <string>
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>


void sample1() {
    cv::VideoCapture cap(cv::VideoCaptureAPIs::CAP_INTELPERC);

    if (!cap.isOpened()) {
        std::cout << "failed to open realsense" << std::endl;
        return;
    }

    int HIGH_RESO = 10000;
    
    cap.set(cv::CAP_PROP_FRAME_WIDTH, HIGH_RESO);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, HIGH_RESO);

    int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    
    std::cout << width << " , " << height << std::endl;

    while (true) {
        cap.grab();

        cv::Mat color_frame;
        cap.retrieve(color_frame, cv::CAP_INTELPERC_IMAGE);
        
        cv::Mat depth_frame;
        cap.retrieve(depth_frame, cv::CAP_INTELPERC_DEPTH_MAP);

        cv::Mat infrared_frame;
        cap.retrieve(infrared_frame, cv::CAP_INTELPERC_IR_MAP);

        cv::imshow("color", color_frame);

        depth_frame.convertTo(depth_frame, CV_8U, -255.0 / 10000.0, 255.0);
        cv::imshow("depth", depth_frame);
        cv::imshow("infrared", infrared_frame);

        int key = cv::waitKey(10);
        if (key == 'q') {
            break;
        }
    }

    cv::destroyAllWindows();
}


void sample2() try {
    rs2::colorizer color_map;
    rs2::pipeline pipe;

    pipe.start();

    std::string window_name = "realsense";
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    while (cv::waitKey(1) < 0 && cv::getWindowProperty(window_name, cv::WND_PROP_AUTOSIZE) >= 0)
    {
        rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera
        rs2::frame depth = data.get_depth_frame().apply_filter(color_map);

        // Query frame size (width and height)
        const int w = depth.as<rs2::video_frame>().get_width();
        const int h = depth.as<rs2::video_frame>().get_height();

        // Create OpenCV matrix of size (w,h) from the colorized depth data
        cv::Mat image(cv::Size(w, h), CV_8UC3, (void*)depth.get_data(), cv::Mat::AUTO_STEP);

        // Update the window with new data
        cv::imshow(window_name, image);
    }
} catch (const rs2::error &e) {
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
} catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
}


int main(int argc, char ** argv) {
    sample1();
    // sample2();

    return 0;
}
