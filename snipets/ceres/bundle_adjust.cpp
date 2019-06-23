#include "ceres/ceres.h"
#include "fmt/format.h"

using Ceres::Problem;
using Ceres::CostFunction;
using Ceres::AutoDiffCostFunction;
using Ceres::Solver;
using Ceres::Solve;

class BALProblem {
  public:
    int num_observations() const { return num_observations_; }
    const double* observations() const { return observations_; }
    double * mutable_cameras() { return parameters_; }
    double * mutable_points() { return parameters_ + 9 * num_cameras_; }

  private:
    int num_cameras_;
    int num_points_;
    int num_observations_;
    int num_parameters_;

    int* point_index_;
    int* camera_index_;
    double* observations_;
    double* parameters_;
};

struct SnavelyReprojectionError {
    SnavelyReprojectionError(double observed_x, double obserbed_y)
        : observed_x(observed_x), observed_y(observed_y) {}

    bool operator() (const T* const camera, const T* const point, T* residuals) const {
        
    }
    
  private:
    double observed_x, observed_y;
};

int main() {
    
}
