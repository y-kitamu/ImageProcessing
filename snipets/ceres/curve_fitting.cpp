#include <random>
#include <sstream>
#include <ostream>
#include <boost/filesystem.hpp>
#include "ceres/ceres.h"
#include "fmt/format.h"

using ceres::Problem;
using ceres::CostFunction;
using ceres::CauchyLoss;
using ceres::AutoDiffCostFunction;
using ceres::Solver;
using ceres::Solve;
namespace fs = boost::filesystem;


struct ExponentialResidual {
    ExponentialResidual(double x, double y) : x_(x), y_(y) {}

    template <typename T>
    bool operator() (const T* const m, const T* const c, T* residual) const {
        residual[0] = T(y_) - exp(m[0] * T(x_) + c[0]);
        return true;
    }
    
  private:
    const double x_, y_;
};


std::vector<double> generate_gaussian_noised_data(
    double m, double c, double sigma, int data_num = 100) {
    /*
     * y = exp(m * i + c) * exp(x^2 / (2 * sigma^2))
     */
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::uniform_real_distribution<> noise_dist(-sigma, sigma);
    std::uniform_real_distribution<> data_dist(0, 10);
    std::vector<double> data;
    
    for (int i = 0; i < data_num; i++) {
        double x = data_dist(engine);
        double r = noise_dist(engine);
        double y = std::exp(m * x + c + r / std::abs(r) * r * r / (2 * sigma * sigma));
        data.emplace_back(x);
        data.emplace_back(y);
        fmt::print("x = {}, y = {}\n", x, y);
    }
    return data;
}


int main () {
    double m = 0.0;
    double c = 0.0;

    int num_observations = 100;
    std::vector<double> data = generate_gaussian_noised_data(0.3, 0.1, 0.2, num_observations);

    data[10] = 0.0;
    data[11] = 20;
    data[22] = 10.0;
    data[23] = 0.0;

    bool is_save_data = true;
    fs::path path("/home/kitamura/work/ImageProcessing/data/curve_fitting.csv");
    if (!fs::exists(path.parent_path())) {
        bool result = fs::create_directory(path.parent_path());
        if (!result) {
            fmt::print("failed to create directory {}\n", path.parent_path().generic_string());
            is_save_data = false;
        }
    }
    
    std::ofstream ofs(path.generic_string());
    if (!ofs.is_open()) {
        fmt::print("failed to open {}\n", path.generic_string());
        is_save_data = false;
    }

    if (is_save_data) {
        for (int i = 0; i < num_observations; i++) {
            ofs << data[2 * i] << ", " << data[2 * i + 1] << ", " << std::endl;
        }
        ofs.close();
    }
    
    Problem problem;
    for (int i = 0; i < num_observations; ++i) {
        CostFunction* cost_function = new AutoDiffCostFunction<ExponentialResidual, 1, 1, 1>(
            new ExponentialResidual(data[2 * i], data[2 * i + 1]));
        problem.AddResidualBlock(cost_function, new CauchyLoss(0.5), &m, &c);
    }

    Solver::Options options;
    options.minimizer_progress_to_stdout = true;
    Solver::Summary summary;
    Solve(options, &problem, &summary);

    std::cout << summary.BriefReport() << std::endl;
    fmt::print("finale m = {}, c = {}\n", m, c);
}
