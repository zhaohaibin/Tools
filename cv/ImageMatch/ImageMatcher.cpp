//
// Created by zhaohaibin on 1/27/18.
//

#include "ImageMatcher.h"
#include "opencv2/imgcodecs.hpp"

ImageMatcher::ImageMatcher()
        : m_caculateBaseSize(100)
        , m_flag(cv::InterpolationFlags::INTER_AREA)
{
}


ImageMatcher::~ImageMatcher()
{
}

vector<std::string> ImageMatcher::MatchImag(
        const string& srcImgFile,
        const vector<string>& targetFiles,
        int maxCount)
{
    return DoMatch(srcImgFile, targetFiles, maxCount);
}

cv::Scalar ImageMatcher::CaculateSSIM(const cv::Mat& img1, const cv::Mat& img2)
{
    return m_ssim.GetSSIMValue(img1, img2);
}

double ImageMatcher::CaculatePSNR(const cv::Mat& img1, const cv::Mat& img2)
{
    return m_psnr.GetPSNRValue(img1, img2);
}

vector<std::string> ImageMatcher::DoMatch(
        const string& srcImgFile,
        const vector<string>& targetFiles,
        int maxCount)
{
    list<ImageSimilarityCriterion> iscList;
    vector<std::string> files;
    cv::Mat srcImg = cv::imread(srcImgFile, cv::IMREAD_COLOR);
    cv::resize(srcImg, srcImg, cv::Size(m_caculateBaseSize, m_caculateBaseSize), 0, 0, m_flag);
    for (int i = 0; i < targetFiles.size(); ++i)
    {
        cv::Mat targetImg = cv::imread(targetFiles[i], cv::IMREAD_COLOR);
        cv::resize(targetImg, targetImg, cv::Size(m_caculateBaseSize, m_caculateBaseSize), 0, 0, m_flag);

        ImageSimilarityCriterion isc(targetFiles[i]);
        DoMatch(srcImg, targetImg, isc);
        HandleSimilarityCriterion(iscList, isc, maxCount);
    }

    iscList.sort(greater<ImageSimilarityCriterion>());
    list<ImageSimilarityCriterion>::iterator it = iscList.begin();
    for (; it != iscList.end(); ++it)
        files.push_back(it->getFile());
    return files;
}

void ImageMatcher::DoMatch(const cv::Mat& img1, const cv::Mat& img2, ImageSimilarityCriterion& isc)
{
    double psnr = CaculatePSNR(img1, img2);
    cv::Scalar scalar = CaculateSSIM(img1, img2);
    isc.SetPSNR(psnr);
    isc.SetSSIM(scalar);
}

void ImageMatcher::HandleSimilarityCriterion(
        std::list<ImageSimilarityCriterion>& iscList,
        const ImageSimilarityCriterion& isc,
        int maxCount)
{
    const cv::Scalar& scalar = isc.getSSIM();
    double totalVal = scalar.val[0] + scalar.val[1] + scalar.val[2];
    if (totalVal < 1.2 && isc.getPSNR() < 15)
        return;
    if (iscList.size() == maxCount)
    {
        list<ImageSimilarityCriterion>::iterator it = iscList.begin();
        if (isc > *it)
        {
            iscList.erase(it);
            iscList.push_back(isc);
        }
    }
    else
    {
        iscList.push_back(isc);
    }
    iscList.sort();
}
