#ifndef SGBM_ANDROID_DEPTH_ESTIMATOR_H
#define SGBM_ANDROID_DEPTH_ESTIMATOR_H

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

class DepthEstimator {
    std::pair<cv::Mat, cv::Mat> mLeftMap, mRightMap;
    cv::Mat mQ;

public:
    void loadCalibrationParams(cv::Mat leftMapX, cv::Mat leftMapY, cv::Mat rightMapX,
                               cv::Mat rightMapY, cv::Mat Q);

    void calcDepth(cv::InputArray leftImage, cv::InputArray rightImage, cv::OutputArray dst,
                   cv::Size imageSize = cv::Size(640, 360));

private:
    static void getDisparity(cv::InputArray leftImage, cv::InputArray rightImage,
                             cv::OutputArray dst, int minDisparity, int maxDisparity,
                             int blockSize = 1);

    void getDepthFromDisparity(cv::InputArray disparity, cv::OutputArray dst, float maxDepth = 10);
};

#endif
