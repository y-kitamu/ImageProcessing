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
    MainLoop(std::string video_filename) {
        cap.open(video_filename);
        if (!cap.isOpened()) {
            logging::logd("Failed to open file : {}\n", video_filename);
            std::exit(EXIT_FAILURE);
        }

        tracker = std::make_unique<Tracker>();
        local_mapper = std::make_unique<Mapper>();
        loop_closer = std::make_unique<Closer>();
    };

    ~MainLoop() {};
    void run() {
        cv::Mat cur_frame, ref_frame;
        for (;;) {
            cap.read(cur_frame);
            if (cur_frame.empty()) {
                logging::logd("Failed to initialize tracking");
                break;
            }

        }

        for (;;) {
            cap.read(cur_frame);
            if (cur_frame.empty()) {
                logging::logd("Video is finished.");
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
