#include "opencv2/core.hpp"
#include "CalibParamsReader.h"
#include "Logging.h"

#define TAG "CalibParamsReader"

CalibParams CalibParamsReader::readCalibParams(const char *content, const std::string &leftMapXName,
                                               const std::string &leftMapYName, const std::string &rightMapXName,
                                               const std::string &rightMapYName, const std::string &qName) {
    cv::Mat leftMapX, leftMapY, rightMapX, rightMapY, Q;

    LOGI(TAG, "Reading calibration parameters from file contents...");

    cv::FileStorage fs(content, cv::FileStorage::READ + cv::FileStorage::MEMORY);

    if (!fs.isOpened()) {
        LOGE(TAG, "Failed to read the received content");
        throw std::invalid_argument("Illegal content format: cv::FileStorage was unable to open it");
    }

    fs[leftMapXName] >> leftMapX;
    fs[leftMapYName] >> leftMapY;
    fs[rightMapXName] >> rightMapX;
    fs[rightMapYName] >> rightMapY;
    fs[qName] >> Q;

    if (leftMapX.size != rightMapX.size || rightMapX.size != leftMapY.size || leftMapY.size != rightMapY.size) {
        LOGE(TAG, "Transformation matrices have different sizes");
        throw std::invalid_argument("Illegal content format: transformation matrices have different sizes");
    }
    if (Q.rows != 4 && Q.cols != 4) {
        LOGE(TAG, "Q matrix has illegal size: expected 4 x 4 but was %i x %i", Q.rows, Q.cols);
        throw std::invalid_argument("Illegal content format: wrong Q size");
    }
    if (leftMapX.type() != CV_16SC2 || leftMapY.type() != CV_16UC1 || rightMapX.type() != CV_16SC2 ||
        rightMapY.type() != CV_16UC1 || Q.type() != CV_64FC1) {
        LOGE(TAG, "Read matrices have illegal types: expected %i %i %i %i %i, but was %i %i %i %i %i", CV_16SC2,
             CV_16UC1, CV_16SC2, CV_16UC1, CV_64FC1, leftMapX.type(), leftMapY.type(), rightMapX.type(),
             rightMapY.type(), Q.type());
        throw std::invalid_argument("Illegal content format: wrong matrix types");
    }

    size_t leftMapXSize = (leftMapX.dataend - leftMapX.datastart) * sizeof(uchar),
            leftMapYSize = (leftMapY.dataend - leftMapY.datastart) * sizeof(uchar),
            rightMapXSize = (rightMapX.dataend - rightMapX.datastart) * sizeof(uchar),
            rightMapYSize = (rightMapY.dataend - rightMapY.datastart) * sizeof(uchar),
            QSize = (Q.dataend - Q.datastart) * sizeof(uchar);

    CalibParams params{leftMapX.rows, leftMapY.cols,
                       {std::make_unique<unsigned char>(leftMapXSize), std::make_unique<unsigned char>(leftMapYSize)},
                       {std::make_unique<unsigned char>(rightMapXSize), std::make_unique<unsigned char>(rightMapYSize)},
                       std::make_unique<unsigned char>(QSize)};

    memcpy(params.leftMap.x.get(), leftMapX.data, leftMapXSize);
    memcpy(params.leftMap.y.get(), leftMapY.data, leftMapYSize);
    memcpy(params.rightMap.x.get(), rightMapX.data, rightMapXSize);
    memcpy(params.rightMap.x.get(), rightMapY.data, rightMapYSize);
    memcpy(params.Q.get(), Q.data, QSize);

    LOGD(TAG, "Read calibration parameters of size %i x %i", params.cols, params.rows);

    return params;
}
