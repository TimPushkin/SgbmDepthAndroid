package me.timpushkin.sgbm_android_lib

object SgbmAndroidLib {
    init {
        System.loadLibrary("sgbm_android_lib")
    }

    external fun loadCalibrationParams(filename: String)

    external fun getDepthMap(
        leftFilename: String,
        rightFilename: String,
        width: Int,
        height: Int
    ): FloatArray
}
