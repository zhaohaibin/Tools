//
// Created by zhaohaibin on 1/27/18.
//

#include "PSNR.h"
#include <opencv2/core.hpp>

PSNR::PSNR()
{
}

PSNR::~PSNR()
{
}

double PSNR::GetPSNRValue(const cv::Mat& img1, const cv::Mat& img2)
{
    cv::Mat s1;
    absdiff(img1, img2, s1);       // |I1 - I2|
    s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
    s1 = s1.mul(s1);           // |I1 - I2|^2

    cv::Scalar s = sum(s1);         // sum elements per channel

    double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

    if (sse <= 1e-10) // for small values return zero
        return 0;
    else
    {
        double  mse = sse / (double)(img1.channels() * img1.total());
        double psnr = 10.0*log10((255 * 255) / mse);
        return psnr;
    }
}