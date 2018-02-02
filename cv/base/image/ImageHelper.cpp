//
// Created by zhaohaibin on 1/27/18.
//

#include "ImageHelper.h"
#include <opencv2/core/mat.hpp>
#include <opencv2/core/utility.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

string uuid_string()
{
    uuids::random_generator rgen;
    uuids::uuid u = rgen();
    std::stringstream strstream;
    strstream << u;
    return strstream.str();
}

ImageHelper::ImageHelper()
{
}

ImageHelper::~ImageHelper()
{
}

bool ImageHelper::GenrateThumbnail(
        const string& srcImage,
        const string& savePath,
        int maxSize,
        cv::ImreadModes readFlag /*= cv::ImreadModes::IMREAD_COLOR*/,
        cv::InterpolationFlags flag /*= cv::InterpolationFlags::INTER_AREA */)
{
    return GenrateThumbnail(filesystem::path(srcImage), filesystem::path(savePath), maxSize, readFlag, flag);
}

bool ImageHelper::GenrateThumbnail(
        const filesystem::path& srcImage,
        const filesystem::path& savePath,
        int maxSize,
        cv::ImreadModes readFlag /*= cv::ImreadModes::IMREAD_COLOR*/,
        cv::InterpolationFlags flag /*= cv::InterpolationFlags::INTER_AREA*/)
{
    cv::Mat srcMat = cv::imread(srcImage.generic_string().c_str(), readFlag);
    int row = srcMat.rows;
    int col = srcMat.cols;

    double scale = (row > col ? row : col) / maxSize;
    cv::Mat thumbnail;
    cv::resize(srcMat, thumbnail, cv::Size(col / scale, row / scale), 0, 0, flag);
    imwrite(savePath.generic_string(), thumbnail);
    return true;
}

bool ImageHelper::MergeMatWith4C(cv::Mat& descImg, const cv::Mat& srcImg, double scale)
{
    if (descImg.channels() != 3 || srcImg.channels() != 4)
        return true;
    if (scale < 0.01)
        return false;

    std::vector<cv::Mat>scr_channels;
    std::vector<cv::Mat>dstt_channels;
    split(srcImg, scr_channels);
    split(descImg, dstt_channels);
    CV_Assert(scr_channels.size() == 4 && dstt_channels.size() == 3);

    if (scale < 1)
    {
        scr_channels[3] *= scale;
        scale = 1;
    }
    for (int i = 0; i < 3; i++)
    {
        dstt_channels[i] = dstt_channels[i].mul(255.0 / scale - scr_channels[3], scale / 255.0);
        dstt_channels[i] += scr_channels[i].mul(scr_channels[3], scale / 255.0);
    }
    merge(dstt_channels, descImg);
    return true;
}

bool ImageHelper::ChangeTransparentBackgroundToSolidColor(
        const filesystem::path& filePath, const cv::Scalar& scalar)
{
    cv::Mat srcImg = cv::imread(filePath.generic_string().c_str(), cv::ImreadModes::IMREAD_UNCHANGED);
    cv::Mat backgroundImg(srcImg.rows, srcImg.cols, CV_32FC1, scalar);
    boost::filesystem::path temp = filePath.parent_path();
    string tempFileName = uuid_string();
    boost::filesystem::path tempFile = temp.append(tempFileName+filePath.extension().generic_string());
    cv::imwrite(tempFile.generic_string(), backgroundImg);

    backgroundImg = cv::imread(tempFile.generic_string(), cv::ImreadModes::IMREAD_UNCHANGED);
    cv::Mat img(backgroundImg, cvRect(0, 0, srcImg.cols, srcImg.rows));
    MergeMatWith4C(img, srcImg, 1.0);
    cv::imwrite(filePath.generic_string(), img);
    filesystem::remove(tempFile);
    return true;
}

bool ImageHelper::ChangeTransparentBackgroundToSolidColorV2(
        const filesystem::path& filePath, const cv::Scalar& scalar)
{
    cv::Mat srcImg = cv::imread(filePath.generic_string().c_str(), cv::ImreadModes::IMREAD_UNCHANGED);
    int row = srcImg.rows;
    int col = srcImg.cols;
    int c = srcImg.channels();
    if(c != 4)
        return true;
    for (int rowindex = 0; rowindex < row; ++rowindex)
    {
        for (int colindex = 0; colindex < col; ++colindex)
        {
            cv::Vec4b p;
            cv::Vec4b t = srcImg.at<cv::Vec4b >(rowindex, colindex);
            if (t[3] == 0)
            {
                p[0] = 255;
                p[1] = 255;
                p[2] = 255;
                p[3] = 255;
                srcImg.at<cv::Vec4b >(rowindex, colindex) = p;
                continue;
            }else if(t[3] < 255)
            {
                t[3] = 255;
                srcImg.at<cv::Vec4b >(rowindex, colindex) = t;
            }
        }
    }
    string msg;
    try
    {
        boost::filesystem::remove(filePath.generic_wstring());
    }
    catch (const std::exception& e)
    {
        msg = e.what();
    }

    cv::imwrite(filePath.generic_string(), srcImg);
    return true;
}