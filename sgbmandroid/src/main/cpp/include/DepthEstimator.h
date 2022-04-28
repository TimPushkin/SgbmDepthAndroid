#ifndef SGBMANDROID_SRC_MAIN_CPP_INCLUDE_DEPTHESTIMATOR_H_
#define SGBMANDROID_SRC_MAIN_CPP_INCLUDE_DEPTHESTIMATOR_H_

#include <string>
#include <vector>
#include <utility>
#include <scapix/bridge/object.h>
#include "opencv2/core.hpp"

namespace sgbmandroid {

class DepthEstimator : public scapix::bridge::object<DepthEstimator> {
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
    static constexpr char kTag[] = "DepthEstimator";

    std::pair<cv::Mat, cv::Mat> mLeftMap, mRightMap;
    cv::Mat mQ;

    int minDisparity = 0, maxDisparity = 112, blockSize = 3;
    float disparityCorrectionFactor = (1.0f / 16), maxDepth = 10;

    void getDisparity(cv::InputArray leftImage, cv::InputArray rightImage, cv::OutputArray dst) const;

    void getDepthFromDisparity(cv::InputArray disparity, cv::OutputArray dst) const;
};

}  // namespace sgbmandroid

#endif  // SGBMANDROID_SRC_MAIN_CPP_INCLUDE_DEPTHESTIMATOR_H_
