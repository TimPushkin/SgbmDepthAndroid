package me.timpushkin.sgbm_android_lib

object SgbmAndroidLib {
    init {
        System.loadLibrary("sgbm_android_lib")
    }

    external fun loadCalibrationParams(content: ByteArray)

    external fun getDepthMap(
        leftImage: ByteArray,
        rightImage: ByteArray,
        width: Int,
        height: Int
    ): FloatArray
}
