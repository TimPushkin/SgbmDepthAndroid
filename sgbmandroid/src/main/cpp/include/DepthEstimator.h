#ifndef SGBMANDROID_SRC_MAIN_CPP_INCLUDE_DEPTHESTIMATOR_H_
#define SGBMANDROID_SRC_MAIN_CPP_INCLUDE_DEPTHESTIMATOR_H_

#include <string>
#include <vector>
#include <utility>
#include "opencv2/core.hpp"

#ifdef SCAPIX_BRIDGE

#include <scapix/bridge/object.h>

template<typename T>
using base_object = scapix::bridge::object<T>;

#else

template<typename T>
class base_object {
};

#endif  // SCAPIX_BRIDGE

namespace sgbmandroid {

class DepthEstimator : public base_object<DepthEstimator> {
 public:
    explicit DepthEstimator(const std::string &calibPath);

    bool calibrate(const std::vector<char> &leftImage, const std::vector<char> &rightImage);

    std::vector<float> estimateDepth(const std::vector<char> &leftImageEncoded,
                                     const std::vector<char> &rightImageEncoded) const;

    void setMaxDisparity(int value);

    void setBlockSize(int value);

    void setSpeckleWindowSize(int value);

    void setMinDepth(float value);

    void setMaxDepth(float value);

    void setImageScaleFactor(float value);

 private:
    std::pair<cv::Mat, cv::Mat> mLeftMap;
    std::pair<cv::Mat, cv::Mat> mRightMap;
    cv::Mat mQ;

    int maxDisparity = 112;
    int blockSize = 3;
    int speckleWindowSize = 50;
    float minDepth = 0;
    float maxDepth = 10;
    float imageScaleFactor = 1;

    void getDisparity(cv::InputArray leftImage, cv::InputArray rightImage, cv::OutputArray dst) const;

    void getDepthFromDisparity(cv::InputArray disparity, cv::OutputArray dst) const;
};

}  // namespace sgbmandroid

#endif  // SGBMANDROID_SRC_MAIN_CPP_INCLUDE_DEPTHESTIMATOR_H_
