/*
 * map.hpp
 *
 *
 * Create Date : 2020-05-03 14:24:38
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */

#ifndef MAP_HPP__
#define MAP_HPP__

#include <vector>

#include <Eigen/Eigen>
#include <opencv2/opencv.hpp>

#include <geometry/geometry.hpp>

namespace slam {

class Point {
  public:

  private:
    Eigen::Vector3d loc;  // 3-D position in the world coordinate system
    Eigen::Vector3d direction;  // mean unit vector of viewing directions
    cv::Mat orb_descriptor; //
    double dist_max, dist_min; //maximum and minimum distance
    bool is_map_point;
};

class MapPoint : public Point {

};


class Frame {
  public:
    Frame() {}
    Frame(const cv::Mat& frame);

    bool empty() {
        return is_empty;
    }

  private:
    bool is_empty = true;

  public:
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
};


class KeyFrame {
  public:
    KeyFrame() {}

  private:
    geometry::Pose camera_pose; // rigid body transformation (world coord to camera coord)
    cv::Mat intrinsic; // camera intrinsic
    std::vector<Point> feature_points;
};


class Map {
  private:
    std::vector<MapPoint> map_points;
    std::vector<KeyFrame> key_frames;
};


} // namespace slam


#endif // MAP_HPP__
