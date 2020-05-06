/*
 * tracking.hpp
 *
 *
 * Create Date : 2020-05-03 14:23:00
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */

#ifndef TRACKING_HPP__
#define TRACKING_HPP__

#include <iostream>
#include <sstream>
#include <future>

#include <opencv2/opencv.hpp>

#include "map.hpp"


namespace slam {

class Tracking {
  public:
    bool initialize(const cv::Mat& current_frame);

    void run() {
        std::stringstream ss;
        ss << "Tracking thread : "  << std::this_thread::get_id() << std::endl;
        std::cout << ss.str();
        extractKeypoint();
    };

  protected:
    virtual void extractKeypoint() {
        std::stringstream ss;
        ss << "extract_keypoint in Tracking" << std::endl;
        std::cout << ss.str();
    };
    virtual void estimateInitPose() {}
    virtual void trackLocalMap() {}
    virtual bool isCreateNewKeyframe() {}

    Frame reference_frame;

    static constexpr int MINIMUM_MATCHES = 50;
};


class TrackingKLT : public Tracking {
    void extractKeypoint() override {
        std::stringstream ss;
        ss << "extract_keypoint in TrackingKLT" << std::endl;
        std::cout << ss.str();
    }
};

} // namespace slam

#endif // TRACKING_HPP__
