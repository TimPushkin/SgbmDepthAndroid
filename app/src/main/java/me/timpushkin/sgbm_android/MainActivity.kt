package me.timpushkin.sgbm_android

import android.net.Uri
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.Image
import androidx.compose.material.*
import androidx.compose.runtime.*
import androidx.compose.ui.graphics.ImageBitmap
import kotlinx.coroutines.launch
import me.timpushkin.sgbm_android.ui.elements.MenuButtons
import me.timpushkin.sgbm_android.ui.theme.MainTheme
import me.timpushkin.sgbm_android.utils.StorageUtils
import me.timpushkin.sgbm_android.utils.getDepthMap
import me.timpushkin.sgbm_android.utils.loadCalibrationParams
import me.timpushkin.sgbm_android.utils.depthMapToBitmap

class MainActivity : ComponentActivity() {
    private lateinit var mStorageUtils: StorageUtils

    companion object {
        init {
            System.loadLibrary("opencv_java4")
            System.loadLibrary("sgbm_android")
        }
    }

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

    private fun processImageUris(leftUri: Uri, rightUri: Uri): ImageBitmap = mStorageUtils.run {
        leftUri.useTempCopy { leftImage ->
            rightUri.useTempCopy { rightImage ->
                depthMapToBitmap(getDepthMap(leftImage.path, rightImage.path))
            }
        }
    }
}
