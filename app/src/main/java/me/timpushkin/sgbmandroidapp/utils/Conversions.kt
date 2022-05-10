package me.timpushkin.sgbmandroidapp.utils

import android.graphics.Bitmap
import android.util.Log
import java.nio.IntBuffer

private const val TAG = "Conversions"

private const val A_SH = 24 // Alpha channel shift
private const val R_SH = 16 // Red channel shift
private const val G_SH = 8 // Green channel shift
private const val B_SH = 0 // Blue channel shift

private const val MAX_COLOR = 0xff
private const val MAX_A = MAX_COLOR shl A_SH

fun depthArrayToBitmap(depthArray: Array<FloatArray>): Bitmap {
    val width = depthArray.firstOrNull()?.size ?: 0
    val height = depthArray.size

    Log.i(TAG, "Converting depth array of size $width x $height to a Bitmap")

    val flatDepthArray = depthArray.flatMap { it.toList() }
    val max = flatDepthArray.maxOfOrNull { it } ?: Float.MIN_VALUE
    val buffer = IntBuffer.allocate(flatDepthArray.size).apply {
        flatDepthArray.forEach { depth ->
            val normalized = (depth.coerceAtLeast(0f) / max * MAX_COLOR).toInt()
            put(MAX_A or (normalized shl R_SH) or (normalized shl G_SH) or (normalized shl B_SH))
        }
        rewind()
    }

    return Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888).apply {
        copyPixelsFromBuffer(buffer)
    }
}
