package me.timpushkin.sgbm_android

import android.net.Uri
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.Image
import androidx.compose.material.*
import androidx.compose.runtime.*
import androidx.compose.ui.graphics.ImageBitmap
import androidx.compose.ui.graphics.asImageBitmap
import kotlinx.coroutines.launch
import me.timpushkin.sgbm_android.ui.elements.MenuButtons
import me.timpushkin.sgbm_android.ui.theme.MainTheme
import me.timpushkin.sgbm_android.utils.StorageUtils
import me.timpushkin.sgbm_android.utils.depthArrayToBitmap
import me.timpushkin.sgbm_android_lib.SgbmAndroidLib.getDepthMap
import me.timpushkin.sgbm_android_lib.SgbmAndroidLib.loadCalibrationParams

private const val WIDTH = 640
private const val HEIGHT = 360

class MainActivity : ComponentActivity() {
    private lateinit var mStorageUtils: StorageUtils

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        mStorageUtils = StorageUtils(this)

        setContent {
            MainTheme {
                val scope = rememberCoroutineScope()
                val scaffoldState = rememberScaffoldState()
                var depthMap by remember { mutableStateOf(null as ImageBitmap?) }

                Scaffold(
                    scaffoldState = scaffoldState,
                    bottomBar = {
                        MenuButtons(
                            this::processParamsUri,
                            { left, right -> depthMap = processImageUris(left, right) },
                            { msg -> scope.launch { scaffoldState.snackbarHostState.showSnackbar(msg) } }
                        )
                    },
                    backgroundColor = MaterialTheme.colors.background
                ) {
                    depthMap?.let {
                        Image(
                            bitmap = it,
                            contentDescription = "Depth map"
                        )
                    }
                }
            }
        }
    }

    private fun processParamsUri(uri: Uri) {
        mStorageUtils.run { uri.useTempCopy(".xml") { loadCalibrationParams(it.path) } }
    }

    private fun processImageUris(leftUri: Uri, rightUri: Uri): ImageBitmap =
        contentResolver.openInputStream(leftUri)?.use { leftStream ->
            contentResolver.openInputStream(rightUri)?.use { rightStream ->
                depthArrayToBitmap(
                    getDepthMap(leftStream.readBytes(), rightStream.readBytes(), WIDTH, HEIGHT),
                    WIDTH,
                    HEIGHT
                ).asImageBitmap()
            }
        } ?: ImageBitmap(WIDTH, HEIGHT)
}
