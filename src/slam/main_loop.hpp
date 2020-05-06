/*
 * main_loop.hpp
 *
 *
 * Create Date : 2020-05-04 12:07:12
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */

#ifndef MAIN_HPP__
#define MAIN_HPP__

#include <future>
#include <memory>

#include <opencv2/opencv.hpp>
#include <fmt/format.h>

#include "logging/logging.hpp"

#include "tracking.hpp"
#include "local_mapping.hpp"
#include "loop_closing.hpp"


namespace slam {


template <class Tracker=Tracking, class Mapper=LocalMapping, class Closer=LoopClosing>
class MainLoop {
  public:
    MainLoop() {}
    MainLoop(std::string video_filename) {
        cap.open(video_filename);
        if (!cap.isOpened()) {
            logd("Failed to open file : {}\n", video_filename);
            std::exit(EXIT_FAILURE);
        }

        tracker = std::make_unique<Tracker>();
        local_mapper = std::make_unique<Mapper>();
        loop_closer = std::make_unique<Closer>();
    };

    ~MainLoop() {};
    void run() {
        cv::Mat frame;
        for (;;) {
            cap.read(frame);
            if (frame.empty()) {
                logd("Failed to initialize tracking.");
                break;
            }
            if (tracker->initialize(frame)) {
                logd("Successfuly initialized map.");
                break;
            }
            for (int i = 0; i < 20; i++) {
                cap.read(frame);
            }
        }

        for (;;) {
            cap.read(frame);
            if (frame.empty()) {
                logd("Video is finished.");
                break;
            }
        }
    }

  private:
    std::unique_ptr<Tracking> tracker;
    std::unique_ptr<LocalMapping> local_mapper;
    std::unique_ptr<LoopClosing> loop_closer;
    cv::VideoCapture cap;
};

} // namespace slam

#endif // MAIN_HPP__
