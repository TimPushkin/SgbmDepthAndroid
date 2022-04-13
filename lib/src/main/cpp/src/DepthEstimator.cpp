#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "../include/DepthEstimator.h"
#include "Conversions.h"
#include "Logging.h"

#define TAG "DepthEstimator"

DepthEstimator::DepthEstimator(const std::string &calibPath) {
    // TODO: call calibrate

    LOGI(TAG, "Loading calibration parameters from %s...", calibPath.c_str());

    cv::FileStorage fs(calibPath, cv::FileStorage::READ);

    if (!fs.isOpened()) {
        LOGE(TAG, "Failed to read calibration parameters from the received file");
        throw std::invalid_argument("Illegal calibration file format");
    }

    fs["leftMapX"] >> mLeftMap.first;
    fs["leftMapY"] >> mLeftMap.second;
    fs["rightMapX"] >> mRightMap.first;
    fs["rightMapY"] >> mRightMap.second;
    fs["Q"] >> mQ;

    LOGD(TAG, "Loaded calibration parameters:\n"
              "- LeftMapX has size %i x %i\n"
              "- LeftMapY has size %i x %i\n"
              "- RightMapX has size %i x %i\n"
              "- RightMapY has size %i x %i\n"
              "- Q has size %i x %i",
         mLeftMap.first.cols, mLeftMap.first.rows,
         mLeftMap.second.cols, mLeftMap.second.rows,
         mRightMap.first.cols, mRightMap.first.rows,
         mRightMap.second.cols, mRightMap.second.rows,
         mQ.cols, mQ.rows);
}

bool DepthEstimator::calibrate(const std::vector<char> &leftImage, const std::vector<char> &rightImage) {
    // TODO: fill calibration parameters
    return true;
}

void DepthEstimator::getDisparity(cv::InputArray leftImage, cv::InputArray rightImage, cv::OutputArray dst) const {
    LOGI(TAG, "Computing disparity...");

    cv::Mat leftImageGray, rightImageGray;

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

void DepthEstimator::getDepthFromDisparity(cv::InputArray disparity, cv::OutputArray dst) const {
    LOGI(TAG, "Getting depth from disparity...");

    cv::Mat image3d, depthMap;

    cv::reprojectImageTo3D(disparity, image3d, mQ);

    depthMap.create(image3d.size(), CV_32FC1);
    image3d.forEach<cv::Point3f>([&](cv::Point3f &point, const int pos[]) -> void {
        depthMap.at<float>(pos[0], pos[1]) = 0 <= point.z && point.z <= maxDepth ? point.z : 0;
    });

    dst.assign(depthMap);
}

std::vector<float> DepthEstimator::estimateDepth(const std::vector<char> &leftImageEncoded,
                                                 const std::vector<char> &rightImageEncoded,
                                                 int width, int height) const {
    cv::Mat leftImage, rightImage, depthMap;
    cv::Size imageSize(width, height);

    LOGI(TAG, "Preparing images for depth calculation...");

    leftImage = cv::imdecode(leftImageEncoded, cv::IMREAD_COLOR);
    rightImage = cv::imdecode(rightImageEncoded, cv::IMREAD_COLOR);

    LOGD(TAG, "Loaded images:\n"
              "- Left image has size %i x %i\n"
              "- Right image has size %i x %i\n",
         leftImage.cols, leftImage.rows,
         rightImage.cols, rightImage.rows);

    cv::remap(leftImage, leftImage, mLeftMap.first, mLeftMap.second, cv::INTER_LINEAR);
    cv::remap(rightImage, rightImage, mRightMap.first, mRightMap.second, cv::INTER_LINEAR);

    cv::resize(leftImage, leftImage, imageSize);
    cv::resize(rightImage, rightImage, imageSize);

    getDisparity(leftImage, rightImage, depthMap);
    getDepthFromDisparity(depthMap * disparityCorrectionFactor, depthMap);

    LOGI(TAG, "Depth calculation finished");

    return matToVector<float>(depthMap);
}

void DepthEstimator::setMinDisparity(int value) { minDisparity = value; }

void DepthEstimator::setMaxDisparity(int value) { maxDisparity = value; }

void DepthEstimator::setBlockSize(int value) { blockSize = value; }

void DepthEstimator::setDisparityCorrectionFactor(float value) { disparityCorrectionFactor = value; }

void DepthEstimator::setMaxDepth(float value) { maxDepth = value; }
