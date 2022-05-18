#ifndef SGBMANDROID_SRC_MAIN_CPP_INCLUDE_CONSTANTS_H_
#define SGBMANDROID_SRC_MAIN_CPP_INCLUDE_CONSTANTS_H_

namespace sgbmandroid {

namespace internal {

// StereoSGBM initialization
constexpr auto minDisparity = 0;
constexpr auto maxDisparity = 112;
constexpr auto blockSize = 3;
constexpr auto p1Multiplier = 8 * 3;
constexpr auto p2Multiplier = 32 * 3;
constexpr auto disp12MaxDiff = 1;
constexpr auto preFilterCap = 0;
constexpr auto uniquenessRatio = 5;
constexpr auto speckleWindowSize = 50;
constexpr auto speckleRange = 2;

// DepthEstimator construction
constexpr auto minDepth = 0;
constexpr auto maxDepth = 10;
constexpr auto imageScaleFactor = 1;

// DepthEstimator operations
constexpr auto disparityCorrectionFactor = 1.0F / 16;

}  // namespace internal

}  // namespace sgbmandroid

#endif  // SGBMANDROID_SRC_MAIN_CPP_INCLUDE_CONSTANTS_H_
