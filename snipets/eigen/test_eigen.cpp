#include <iostream>
#include <Eigen/Eigen>

int main() {
    /*
     * cmake の include_directories で　SYSTEM をつけて
     * include_directories(SYSTEM AFTER ext/eigen)
     * みたいにして、 Eigen の warning が出ないようにする。
     */
    Eigen::Matrix3d mat;
    mat <<
        1, 2, 3,
        4, 5, 6,
        7, 8, 9;

    std::cout << mat << std::endl;
}
