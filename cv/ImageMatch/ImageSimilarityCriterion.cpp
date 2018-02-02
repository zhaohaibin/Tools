//
// Created by zhaohaibin on 1/27/18.
//

#include "ImageSimilarityCriterion.h"

ImageSimilarityCriterion::ImageSimilarityCriterion(const string& file)
        : m_psnr(0)
        , m_imgFile(file)
{
}


ImageSimilarityCriterion::~ImageSimilarityCriterion()
{
}

std::string ImageSimilarityCriterion::getFile() const
{
    return m_imgFile;
}

bool ImageSimilarityCriterion::operator>(const ImageSimilarityCriterion& other) const
{
    double lTotal = m_ssim.val[0] + m_ssim.val[1] + m_ssim.val[2];
    double rTotal = other.m_ssim.val[0] + other.m_ssim.val[1] + other.m_ssim.val[2];
    if (lTotal > rTotal && m_psnr > other.m_psnr)
        return true;
    return ((lTotal - rTotal) * 70 + (m_psnr - other.m_psnr)) >= 0 ? true : false;
}

bool ImageSimilarityCriterion::operator<(const ImageSimilarityCriterion& other) const
{
    return !(*this > other);
}

void ImageSimilarityCriterion::SetSSIM(const cv::Scalar& ssim)
{
    m_ssim = ssim;
}

void ImageSimilarityCriterion::SetPSNR(double psnr)
{
    m_psnr = psnr;
}

const cv::Scalar& ImageSimilarityCriterion::getSSIM() const
{
    return m_ssim;
}

double ImageSimilarityCriterion::getPSNR() const
{
    return m_psnr;
}