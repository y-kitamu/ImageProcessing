#include "TGV.hpp"

namespace TGV {

void TGV::denoise() {
    double t0 = 1, t1 = 0.5 + std::sqrt(1.25), t2;
    for (int i = 0; i < iter_num; i++) {
        prox_grad_v();
        t2 = nesterov_acc(v0, v1, &v_bar, t0, t1);
        t0 = t1;
        t1 = t2;
    }
}

void TGV::prox_grad_v() {
    double t0 = 1, t1 = 0.5 + std::sqrt(1.25), t2; 
    cv::Mat shrink_image;
    for (int i = 0; i < iter_num; i++) {
        shrinkage(&shrink_image, alpha_0);
        shrinkage(&shrink_image, sigma * alpha_1);
        
        t2 = nesterov_acc(eta0, eta1, &eta_bar, t0, t1);
        
        double error = calc_error();
        if (error < epsilon_p) {
            break;
        }
        
        t0 = t1;
        t1 = t2;
    }
}

double TGV::nesterov_acc(const cv::Mat &image0, const cv::Mat &image1, cv::Mat *dst, double t0, double t1) {
    double t2 = (1 + std::sqrt(1 + 4 * t1 * t1)) * 0.5;
    double scale = t0 / t2;
    
    const float * ptr_i0 = image0.ptr<float>();
    const float * ptr_i1 = image1.ptr<float>();
    float * ptr_dst = dst->ptr<float>();

    int stride_x = image0.channels();
    int stride_y = image0.step1();

    int width = image0.cols;
    int height = image0.rows;

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

void TGV::shrinkage(cv::Mat *image, double lambda) {

}


void TGV::divergence(const cv::Mat &src, cv::Mat *dst) {
    int src_stride_x = src.channels();
    int src_stride_y = src.step1();
    int dst_stride_x = dst->channels();
    int dst_stride_y = dst->step1();

    if (src_stride_x != 3 || dst_stride_x != 2) {
        return;
    }
    
    const float * ptr_src0 = src.ptr<float>();
    const float * ptr_src1 = ptr_src0 + src_stride_y;
    float * ptr_dst = dst->ptr<float>();

     int width = src.cols, height = src.rows;

    for (int y = 0; y < height - 1; y++) {
        const float * ps0 = ptr_src0;
        const float * ps1 = ptr_src1;
        float * pd = ptr_dst;
        
        for (int x = 0; x < width - 1; x++, ps0 += src_stride_x, ps1 += src_stride_x, pd += dst_stride_x) {
            pd[0] = ps0[src_stride_x] - ps0[0] + ps1[2] - ps0[2];
            pd[1] = ps0[src_stride_x + 2] - ps0[2] + ps1[1] - ps0[1];
        }

        pd[0] = ps1[2] - ps0[2];
        pd[1] = ps1[1] - ps0[1];
        
        ptr_src0 = ptr_src1;
        ptr_src1 += src_stride_y;
        ptr_dst += dst_stride_y;
    }
    for (int x = 0; x < width; x++, ptr_src0 += src_stride_x, ptr_src1 += src_stride_x, ptr_dst += dst_stride_x) {
        if (x == width - 1) {
            ptr_dst[0] = 0;
            ptr_dst[1] = 0;
        } else {
            ptr_dst[0] = ptr_src0[src_stride_x] - ptr_src0[0];
            ptr_dst[1] = ptr_src0[src_stride_x + 2] - ptr_src0[2];
        }
    }
}

}
