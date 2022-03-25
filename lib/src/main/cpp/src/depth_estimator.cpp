#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "../include/depth_estimator.h"
#include "conversions.h"
#include "logging.h"

#define TAG "DepthEstimator"

#define LOG_PARAMS \
LOGD(TAG, "Loaded calibration parameters:\n" \
"- LeftMapX has size %i x %i\n" \
"- LeftMapY has size %i x %i\n" \
"- RightMapX has size %i x %i\n" \
"- RightMapY has size %i x %i\n" \
"- Q has size %i x %i", \
mLeftMap.first.cols, mLeftMap.first.rows, \
mLeftMap.second.cols, mLeftMap.second.rows, \
mRightMap.first.cols, mRightMap.first.rows, \
mRightMap.second.cols, mRightMap.second.rows, \
mQ.cols, mQ.rows)

DepthEstimator::DepthEstimator(const cv::Mat &leftMapX, const cv::Mat &leftMapY, const cv::Mat &rightMapX,
                               const cv::Mat &rightMapY, const cv::Mat &Q) {
    loadCalibrationParams(leftMapX, leftMapY, rightMapX, rightMapY, Q);
}

DepthEstimator::DepthEstimator(const char *content, const std::string &leftMapXName,
                               const std::string &leftMapYName, const std::string &rightMapXName,
                               const std::string &rightMapYName, const std::string &qName) {
    loadCalibrationParams(content, leftMapXName, leftMapYName, rightMapXName, rightMapYName, qName);
}

void DepthEstimator::loadCalibrationParams(const cv::Mat &leftMapX, const cv::Mat &leftMapY, const cv::Mat &rightMapX,
                                           const cv::Mat &rightMapY, const cv::Mat &Q) {
    LOGI(TAG, "Loading calibration parameters...");

    mLeftMap.first = leftMapX;
    mLeftMap.second = leftMapY;
    mRightMap.first = rightMapX;
    mRightMap.second = rightMapY;
    mQ = Q;

    LOG_PARAMS;
}

void DepthEstimator::loadCalibrationParams(const char *content, const std::string &leftMapXName,
                                           const std::string &leftMapYName, const std::string &rightMapXName,
                                           const std::string &rightMapYName, const std::string &qName) {
    LOGI(TAG, "Loading calibration parameters from file contents...");

    cv::FileStorage fs(content, cv::FileStorage::READ + cv::FileStorage::MEMORY);

    if (!fs.isOpened()) {
        LOGE(TAG, "Failed to read the received content");
        // TODO: raise an exception
        return;
    }

    fs[leftMapXName] >> mLeftMap.first;
    fs[leftMapYName] >> mLeftMap.second;
    fs[rightMapXName] >> mRightMap.first;
    fs[rightMapYName] >> mRightMap.second;
    fs[qName] >> mQ;

    LOG_PARAMS;
}

void DepthEstimator::getDisparity(cv::InputArray leftImage, cv::InputArray rightImage,
                                  cv::OutputArray dst, int minDisparity, int maxDisparity,
                                  int blockSize) {
    LOGI(TAG, "Computing disparity...");

    cv::Mat leftImageGray, rightImageGray;

    // TODO: remove consts
    int numDisparities = maxDisparity - minDisparity,
            P1 = 8 * 3 * blockSize * blockSize,
            P2 = 32 * 3 * blockSize * blockSize,
            disp12MaxDiff = 1,
            uniquenessRatio = 5,
            speckleWindowSize = 50,
            speckleRange = 2;

    cv::cvtColor(leftImage, leftImageGray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(rightImage, rightImageGray, cv::COLOR_BGR2GRAY);

    cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(
            minDisparity,
            numDisparities,
            blockSize,
            P1,
            P2,
            disp12MaxDiff,
            0,
            uniquenessRatio,
            speckleWindowSize,
            speckleRange,
            cv::StereoSGBM::MODE_SGBM_3WAY
    );
    sgbm->compute(leftImageGray, rightImageGray, dst);
}

void DepthEstimator::getDepthFromDisparity(cv::InputArray disparity, cv::OutputArray dst, float maxDepth) {
    LOGI(TAG, "Getting depth from disparity...");

    cv::Mat image3d, depthMap;

    cv::reprojectImageTo3D(disparity, image3d, mQ);

    depthMap.create(image3d.size(), CV_32FC1);
    image3d.forEach<cv::Point3f>([&](cv::Point3f &point, const int pos[]) -> void {
        depthMap.at<float>(pos[0], pos[1]) = 0 <= point.z && point.z <= maxDepth ? point.z : 0;
    });

    dst.assign(depthMap);
}

void DepthEstimator::calcDepth(cv::InputArray leftImage, cv::InputArray rightImage, cv::OutputArray dst,
                               cv::Size imageSize) {
    LOGI(TAG, "Calculating depth...");

    cv::Mat leftImage_, rightImage_, depthMap;

    cv::remap(leftImage, leftImage_, mLeftMap.first, mLeftMap.second, cv::INTER_LINEAR);
    cv::remap(rightImage, rightImage_, mRightMap.first, mRightMap.second, cv::INTER_LINEAR);

    cv::resize(leftImage, leftImage_, imageSize);
    cv::resize(rightImage, rightImage_, imageSize);

    // TODO: remove consts
    getDisparity(leftImage_, rightImage_, depthMap, 0, 112, 3);
    getDepthFromDisparity(depthMap / 16.0, depthMap);

    LOGI(TAG, "Depth calculation finished");

    dst.assign(depthMap);
}

std::vector<float> DepthEstimator::calcDepth(const std::vector<char> &leftImageEncoded,
                                             const std::vector<char> &rightImageEncoded, int width, int height) {
    LOGI(TAG, "Converting `char *` images into OpenCV classes...");

    cv::Mat leftImage = cv::imdecode(leftImageEncoded, cv::IMREAD_COLOR),
            rightImage = cv::imdecode(rightImageEncoded, cv::IMREAD_COLOR),
            depthMap;

    LOGD(TAG, "Loaded images:\n"
              "- Left image has size %i x %i\n"
              "- Right image has size %i x %i\n",
         leftImage.cols, leftImage.rows,
         rightImage.cols, rightImage.rows);

    calcDepth(leftImage, rightImage, depthMap, cv::Size(width, height));

    return matToVector<float>(depthMap);
}
