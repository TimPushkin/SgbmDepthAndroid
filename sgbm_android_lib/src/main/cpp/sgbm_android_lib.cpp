#include <jni.h>
#include <opencv2/imgcodecs.hpp>
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

char *jbyteArrayToChars(JNIEnv *env, jbyteArray &jarray) {
    size_t len = env->GetArrayLength(jarray);
    char *str = new char[len];
    env->GetByteArrayRegion(jarray, 0, len, reinterpret_cast<jbyte *>(str));
    return str;
}

template<class T>
std::vector<T> jbyteArrayToVector(JNIEnv *env, jbyteArray &jarray) {
    size_t len = env->GetArrayLength(jarray);
    std::vector<T> vec(len);
    env->GetByteArrayRegion(jarray, 0, len, reinterpret_cast<jbyte *>(vec.data()));
    return vec;
}

template<class T>
std::vector<T> matToVector(const cv::Mat &mat) {
    std::vector<T> result;
    cv::Mat cont = mat.isContinuous() ? mat : mat.clone(); // make sure the Mat is continuous
    cont.reshape(1, 1).copyTo(result); // copy from a 1D Mat
    return result;
}

extern "C" {

JNIEXPORT void JNICALL Java_me_timpushkin_sgbm_1android_1lib_SgbmAndroidLib_loadCalibrationParams(
        JNIEnv *env, jobject, jbyteArray jcontent) {
    LOGI(TAG, "Loading calibration parameters...");

    cv::Mat leftMapX, leftMapY, rightMapX, rightMapY, Q;
    char *content = jbyteArrayToChars(env, jcontent);

    cv::FileStorage fs(content, cv::FileStorage::READ + cv::FileStorage::MEMORY);

    if (!fs.isOpened()) {
        LOGE(TAG, "Failed to read the received content");
        // TODO: raise an exception
        return;
    }

    fs[LEFT_MAP_X_NAME] >> leftMapX;
    fs[LEFT_MAP_Y_NAME] >> leftMapY;
    fs[RIGHT_MAP_X_NAME] >> rightMapX;
    fs[RIGHT_MAP_Y_NAME] >> rightMapY;
    fs[Q_NAME] >> Q;

    depthEstimator.loadCalibrationParams(leftMapX, leftMapY, rightMapX, rightMapY, Q);
}

JNIEXPORT jfloatArray JNICALL Java_me_timpushkin_sgbm_1android_1lib_SgbmAndroidLib_getDepthMap(
        JNIEnv *env, jobject, jbyteArray jleftImage, jbyteArray jrightImage, jint width,
        jint height) {
    LOGI(TAG, "Getting depth map...");

    cv::Mat leftImage = cv::imdecode(jbyteArrayToVector<char>(env, jleftImage), cv::IMREAD_COLOR),
            rightImage = cv::imdecode(jbyteArrayToVector<char>(env, jrightImage), cv::IMREAD_COLOR);

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
