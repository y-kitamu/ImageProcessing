#include "ceres/ceres.h"

using ceres::Problem;

struct F1 {
    template <class T>
    bool operator() (const T* const x1, const T* const x2, T* residual) const {
        residual[0] = x1[0] - T(10.0) * x2[0];
        return true;  
    }
};


struct F2 {
    template<class T>
    bool operator() (const T* const x3, const T* const x4, T* residual) const {
        residual[0] = T(std::sqrt(5.0)) * (x3[0] - x4[0]);
        return true;
    }
};


struct F3 {
    template<class T>
    bool operator() (const T* const x2, const T* const x3, T* residual) const {
        residual[0] = (x2[0] - T(2.0) * x3[0]) * (x2[0] - T(2.0) * x3[0]);
        return true;
    }
};

struct F4 {
    template<class T>
    bool operator() (const T* const x1, const T* const x4, T* residual) const {
        residual[0] = T(std::sqrt(10.0)) * (x1[0] - x4[0]) * (x1[0] - x4[0]);
    }
};


int main(int argc, char ** argv) {
    double x1 = 3.0, x2 = -1.0, x3 = 0.0, x4 = 1.0;

    Problem problem;

    problem.AddResidualBlock(new AutoDiffCostFunction)
}
