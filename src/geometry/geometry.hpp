/*
 * geometry.hpp
 *
 *
 * Create Date : 2020-05-05 15:12:42
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */

#ifndef GEOMETRY_HPP__
#define GEOMETRY_HPP__

#include <Eigen/Eigen>

namespace geometry {

class Pose {
    /*
     * Camera Pose
     */
  public:
    Pose(Eigen::AngleAxisd angle_axis, Eigen::Vector3d trans): rot(angle_axis), trans(trans) {}

    Eigen::Vector3d transform(const Eigen::Vector3d& pt) {
        return rot * pt + trans;
    }

  private:
    Eigen::Quaterniond rot;
    Eigen::Vector3d trans;
};

}

#endif // GEOMETRY_HPP__
