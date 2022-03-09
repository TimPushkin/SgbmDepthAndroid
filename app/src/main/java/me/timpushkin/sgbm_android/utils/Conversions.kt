package me.timpushkin.sgbm_android.utils

import android.graphics.Bitmap
import androidx.compose.ui.graphics.ImageBitmap
import androidx.compose.ui.graphics.asImageBitmap
import org.opencv.android.Utils
import org.opencv.core.Core
import org.opencv.core.CvType
import org.opencv.core.Mat

fun depthMapToBitmap(matAddr: Long): ImageBitmap {
    val mat = Mat()
    Core.normalize(Mat(matAddr), mat, 0.0, 255.0, Core.NORM_MINMAX, CvType.CV_8UC1)

    val bitmap = mat.run { Bitmap.createBitmap(cols(), rows(), Bitmap.Config.ARGB_8888) }
    Utils.matToBitmap(mat, bitmap)

    return bitmap.asImageBitmap()
}
