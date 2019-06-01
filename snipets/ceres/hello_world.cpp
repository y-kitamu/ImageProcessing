#include "ceres/ceres.h"
#include "glog/logging.h"

using ceres::AutoDiffCostFunction;
using ceres::NumericDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;


struct CostFunctor {
    template<typename T>
    bool operator()(const T* const x, T* residual) const {
        residual[0] = T(10.0) - x[0];
        return true;
    }
};

struct NumericDiffCostFunctor {
    bool operator() (const double * const x, double * residual) const {
        residual[0] = 10.0 - x[0];
        return true;
    }
};

class QuadraticCostFunction : public ceres::SizedCostFunction<1, 1> {
  public:
    virtual ~QuadraticCostFunction() {}
    virtual bool Evaluate(double const* const* parameters,
                          double* residuals, double** jacobians)  const {
        const double x = parameters[0][0];
        residuals[0] = 10 - x;

        if (jacobians != NULL && jacobians[0] != NULL) {
            jacobians[0][0] = 0;
        }
        return true;
    }
};


int main(int argc, char ** argv) {
    google::InitGoogleLogging(argv[0]);

    double initial_x = 5.0;
    double x = initial_x;

    Problem problem;

    // automatic difference
    // CostFunction * cost_function = new AutoDiffCostFunction<CostFunctor, 1, 1>(new CostFunctor);

    // numerical difference
    // CostFunction * cost_function =
    //     new NumericDiffCostFunction<NumericDiffCostFunctor, ceres::CENTRAL, 1, 1>(
    //         new NumericDiffCostFunctor);

    // analytical differnce
    CostFunction * cost_function = new QuadraticCostFunction();
        
    problem.AddResidualBlock(cost_function, NULL, &x);

    Solver::Options options;

    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    Solver::Summary summary;
    Solve(options, &problem, &summary);

    std::cout << summary.BriefReport() << std::endl;
    std::cout << "x : " << initial_x << " -> " << x << std::endl;
}
