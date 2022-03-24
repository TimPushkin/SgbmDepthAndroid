#include "conversions.h"

template<class T>
std::vector<T> matToVector(const cv::Mat &mat) {
    std::vector<T> result;
    cv::Mat cont = mat.isContinuous() ? mat : mat.clone(); // make sure the Mat is continuous
    cont.reshape(1, 1).copyTo(result); // copy from a 1D Mat
    return result;
}
