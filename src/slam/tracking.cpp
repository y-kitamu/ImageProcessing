/*
 * tracking.cpp
 *
 * Create Date : 2020-05-05 15:10:51
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */
#include "tracking.hpp"

#include <future>

#include "map.hpp"
#include "logging/logging.hpp"


namespace slam {

void calcHomography() {
    logging::logd("calcHomography");
}

void calcFundamentalMatrix() {
    logging::logd("calcFundamentalmatrix");
}

bool Tracking::initialize(const cv::Mat& current_image) {
    /*
     * initialize map
     * If map is initialized successfuly, return true, else return false.
     */
    if (reference_frame.empty()) {
        reference_frame = Frame(current_image);
        return false;
    }
    Frame current_frame(current_image);

    auto fut_homography = std::async(calcHomography);
    auto fut_fundamental_mat = std::async(calcFundamentalMatrix);

    fut_homography.wait();
    fut_fundamental_mat.wait();
}

} // namespace slam
