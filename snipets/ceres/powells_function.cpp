#include "ceres/ceres.h"
#include "fmt/format.h"

using ceres::Problem;
using ceres::AutoDiffCostFunction;
using ceres::Solver;
using ceres::Solve;

struct F1 {
    template <class T>
    bool operator() (const T* const x1, const T* const x2, T* residual) const {
        residual[0] = x1[0] + T(10.0) * x2[0];
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
        return true;
    }
};


int main(int argc, char ** argv) {
    double x1 = 3.0, x2 = -1.0, x3 = 0.0, x4 = 1.0;

    Problem problem;

    problem.AddResidualBlock(new AutoDiffCostFunction<F1, 1, 1, 1>(new F1), NULL, &x1, &x2);
    problem.AddResidualBlock(new AutoDiffCostFunction<F2, 1, 1, 1>(new F2), NULL, &x3, &x4);
    problem.AddResidualBlock(new AutoDiffCostFunction<F3, 1, 1, 1>(new F3), NULL, &x2, &x3);
    problem.AddResidualBlock(new AutoDiffCostFunction<F4, 1, 1, 1>(new F4), NULL, &x1, &x4);

    Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    Solver::Summary summary;
    Solve(options, &problem, &summary);

    std::cout << summary.BriefReport() << std::endl;
    fmt::print("Final x1 = {}, x2 = {}, x3 = {}, x4 = {}\n", x1, x2, x3, x4);
}
