#ifndef SGBM_ANDROID_CORE_DEPTH_ESTIMATOR_H
#define SGBM_ANDROID_CORE_DEPTH_ESTIMATOR_H

#include "opencv2/core.hpp"

#define LEFT_MAP_X_NAME "leftMapX"
#define LEFT_MAP_Y_NAME "leftMapY"
#define RIGHT_MAP_X_NAME "rightMapX"
#define RIGHT_MAP_Y_NAME "rightMapY"
#define Q_NAME "Q"

#define IMG_WIDTH 640
#define IMG_HEIGHT 360

class DepthEstimator {
    std::pair<cv::Mat, cv::Mat> mLeftMap, mRightMap;
    cv::Mat mQ;

    static void getDisparity(cv::InputArray leftImage, cv::InputArray rightImage, cv::OutputArray dst, int minDisparity,
                             int maxDisparity, int blockSize = 1);

    void getDepthFromDisparity(cv::InputArray disparity, cv::OutputArray dst, float maxDepth = 10);

public:
    DepthEstimator(const cv::Mat &leftMapX, const cv::Mat &leftMapY, const cv::Mat &rightMapX, const cv::Mat &rightMapY,
                   const cv::Mat &Q);

    explicit DepthEstimator(const char *content, const std::string &leftMapXName = LEFT_MAP_X_NAME,
                            const std::string &leftMapYName = LEFT_MAP_Y_NAME,
                            const std::string &rightMapXName = RIGHT_MAP_X_NAME,
                            const std::string &rightMapYName = RIGHT_MAP_Y_NAME, const std::string &qName = Q_NAME);

    void loadCalibrationParams(const cv::Mat &leftMapX, const cv::Mat &leftMapY, const cv::Mat &rightMapX,
                               const cv::Mat &rightMapY, const cv::Mat &Q);

    void loadCalibrationParams(const char *content, const std::string &leftMapXName = LEFT_MAP_X_NAME,
                               const std::string &leftMapYName = LEFT_MAP_Y_NAME,
                               const std::string &rightMapXName = RIGHT_MAP_X_NAME,
                               const std::string &rightMapYName = RIGHT_MAP_Y_NAME, const std::string &qName = Q_NAME);

    void calcDepth(cv::InputArray leftImage, cv::InputArray rightImage, cv::OutputArray dst,
                   cv::Size imageSize = cv::Size(IMG_WIDTH, IMG_HEIGHT));

    std::vector<float> calcDepth(const std::vector<char> &leftImageEncoded, const std::vector<char> &rightImageEncoded,
                                 int width = IMG_WIDTH, int height = IMG_HEIGHT);
};

#endif
