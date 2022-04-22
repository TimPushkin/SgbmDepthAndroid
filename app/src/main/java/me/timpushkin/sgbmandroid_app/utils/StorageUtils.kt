package me.timpushkin.sgbmandroid_app.utils

import android.content.Context
import android.net.Uri
import android.util.Log
import com.google.modernstorage.storage.AndroidFileSystem
import com.google.modernstorage.storage.toOkioPath
import okio.IOException
import okio.Path.Companion.toOkioPath
import java.io.File

private const val TAG = "StorageUtils"

private const val TMP_PREFIX = "tmp"

class StorageUtils(private val context: Context) {
    private val mFileSystem = AndroidFileSystem(context)

    private fun getTempCopy(src: Uri, suffix: String? = null): File? =
        try {
            File.createTempFile(TMP_PREFIX, suffix, context.cacheDir).also { copy ->
                mFileSystem.copy(src.toOkioPath(), copy.toOkioPath())
            }
        } catch (e: IOException) {
            Log.e(TAG, "Failed to copy $src", e)
            null
        }

    fun <T> Uri.useTempCopy(suffix: String? = null, action: (File) -> T): T? =
        getTempCopy(this, suffix)?.let { tmp -> action(tmp).also { tmp.delete() } }
}
