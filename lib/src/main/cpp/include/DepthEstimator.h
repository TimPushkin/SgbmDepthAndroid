#ifndef SGBM_ANDROID_CORE_DEPTHESTIMATOR_H
#define SGBM_ANDROID_CORE_DEPTHESTIMATOR_H

#include "opencv2/core.hpp"

class DepthEstimator {
public:
    std::string leftMapXName = "leftMapX", leftMapYName = "leftMapY", rightMapXName = "rightMapX",
            rightMapYName = "rightMapY", qName = "Q";

    explicit DepthEstimator(const char *content);

    void loadCalibrationParams(const char *content);

    std::vector<float> calcDepth(const std::vector<char> &leftImage, const std::vector<char> &rightImage,
                                 int width = 640, int height = 360);

private:
    std::pair<cv::Mat, cv::Mat> mLeftMap, mRightMap;
    cv::Mat mQ;

    static void getDisparity(cv::InputArray leftImage, cv::InputArray rightImage, cv::OutputArray dst, int minDisparity,
                             int maxDisparity, int blockSize = 1);

    void getDepthFromDisparity(cv::InputArray disparity, cv::OutputArray dst, float maxDepth = 10);
};

#endif
