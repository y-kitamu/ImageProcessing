#ifndef __TGV__  // TODO : rename include guard
#define __TGV__

#include <memory>
#include <opencv2/opencv.hpp>


class TGV {
    // TGV denoising class
  public:
    void start(); // TODO : rename
    
  private:
    void prox_grad_v(); // proximal gradient methods (eq.(4.5) top)
    void shrinkage(std::shared_ptr<cv::Mat> image, double lambda);  // (eq.(4.10) top)
    double nesterov_acc(std::shared_ptr<cv::Mat> image0, std::shared_ptr<cv::Mat> image1, std::shared_ptr<cv::Mat> dst,
                        double t0, double t1); // nesterov accerelation (eq.(4.5) middle & bottom)
    double clalc_error();

    std::shared_ptr<cv::Mat> sym_grad(std::shared_ptr<cv::Mat> image);
    std::shared_ptr<cv::Mat> divergence(std::shared_ptr<cv::Mat> image);
    std::shared_ptr<cv::Mat> square_sym_grad(std::shared_ptr<cv::Mat> image);
    std::shared_ptr<cv::Mat> square_divergence(std::shared_ptr<cv::Mat> iamge);

  private:
    std::shared_ptr<cv::Mat> input, output;
    std::shared_ptr<cv::Mat> v0, v1, v_bar; // v0 : v_k, v1 : v_{k + 1}
    std::shared_ptr<cv::Mat> q, eta0, eta1, eta_bar; // eta0 : eta_k, eta1 : eta_{k + 1}
    double epsilon_p = 1e-4; // stop criterion
    double sigma = 0.125;    // step width (in update eta)
    double alpha_0 = 0.1, alpha_1 = 0.05;
    double iter_num = 1500;
};

#endif // __TGV__
