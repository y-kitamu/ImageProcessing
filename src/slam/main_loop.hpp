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

#include "tracking.hpp"
#include "local_mapping.hpp"
#include "loop_closing.hpp"


namespace slam {


class IOHandler {
    cv::Mat& read();
};


template <class Tracker=Tracking, class Mapper=LocalMapping, class Closer=LoopClosing>
class MainLoop {
  public:
    MainLoop() {
        tracker = std::make_unique<Tracker>();
        local_mapper = std::make_unique<Mapper>();
        loop_closer = std::make_unique<Closer>();
    };

    ~MainLoop() {};
    void run() {
        while (true) {
            auto fut_tracker = std::async(&Tracking::run, tracker.get());
            fut_tracker.get();
            if (true) {
                auto fut_mapper = std::async(&LocalMapping::run, local_mapper.get());
                fut_mapper.wait();
            }
            if (true) {
                auto fut_closer = std::async(&LoopClosing::run, loop_closer.get());
            }
            fut_tracker.wait();
            break;
        }
    }

  private:
    std::unique_ptr<Tracking> tracker;
    std::unique_ptr<LocalMapping> local_mapper;
    std::unique_ptr<LoopClosing> loop_closer;
};

} // namespace slam

#endif // MAIN_HPP__
