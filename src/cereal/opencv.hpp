#ifndef __CEREAL_OPENCV_HPP__
#define __CEREAL_OPENCV_HPP__

#include <cereal/cereal.hpp>
#include <opencv2/opencv.hpp>


namespace cereal {

template <class Archive>
void save(Archive &ar, const cv::Mat &mat) {
    int rows = mat.rows, cols = mat.cols;
    int type = mat.type();
    bool is_continuous = mat.isContinuous();
    ar(rows, cols, type, is_continuous);
    
    if (is_continuous) {
        ar(binary_data(mat.ptr(), rows * cols  * mat.elemSize()));
    } else {
        const int row_size = cols * mat.elemSize();
        for (int i = 0; i < rows; i++) {
            ar(binary_data(mat.ptr(i), row_size));
        }
    }
}

template <class Archive>
void save(Archive &ar, cv::Mat &mat) {
    int rows, cols, type;
    bool is_continuous;
    ar(rows, cols, type, is_continuous);

    mat.create(rows, cols, type);
    if (is_continuous) {
        ar(binary_data(mat.ptr(), rows * cols * mat.elemSize()));
    } else {
        int row_size = cols * mat.elemSize();
        for (int i = 0; i < rows; i++) {
            ar(binary_data(mat.ptr(i), row_size));
        }
    }
}


template <class Archive, class _Tp>
void save(Archive &ar, const cv::Point_<_Tp> &pt) {
    ar(pt.x, pt.y);
}

template <class Archive, class _Tp>
void load(Archive &ar, cv::Point_<_Tp> &pt) {
    ar(pt.x, pt.y);
}

} // namespace cereal

#endif // __CEREAL_OPENCV_HPP__
