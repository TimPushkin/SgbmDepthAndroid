package me.timpushkin.sgbm_android.utils

import android.content.Context
import android.net.Uri
import com.google.modernstorage.storage.AndroidFileSystem
import com.google.modernstorage.storage.toOkioPath
import okio.Path.Companion.toOkioPath
import java.io.File

private const val TMP_PREFIX = "tmp"

class StorageUtils(private val context: Context) {
    private val mFileSystem = AndroidFileSystem(context)

    fun getTempCopy(src: Uri, suffix: String? = null): File =
        File.createTempFile(TMP_PREFIX, suffix, context.cacheDir).also {
            mFileSystem.copy(src.toOkioPath(), it.toOkioPath())
        }

    inline fun <T> Uri.useTempCopy(suffix: String? = null, action: (File) -> T): T {
        val tmp = getTempCopy(this, suffix)
        return action(tmp).also { tmp.delete() }
    }
}
