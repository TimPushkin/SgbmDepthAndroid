#ifndef SGBM_ANDROID_CORE_DEPTHESTIMATOR_H
#define SGBM_ANDROID_CORE_DEPTHESTIMATOR_H

#include "opencv2/core.hpp"

class DepthEstimator {
public:
    explicit DepthEstimator(const std::string &calibPath);

    bool calibrate(const std::vector<char> &leftImage, const std::vector<char> &rightImage);

    std::vector<float> estimateDepth(const std::vector<char> &leftImageEncoded,
                                     const std::vector<char> &rightImageEncoded,
                                     int width = 640, int height = 360) const;

    void setMinDisparity(int value);

    void setMaxDisparity(int value);

    void setBlockSize(int value);

    void setDisparityCorrectionFactor(float value);

    void setMaxDepth(float value);

private:
    std::pair<cv::Mat, cv::Mat> mLeftMap, mRightMap;
    cv::Mat mQ;

    int minDisparity = 0, maxDisparity = 112, blockSize = 3;
    float disparityCorrectionFactor = (1.0f / 16), maxDepth = 10;

    void getDisparity(cv::InputArray leftImage, cv::InputArray rightImage, cv::OutputArray dst) const;

    void getDepthFromDisparity(cv::InputArray disparity, cv::OutputArray dst) const;
};

#endif
