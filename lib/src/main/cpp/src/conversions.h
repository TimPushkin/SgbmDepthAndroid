#ifndef SGBM_ANDROID_CORE_CONVERSIONS_H
#define SGBM_ANDROID_CORE_CONVERSIONS_H

#include "opencv2/core.hpp"

template<class T>
std::vector<T> matToVector(const cv::Mat &mat) {
    std::vector<T> result;
    cv::Mat cont = mat.isContinuous() ? mat : mat.clone(); // make sure the Mat is continuous
    cont.reshape(1, 1).copyTo(result); // copy from a 1D Mat
    return result;
}

#endif
