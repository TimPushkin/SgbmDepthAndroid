#ifndef SGBMANDROID_SRC_MAIN_CPP_SRC_CONVERSIONS_H_
#define SGBMANDROID_SRC_MAIN_CPP_SRC_CONVERSIONS_H_

#include <vector>
#include "opencv2/core.hpp"
#include "Logging.h"

template<class T>
std::vector<T> matToVector(const cv::Mat &mat) {
    std::vector<T> result;
    cv::Mat cont = mat.isContinuous() ? mat : mat.clone();  // make sure the Mat is continuous
    cont.reshape(1, 1).copyTo(result);  // copy from a 1D Mat
    return result;
}

template<class T>
std::vector<std::vector<T>> twoDimMatToTwoDimVector(const cv::Mat &mat) {
    std::vector<std::vector<T>> result;
    cv::Mat cont = mat.isContinuous() ? mat : mat.clone();  // make sure the Mat is continuous

    result.reserve(cont.rows);

    auto rowLen = cont.cols * cont.channels();
    for (auto i = 0; i < cont.rows; i++) {
        const auto *rowPtr = cont.ptr<float>(i);
        result.push_back(std::vector<float>(rowPtr, rowPtr + rowLen));
    }

    return result;
}

#endif  // SGBMANDROID_SRC_MAIN_CPP_SRC_CONVERSIONS_H_
