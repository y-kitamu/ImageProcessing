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
  private:
    Eigen::Vector3d loc;  // 3-D position in the world coordinate system
    Eigen::Vector3d direction;  // mean unit vector of viewing directions
    cv::Mat orb_descriptor; //
    double dist_max, dist_min; //maximum and minimum distance
};


class KeyFrame {
  private:
    geometry::Pose camera_pose; // rigid body transformation

};


class Map {
  private:
    std::vector<Point> map_points;
    std::vector<KeyFrame> key_frames;

    // covisibility graph
    // spanning tree
};


} // namespace slam


#endif // MAP_HPP__
