package me.timpushkin.sgbmandroidapp.utils

import android.content.Context
import android.graphics.Bitmap
import android.net.Uri
import android.util.Log
import com.google.modernstorage.storage.AndroidFileSystem
import com.google.modernstorage.storage.toOkioPath
import java.io.File
import java.io.FileNotFoundException
import okio.IOException
import okio.Path.Companion.toOkioPath

private const val TAG = "StorageUtils"

class StorageUtils(private val context: Context) {
    private val mFileSystem = AndroidFileSystem(context)

    fun Uri.copyToCache(filename: String): File? =
        try {
            File(context.cacheDir, filename).also { copy ->
                mFileSystem.copy(toOkioPath(), copy.toOkioPath())
            }
        } catch (e: IOException) {
            Log.e(TAG, "Failed to copy $this to $filename", e)
            null
        }

    fun Bitmap.writeToCache(filename: String): File? {
        val file = File(context.cacheDir, filename)

        val success = try {
            file.outputStream().use { out -> compress(Bitmap.CompressFormat.PNG, 100, out) }
        } catch (e: FileNotFoundException) {
            Log.e(TAG, "Failed to write a bitmap to $filename", e)
            false
        }

        return if (success) file else null.also { file.delete() }
    }

    fun getFromCache(filename: String): File? =
        File(context.cacheDir, filename).takeIf { it.exists() }
}
