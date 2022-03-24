#ifndef SGBM_ANDROID_CORE_CONVERSIONS_H
#define SGBM_ANDROID_CORE_CONVERSIONS_H

#include <opencv2/core.hpp>

template<class T>
std::vector<T> matToVector(const cv::Mat &mat);

#endif
