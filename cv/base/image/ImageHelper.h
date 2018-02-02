//
// Created by zhaohaibin on 1/27/18.
//

#ifndef IMAGEMATCHER_IMAGEHELPER_H
#define IMAGEMATCHER_IMAGEHELPER_H

#include <string>

#include <boost/filesystem.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>

using namespace boost;
using namespace std;

class ImageHelper
{

public:
    ImageHelper();
    ~ImageHelper();

public:

    //////////////////////////////////////////////////////////////////////////
    ////////////////////////生成缩略图//////////////////////////////////////
    static bool GenrateThumbnail(
            const string& srcImage,
            const string& savePath,
            int maxSize,
            cv::ImreadModes readFlag = cv::ImreadModes::IMREAD_COLOR,
            cv::InterpolationFlags flag = cv::InterpolationFlags::INTER_AREA
    );

    static bool GenrateThumbnail(
            const filesystem::path& srcImage,
            const filesystem::path& savePath,
            int maxSize,
            cv::ImreadModes readFlag = cv::ImreadModes::IMREAD_COLOR,
            cv::InterpolationFlags flag = cv::InterpolationFlags::INTER_AREA);

    //////////////////////////////////////////////////////////////////////////
    /////////////////////////合并图片///////////////////////////////////////
    static bool MergeMatWith4C(cv::Mat& descImg, const cv::Mat& srcImg, double scale);

    //////////////////////////////////////////////////////////////////////////
    ////////////////////将图片透明背景设置为白色背景/////////////////////
    static bool ChangeTransparentBackgroundToSolidColor(
            const filesystem::path& filePath, const cv::Scalar& scalar);

    static bool ChangeTransparentBackgroundToSolidColorV2(
            const filesystem::path& filePath, const cv::Scalar& scalar);

    static bool ChangeRGBA2RGB(const filesystem::path& filePath);
};


#endif //IMAGEMATCHER_IMAGEHELPER_H
