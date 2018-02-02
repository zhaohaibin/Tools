//
// Created by zhaohaibin on 1/27/18.
//

#ifndef IMAGEMATCHER_IMAGESIMILARITYCRITERION_H
#define IMAGEMATCHER_IMAGESIMILARITYCRITERION_H

#include <string>
#include <opencv2/core/types.hpp>

using namespace std;

class ImageSimilarityCriterion
{
public:
    ImageSimilarityCriterion(const string& file);
    ~ImageSimilarityCriterion();

public:
    string getFile() const;
public:
    bool operator > (const ImageSimilarityCriterion& other) const;
    bool operator < (const ImageSimilarityCriterion& other) const;
public:
    void SetSSIM(const cv::Scalar& ssim);
    void SetPSNR(double psnr);
    const cv::Scalar& getSSIM() const;
    double getPSNR() const;
private:
    string m_imgFile;
    cv::Scalar m_ssim;
    double m_psnr;
};


#endif //IMAGEMATCHER_IMAGESIMILARITYCRITERION_H
