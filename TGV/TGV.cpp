#include "TGV.hpp"


void TGV::start() {
    double t0 = 1, t1 = 0.5 + std::sqrt(1.25), t2; // TODO : もっといい初期化方法、変数設定、更新方法
    for (int i = 0; i < iter_num; i++) {
        prox_grad_v();
        t2 = nesterov_acc(v0, v1, v_bar, t0, t1);
        t0 = t1;
        t1 = t2;
    }
}

void TGV::prox_grad_v() {
    double t0 = 1, t1 = 0.5 + std::sqrt(1.25), t2; // TODO : もっといい初期化方法、変数設定、更新方法
    std::shared_ptr<cv::Mat> shrink_image;
    for (int i = 0; i < iter_num; i++) {
        shrinkage(shrink_image, alpha_0);
        shrinkage(shrink_image, sigma * alpha_1);
        
        t2 = nesterov_acc(eta0, eta1, eta_bar, t0, t1);
        
        double error = clalc_error();
        if (error < epsilon_p) {
            break;
        }
        
        t0 = t1;
        t1 = t2;
    }
}

double TGV::nesterov_acc(std::shared_ptr<cv::Mat> image0, std::shared_ptr<cv::Mat> image1, std::shared_ptr<cv::Mat> dst,
                         double t0, double t1) {
    double t2 = (1 + std::sqrt(1 + 4 * t1 * t1)) * 0.5;
    double scale = t0 / t2;
    
    float * ptr_i0 = image0->ptr<float>();
    float * ptr_i1 = image1->ptr<float>();
    float * ptr_dst = dst->ptr<float>();

    int stride_x = image0->channels();
    int stride_y = image0->step1();

    int width = image0->cols;
    int height = image0->rows;

    for (int y =0; y < height; y++, ptr_i0 += stride_y, ptr_i1 += stride_y, ptr_dst += stride_y) {
        for (int x = 0; x < width; x++) {
            int offset = x * stride_x;
            for (int k = 0; k < stride_x; k++) {
                int ofs = offset + k;
                ptr_dst[ofs] = ptr_i1[ofs] + scale * (ptr_i1[ofs] - ptr_i0[ofs]);
            }
        }
    }
    
    return t2;
}

void TGV::shrinkage(std::shared_ptr<cv::Mat> image, double lambda) {

}

