/*
 * map.cpp
 *
 * Create Date : 2020-05-06 02:20:33
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */
#include "map.hpp"

namespace slam {

Frame::Frame(const cv::Mat& frame) {
    cv::Ptr<cv::Feature2D> ptr =
        cv::ORB::create(500, 1.2f, 8, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31, 20);
    std::vector<cv::KeyPoint> kpts;
    cv::Mat descriptors;
    ptr->detect(frame, kpts);
    ptr->compute(frame, kpts, descriptors);

    is_empty = false;
}

} // namespace slam
