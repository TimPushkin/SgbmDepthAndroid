package me.timpushkin.sgbmdepthapp.utils

import android.content.Context
import android.net.Uri
import android.util.Log
import com.google.modernstorage.storage.AndroidFileSystem
import com.google.modernstorage.storage.toOkioPath
import java.io.File
import okio.IOException
import okio.Path.Companion.toOkioPath

private const val TAG = "StorageUtils"

class StorageUtils(private val context: Context) {
    private val mFileSystem = AndroidFileSystem(context)

    fun copyToCache(uri: Uri, filename: String): File? =
        try {
            File(context.cacheDir, filename).also { copy ->
                mFileSystem.copy(uri.toOkioPath(), copy.toOkioPath())
            }
        } catch (e: IOException) {
            Log.e(TAG, "Failed to copy $uri to $filename", e)
            null
        }

    fun getFromCache(filename: String): File? =
        File(context.cacheDir, filename).takeIf { it.exists() }

    fun readPair(pair: Pair<Uri, Uri>): Pair<ByteArray, ByteArray>? =
        with(context.contentResolver) {
            openInputStream(pair.first)?.use { leftStream ->
                openInputStream(pair.second)?.use { rightStream ->
                    leftStream.readBytes() to rightStream.readBytes()
                }
            }
        }
}
