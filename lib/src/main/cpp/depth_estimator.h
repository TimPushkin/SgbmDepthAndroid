#ifndef SGBM_ANDROID_CORE_DEPTH_ESTIMATOR_H
#define SGBM_ANDROID_CORE_DEPTH_ESTIMATOR_H

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

class DepthEstimator {
    std::pair<cv::Mat, cv::Mat> mLeftMap, mRightMap;
    cv::Mat mQ;

    static void getDisparity(cv::InputArray leftImage, cv::InputArray rightImage,
                             cv::OutputArray dst, int minDisparity, int maxDisparity,
                             int blockSize = 1);

    void getDepthFromDisparity(cv::InputArray disparity, cv::OutputArray dst, float maxDepth = 10);

public:
    DepthEstimator(const cv::Mat &leftMapX, const cv::Mat &leftMapY, const cv::Mat &rightMapX,
                   const cv::Mat &rightMapY, const cv::Mat &Q);

    void loadCalibrationParams(const cv::Mat &leftMapX, const cv::Mat &leftMapY, const cv::Mat &rightMapX,
                               const cv::Mat &rightMapY, const cv::Mat &Q);

    void calcDepth(cv::InputArray leftImage, cv::InputArray rightImage, cv::OutputArray dst,
                   cv::Size imageSize = cv::Size(640, 360));
};

#endif
