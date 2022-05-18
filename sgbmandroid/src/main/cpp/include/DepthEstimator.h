#ifndef SGBMANDROID_SRC_MAIN_CPP_INCLUDE_DEPTHESTIMATOR_H_
#define SGBMANDROID_SRC_MAIN_CPP_INCLUDE_DEPTHESTIMATOR_H_

#include <string>
#include <vector>
#include <utility>
#include "opencv2/core.hpp"
#include "opencv2/calib3d.hpp"
#include "../src/Constants.h"

#ifdef SCAPIX_BRIDGE

#include <scapix/bridge/object.h>

namespace sgbmandroid {

namespace internal {

template<typename T>
using base_object = scapix::bridge::object<T>;

}  // namespace internal

}  // namespace sgbmandroid

#else

namespace sgbmandroid {

namespace internal {

template<typename T>
class base_object {
};

}  // namespace internal

}  // namespace sgbmandroid

#endif  // SCAPIX_BRIDGE

namespace sgbmandroid {

class DepthEstimator : public internal::base_object<DepthEstimator> {
 public:
    explicit DepthEstimator(const std::string &calibPath);

    bool calibrate(const std::vector<char> &leftImage, const std::vector<char> &rightImage);

    std::vector<std::vector<float>> estimateDepth(const std::vector<char> &leftImageEncoded,
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

    cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(
            internal::minDisparity,
            internal::maxDisparity - internal::minDisparity,
            internal::blockSize,
            internal::p1Multiplier * internal::blockSize * internal::blockSize,
            internal::p2Multiplier * internal::blockSize * internal::blockSize,
            internal::disp12MaxDiff,
            internal::preFilterCap,
            internal::uniquenessRatio,
            internal::speckleWindowSize,
            internal::speckleRange,
            cv::StereoSGBM::MODE_SGBM_3WAY);

    float minDepth = internal::minDepth;
    float maxDepth = internal::maxDepth;
    float imageScaleFactor = internal::imageScaleFactor;

    void getDisparity(cv::InputArray leftImage, cv::InputArray rightImage, cv::OutputArray dst) const;

    void getDepthFromDisparity(cv::InputArray disparity, cv::OutputArray dst) const;
};

}  // namespace sgbmandroid

#endif  // SGBMANDROID_SRC_MAIN_CPP_INCLUDE_DEPTHESTIMATOR_H_
