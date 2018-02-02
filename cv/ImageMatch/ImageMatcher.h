//
// Created by zhaohaibin on 1/27/18.
//

#ifndef IMAGEMATCHER_IMAGEMATCHER_H
#define IMAGEMATCHER_IMAGEMATCHER_H

#include <vector>
#include <string>
#include <list>

#include "SSIM.h"
#include "PSNR.h"
#include "ImageSimilarityCriterion.h"

#include <opencv2/imgproc.hpp>


class ImageMatcher
{
public:
    ImageMatcher();
    ~ImageMatcher();

    vector<string> MatchImag(
            const string& srcImgFile,
            const vector<string>& targetFiles,
            int maxCount);

private:
    cv::Scalar CaculateSSIM(const cv::Mat& img1, const cv::Mat& img2);

    double CaculatePSNR(const cv::Mat& img1, const cv::Mat& img2);

    vector<std::string>  DoMatch(const string& srcImgFile,
                                 const vector<string>& targetFiles,
                                 int maxCount);

    void DoMatch(const cv::Mat& img1, const cv::Mat& img2, ImageSimilarityCriterion& isc);

    void HandleSimilarityCriterion(
            std::list<ImageSimilarityCriterion>& iscList,
            const ImageSimilarityCriterion& isc,
            int maxCount);
private:
    SSIM m_ssim;
    PSNR m_psnr;
    int m_caculateBaseSize;
    cv::InterpolationFlags m_flag;

};


#endif //IMAGEMATCHER_IMAGEMATCHER_H
