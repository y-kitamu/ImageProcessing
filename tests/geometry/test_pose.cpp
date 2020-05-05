/*
 * test_pose.cpp
 *
 * Create Date : 2020-05-05 18:10:33
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */
#include <bits/stdc++.h>
#include <Eigen/Eigen>
#include <gtest/gtest.h>

#include <geometry/geometry.hpp>

class PoseTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
        x_norm = Eigen::Vector3d(1, 0, 0);
        y_norm = Eigen::Vector3d(0, 1, 0);
        z_norm = Eigen::Vector3d(0, 0, 1);

        x_trans = Eigen::Vector3d(2, 0, 0);
        y_trans = Eigen::Vector3d(0, 2, 0);
        z_trans = Eigen::Vector3d(0, 0, 2);

        x_90 = Eigen::Quaterniond(Eigen::AngleAxisd(M_PI * 0.5, x_norm));
        y_90 = Eigen::Quaterniond(Eigen::AngleAxisd(M_PI * 0.5, y_norm));
        z_90 = Eigen::Quaterniond(Eigen::AngleAxisd(M_PI * 0.5, z_norm));

        z_30 = Eigen::Quaterniond(Eigen::AngleAxisd(M_PI / 6.0, z_norm));
    }

    Eigen::Vector3d x_norm, y_norm, z_norm;
    Eigen::Vector3d x_trans, y_trans, z_trans;
    Eigen::Quaterniond x_90, y_90, z_90;
    Eigen::Quaterniond z_30;
};

static constexpr double MAX_ERROR = 1e-10;


TEST_F(PoseTest, Quaternion) {
    auto mat = x_90.matrix();
    ASSERT_LT(std::abs(mat(0, 0) - 1.0), MAX_ERROR);
    ASSERT_LT(std::abs(mat(1, 2) + 1.0), MAX_ERROR);
    ASSERT_LT(std::abs(mat(2, 1) - 1.0), MAX_ERROR);

    mat = y_90.matrix();
    ASSERT_LT(std::abs(mat(0, 2) - 1.0), MAX_ERROR);
    ASSERT_LT(mat(2, 2), MAX_ERROR);

    mat = z_30.matrix();
    ASSERT_LT(std::abs(mat(0, 0) - std::sqrt(3.0) * 0.5), MAX_ERROR);
    ASSERT_LT(std::abs(mat(1, 0) - 0.5), MAX_ERROR);
}


TEST_F(PoseTest, RotationTransform) {
    auto res0 = z_90 * x_norm;
    ASSERT_LT((res0 - y_norm).norm(), MAX_ERROR);

    auto res1 = z_90 * y_norm;
    ASSERT_LT((res1 + x_norm).norm(), MAX_ERROR);

    auto res2 = x_90 * z_norm;
    ASSERT_LT((res2 + y_norm).norm(), MAX_ERROR);

    auto res3 = z_30 * x_norm;
    ASSERT_LT(std::abs(res3.x() - std::sqrt(3) * 0.5), MAX_ERROR);
    ASSERT_LT(std::abs(res3.y() - 0.5), MAX_ERROR);

    geometry::Pose pose(Eigen::AngleAxisd(M_PI / 6, z_norm), Eigen::Vector3d::Zero());
    auto res4 = pose.transform(x_norm);
    ASSERT_LT((res3 - res4).norm(), MAX_ERROR);
}

TEST_F(PoseTest, TranslationTransform) {
    geometry::Pose pose(Eigen::AngleAxisd(0, z_norm), x_trans);
    auto res0 = pose.transform(x_norm);
    ASSERT_LT((res0 - Eigen::Vector3d(3.0, 0.0, 0.0)).norm(), MAX_ERROR);
}

TEST_F(PoseTest, Transform) {
    geometry::Pose pose(Eigen::AngleAxisd(M_PI / 6, z_norm), z_trans);
    auto res0 = pose.transform(x_norm);
    ASSERT_LT((res0 - Eigen::Vector3d(std::sqrt(3) * 0.5, 0.5, 2)).norm(), MAX_ERROR);
}

TEST_F(PoseTest, InverseTransform) {
    geometry::Pose pose(z_30, z_trans);
    auto res0 = pose.transform(x_norm);
    res0 = pose.inv_transform(res0);
    ASSERT_LT((res0 - x_norm).norm(), MAX_ERROR);

    auto res1 = pose.inv_transform(z_norm);
    res1 = pose.transform(res1);
    ASSERT_LT((res1 - z_norm).norm(), MAX_ERROR);
}

int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
