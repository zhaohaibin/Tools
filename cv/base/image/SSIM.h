//
// Created by zhaohaibin on 1/27/18.
//

#ifndef IMAGEMATCHER_SSIM_H
#define IMAGEMATCHER_SSIM_H

#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>

class SSIM
{
public:
    SSIM();
    ~SSIM();

    cv::Scalar GetSSIMValue(const cv::Mat& img1, const cv::Mat& img2);
};


#endif //IMAGEMATCHER_SSIM_H
