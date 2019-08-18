#ifndef __CEREAL_EIGEN_HPP__
#define __CEREAL_EIGEN_HPP__

#include <cereal/cereal.hpp>
#include <Eigen/Eigen>

namespace cereal {

template <class Archive, class _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols> inline
typename std::enable_if<traits::is_output_serializable<BinaryData<_Scalar>, Archive>::value, void>::type
save(Archive & ar, Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> const & m) {
    int32_t rows = m.rows();
    int32_t cols = m.cols();
    ar(rows, cols);
    ar(binary_data(m.data(), rows * cols * sizeof(_Scalar)));
}

template <class Archive, class _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols> inline
typename std::enable_if<traits::is_input_serializable<BinaryData<_Scalar>, Archive>::value, void>::type
load(Archive & ar, Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> & m) {
    int32_t rows;
    int32_t cols;
    ar(rows, cols);

    m.resize(rows, cols);

    ar(binary_data(m.data(), static_cast<std::size_t>(rows * cols * sizeof(_Scalar))));
}

} // namespace cereal

#endif // __CEREAL_EIGEN_HPP__
