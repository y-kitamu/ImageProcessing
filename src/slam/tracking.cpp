/*
 * tracking.cpp
 *
 * Create Date : 2020-05-05 15:10:51
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */
#include "tracking.hpp"

#include <future>
#include <chrono>

#include "map.hpp"
#include "logging/logging.hpp"
#include "gl/base.hpp"


namespace slam {

void calcHomography() {
    logd("calcHomography");
}

void calcFundamentalMatrix() {
    logd("calcFundamentalmatrix");
}

bool Tracking::initialize(const cv::Mat& current_image) {
    /*
     * initialize map
     * If map is initialized successfuly, return true, else return false.
     */
    {
        auto window = gl::BaseGL::getInstance();
        window.addFrame(current_image);
    }

    if (reference_frame.empty()) {
        reference_frame = Frame(current_image);
        return false;
    }
    Frame current_frame(current_image);

    std::vector<cv::DMatch> matches;
    cv::BFMatcher matcher(cv::NORM_HAMMING, true);
    // cv::FlannBasedMatcher matcher;
    // if (current_frame.descriptors.type() != CV_32F) {
    //     current_frame.descriptors.convertTo(current_frame.descriptors, CV_32F);
    // }
    // if (reference_frame.descriptors.type() != CV_32F) {
    //     reference_frame.descriptors.convertTo(reference_frame.descriptors, CV_32F);
    // }

    matcher.match(current_frame.descriptors, reference_frame.descriptors, matches);

    logd("match_num : {}", matches.size());

    if (matches.size() < MINIMUM_MATCHES) {
        reference_frame = std::move(current_frame);
        return false;
    }

    auto fut_homography = std::async(calcHomography);
    auto fut_fundamental_mat = std::async(calcFundamentalMatrix);

    fut_homography.wait();
    fut_fundamental_mat.wait();

    {
        auto ref_image = gl::BaseGL::frames[gl::BaseGL::frames.size() - 2];
        auto cur_image = gl::BaseGL::frames[gl::BaseGL::frames.size() - 1];

        auto add_pts = [] (std::shared_ptr<gl::Image> img, std::vector<cv::KeyPoint> kpts) {
            logd("point num : {}", kpts.size());
            for (auto &&kp : kpts) {
                img->addPoint(Eigen::Vector2d(kp.pt.x, kp.pt.y));
            }
        };

        add_pts(ref_image, reference_frame.keypoints);
        add_pts(cur_image, current_frame.keypoints);

        auto window = gl::BaseGL::getInstance();

        for (auto &&match : matches) {
            auto cur_kpt = current_frame.keypoints[match.queryIdx];
            auto ref_kpt = reference_frame.keypoints[match.trainIdx];

            auto cur_pt = ref_image->addPoint(Eigen::Vector2d(ref_kpt.pt.x, ref_kpt.pt.y),
                                              Eigen::Vector4f(1.0, 0.0, 0.0, 0.5));
            auto ref_pt = cur_image->addPoint(Eigen::Vector2d(cur_kpt.pt.x, cur_kpt.pt.y),
                                              Eigen::Vector4f(1.0, 0.0, 0.0, 0.5));
            window.addLine(cur_pt, ref_pt);
        }
    }

    return true;
}

} // namespace slam
