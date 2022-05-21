#ifndef SGBMDEPTH_SRC_MAIN_CPP_INCLUDE_DEPTHESTIMATOR_H_
#define SGBMDEPTH_SRC_MAIN_CPP_INCLUDE_DEPTHESTIMATOR_H_

#include <string>
#include <vector>
#include <utility>
#include "opencv2/core.hpp"
#include "opencv2/calib3d.hpp"
#include "../src/Constants.h"

#ifdef SCAPIX_BRIDGE

#include <scapix/bridge/object.h>

namespace sgbmdepth {

namespace internal {

template<typename T>
using base_object = scapix::bridge::object<T>;

}  // namespace internal

}  // namespace sgbmdepth

#else

namespace sgbmdepth {

namespace internal {

template<typename T>
class base_object {
};

}  // namespace internal

}  // namespace sgbmdepth

#endif  // SCAPIX_BRIDGE

namespace sgbmdepth {

// Computes a depth map from a pair of images.
class DepthEstimator : public internal::base_object<DepthEstimator> {
 public:
    // Creates a DepthEstimator instance that will use the calibration parameters from the contents of the given file.
    //
    // The structure of which must be compatible with cv::FileStorage from OpenCV.
    //
    // The calibration parameters must contain the following:
    // - Nodes with names "leftMapX", "leftMapY", "rightMapX", "rightMapY" containing matrices needed to rectify left
    //   and right images of the image pairs with cv::remap from OpenCV.
    // - A node named "Q" containing a perspective transformation matrix needed to obtain depth with
    //   cv::reprojectImageTo3D from OpenCV.
    explicit DepthEstimator(const std::string &calibPath);

    // Computes a depth map from the given image pair and returns it.
    //
    // The given images must be of the same size, which must be equal to the size of the rectification matrices provided
    // in the calibration parameters. They must be encoded in a format supported by cv::imdecode from OpenCV.
    //
    // The returned depth map is a two-dimensional vector of floats of the size equal to the size of the given images
    // multiplied by the imageScaleFactor. The depth values are in the same units as the values of the perspective
    // transformation matrix "Q" provided in the calibration parameters.
    std::vector<std::vector<float>> estimateDepth(const std::vector<char> &leftImageEncoded,
                                                  const std::vector<char> &rightImageEncoded) const;

    // Maximum possible disparity value. It is expected to be greater than 0 and divisible by 16.
    //
    // Default is 112.
    //
    // Read more about this parameter in the documentation of cv::StereoSGBM from OpenCV.
    void setMaxDisparity(int value);

    // Matched block size. It is expected to be an odd number greater or equal to 1.
    //
    // Default is 3.
    //
    // Read more about this parameter in the documentation of cv::StereoSGBM from OpenCV.
    void setBlockSize(int value);

    // Maximum size of smooth disparity regions to apply speckle filtering to.
    //
    // Default is 50.
    //
    // Read more about this parameter in the documentation of cv::StereoSGBM from OpenCV.
    void setSpeckleWindowSize(int value);

    // Minimum depth value in the resulting depth map. All values lower than this will be set to 0.
    //
    // Default is 0.
    void setMinDepth(float value);

    // Maximum depth value in the resulting depth map. All values greater than this will be set to 0.
    //
    // Default is 10.
    void setMaxDepth(float value);

    // The value by which the images will be scaled before computing the depth map. Lower values lead to better
    // performance and worse accuracy.
    //
    // Default is 1 (i.e. no scaling is applied).
    //
    // This parameter influences the last column of the perspective transformation matrix "Q" provided in the
    // calibration parameters and the maxDisparity parameter, which are also scaled accordingly if needed.
    void setImageScaleFactor(float value);

 private:
    std::pair<cv::Mat, cv::Mat> mLeftMap;
    std::pair<cv::Mat, cv::Mat> mRightMap;
    cv::Mat mQ;
    cv::Mat mScaleDependentQCol;
    cv::Mat mUnscaledScaleDependentQCol;

    float mUnscaledMaxDisparity = internal::maxDisparity;
    float mMinDepth = internal::minDepth;
    float mMaxDepth = internal::maxDepth;
    float mImageScaleFactor = internal::imageScaleFactor;

    cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(
            internal::minDisparity,
            static_cast<int> (mUnscaledMaxDisparity * mImageScaleFactor) - internal::minDisparity,
            internal::blockSize,
            internal::p1Multiplier * internal::blockSize * internal::blockSize,
            internal::p2Multiplier * internal::blockSize * internal::blockSize,
            internal::disp12MaxDiff,
            internal::preFilterCap,
            internal::uniquenessRatio,
            internal::speckleWindowSize,
            internal::speckleRange,
            cv::StereoSGBM::MODE_SGBM_3WAY);

    void getDisparity(cv::InputArray leftImage, cv::InputArray rightImage, cv::OutputArray dst) const;

    void getDepthFromDisparity(cv::InputArray disparity, cv::OutputArray dst) const;
};

}  // namespace sgbmdepth

#endif  // SGBMDEPTH_SRC_MAIN_CPP_INCLUDE_DEPTHESTIMATOR_H_
