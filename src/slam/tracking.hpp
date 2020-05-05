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

namespace slam {

class Tracking {
  public:
    void initialize();

    void run() {
        std::stringstream ss;
        ss << "Tracking thread : "  << std::this_thread::get_id() << std::endl;
        std::cout << ss.str();
        extract_keypoint();
    };

  protected:
    virtual void extract_keypoint() {
        std::stringstream ss;
        ss << "extract_keypoint in Tracking" << std::endl;
        std::cout << ss.str();
    };
    virtual void estimate_init_pose() {}
    virtual void track_local_map() {}
    virtual bool is_create_new_keyframe() {}
};


class TrackingKLT : public Tracking {
    void extract_keypoint() override {
        std::stringstream ss;
        ss << "extract_keypoint in TrackingKLT" << std::endl;
        std::cout << ss.str();
    }
};

} // namespace slam

#endif // TRACKING_HPP__
