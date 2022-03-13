#include <jni.h>
#include "logging.h"
#include "depth_estimator.h"

#define TAG "n_sgbm_android_lib"

#define LEFT_MAP_X_NAME "leftMapX"
#define LEFT_MAP_Y_NAME "leftMapY"
#define RIGHT_MAP_X_NAME "rightMapX"
#define RIGHT_MAP_Y_NAME "rightMapY"
#define Q_NAME "Q"

static DepthEstimator depthEstimator;

// TODO: move to a separate file (linkage problems may arise)
template<class T>
std::vector<T> matToVector(const cv::Mat &mat) {
    std::vector<T> result;
    cv::Mat cont = mat.isContinuous() ? mat : mat.clone(); // make sure the Mat is continuous
    cont.reshape(1, 1).copyTo(result); // copy from a 1D Mat
    return result;
}

extern "C" {

JNIEXPORT void JNICALL Java_me_timpushkin_sgbm_1android_utils_NativeKt_loadCalibrationParams(
        JNIEnv *env, jclass, jstring filename) {
    LOGI(TAG, "Loading calibration parameters...");

    cv::Mat leftMapX, leftMapY, rightMapX, rightMapY, Q;
    std::string filenameStr = env->GetStringUTFChars(filename, nullptr);

    LOGD(TAG, "Opening %s for reading", filenameStr.c_str());

    cv::FileStorage file(filenameStr, cv::FileStorage::Mode::READ);

    if (!file.isOpened()) {
        LOGE(TAG, "Failed to open %s for reading", filenameStr.c_str());
        // TODO: raise an exception
        return;
    }

    file[LEFT_MAP_X_NAME] >> leftMapX;
    file[LEFT_MAP_Y_NAME] >> leftMapY;
    file[RIGHT_MAP_X_NAME] >> rightMapX;
    file[RIGHT_MAP_Y_NAME] >> rightMapY;
    file[Q_NAME] >> Q;

    depthEstimator.loadCalibrationParams(leftMapX, leftMapY, rightMapX, rightMapY, Q);
}

JNIEXPORT jfloatArray JNICALL Java_me_timpushkin_sgbm_1android_utils_NativeKt_getDepthMap(
        JNIEnv *env, jclass, jstring leftFilename, jstring rightFilename, jint width, jint height) {
    LOGI(TAG, "Getting depth map...");

    cv::Mat leftImage = cv::imread(env->GetStringUTFChars(leftFilename, nullptr)),
            rightImage = cv::imread(env->GetStringUTFChars(rightFilename, nullptr));

    LOGD(TAG, "Loaded images:\n"
              "- Left image has size %i x %i\n"
              "- Right image has size %i x %i\n",
         leftImage.cols, leftImage.rows,
         rightImage.cols, rightImage.rows);

    cv::Mat depthMap;
    depthEstimator.calcDepth(leftImage, rightImage, depthMap, cv::Size(width, height));

    std::vector<float> depthVec = matToVector<float>(depthMap);

    jfloatArray result = env->NewFloatArray(depthVec.size());
    if (result == nullptr) {
        LOGE(TAG, "Failed to obtain jfloatArray of size %u", depthVec.size());
        // TODO: raise an exception
        return nullptr;
    }
    env->SetFloatArrayRegion(result, 0, depthVec.size(), &depthVec[0]);

    return result;
}

}
