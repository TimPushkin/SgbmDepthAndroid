#include "depth_estimator.h"
#include "logging.h"

#define TAG "n_DepthEstimator"

void DepthEstimator::loadCalibrationParams(cv::Mat leftMapX, cv::Mat leftMapY, cv::Mat rightMapX,
                                           cv::Mat rightMapY, cv::Mat Q) {
    LOGI(TAG, "Loading calibration parameters...");

    mLeftMap.first = std::move(leftMapX);
    mLeftMap.second = std::move(leftMapY);
    mRightMap.first = std::move(rightMapX);
    mRightMap.second = std::move(rightMapY);
    mQ = std::move(Q);

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

void DepthEstimator::getDepthFromDisparity(cv::InputArray disparity, cv::OutputArray dst,
                                           float maxDepth) {
    LOGI(TAG, "Getting depth from disparity...");

    cv::Mat image3d, depthMap;

    // TODO: check that Q has been loaded
    cv::reprojectImageTo3D(disparity, image3d, mQ);

    depthMap.create(image3d.size(), CV_32FC1);
    image3d.forEach<cv::Point3f>([&](cv::Point3f &point, const int pos[]) -> void {
        depthMap.at<float>(pos[0], pos[1]) = 0 <= point.z && point.z <= maxDepth ? point.z : 0;
    });

    dst.assign(depthMap);
}

void DepthEstimator::calcDepth(cv::InputArray leftImage, cv::InputArray rightImage,
                               cv::OutputArray dst, cv::Size imageSize) {
    LOGI(TAG, "Calculating depth...");

    cv::Mat leftImage_, rightImage_, depthMap;

    // TODO: check that the maps have been loaded
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
