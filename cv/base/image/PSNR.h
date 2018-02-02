//
// Created by zhaohaibin on 1/27/18.
//

#ifndef IMAGEMATCHER_PSNR_H
#define IMAGEMATCHER_PSNR_H

#include <opencv2/core/mat.hpp>
class PSNR
{
public:
    PSNR();
    ~PSNR();

    double GetPSNRValue(const cv::Mat& img1, const cv::Mat& img2);

};


#endif //IMAGEMATCHER_PSNR_H
