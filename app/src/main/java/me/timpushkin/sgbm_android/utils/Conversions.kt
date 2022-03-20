package me.timpushkin.sgbm_android.utils

import android.graphics.Bitmap
import android.util.Log
import java.nio.IntBuffer

private const val TAG = "Conversions"

private const val MAX_COLOR = 0xff

fun depthArrayToBitmap(depthArray: FloatArray, width: Int, height: Int): Bitmap {
    Log.i(
        TAG,
        "Converting depth array of size ${depthArray.size} to a Bitmap of size $width x $height = ${width * height}"
    )

    val max = depthArray.maxOfOrNull { it }?.coerceAtLeast(Float.MIN_VALUE) ?: Float.MIN_VALUE
    val buffer = IntBuffer.allocate(depthArray.size).apply {
        val alpha = MAX_COLOR shl 24
        depthArray.forEach { depth ->
            val normalized = (depth.coerceAtLeast(0f) / max * MAX_COLOR).toInt()
            put(alpha or (normalized shl 16) or (normalized shl 8) or normalized)
        }
        rewind()
    }

    return Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888).apply {
        copyPixelsFromBuffer(buffer)
    }
}
