#ifndef LIB_SRC_MAIN_CPP_SRC_CONVERSIONS_H_
#define LIB_SRC_MAIN_CPP_SRC_CONVERSIONS_H_

#include <vector>
#include "opencv2/core.hpp"

template<class T>
std::vector<T> matToVector(const cv::Mat &mat) {
    std::vector<T> result;
    cv::Mat cont = mat.isContinuous() ? mat : mat.clone();  // make sure the Mat is continuous
    cont.reshape(1, 1).copyTo(result);  // copy from a 1D Mat
    return result;
}

#endif  // LIB_SRC_MAIN_CPP_SRC_CONVERSIONS_H_
