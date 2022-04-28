package me.timpushkin.sgbmandroidapp

import android.net.Uri
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.padding
import androidx.compose.material.MaterialTheme
import androidx.compose.material.Scaffold
import androidx.compose.material.rememberScaffoldState
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.ImageBitmap
import androidx.compose.ui.graphics.asImageBitmap
import kotlinx.coroutines.launch
import me.timpushkin.sgbmandroid.DepthEstimator
import me.timpushkin.sgbmandroidapp.ui.elements.MenuButtons
import me.timpushkin.sgbmandroidapp.utils.StorageUtils
import me.timpushkin.sgbmandroidapp.utils.depthArrayToBitmap

private const val WIDTH = 640
private const val HEIGHT = 360

class MainActivity : ComponentActivity() {
    private lateinit var mStorageUtils: StorageUtils
    private lateinit var mDepthEstimator: DepthEstimator

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        mStorageUtils = StorageUtils(this)

        setContent {
            MaterialTheme {
                val scope = rememberCoroutineScope()
                val scaffoldState = rememberScaffoldState()
                var depthMap by remember { mutableStateOf(null as ImageBitmap?) }

                Scaffold(
                    scaffoldState = scaffoldState,
                    bottomBar = {
                        fun displayError(message: String) =
                            scope.launch { scaffoldState.snackbarHostState.showSnackbar(message) }

                        MenuButtons(
                            { uri -> processParamsUri(uri, ::displayError) },
                            { left, right ->
                                depthMap = processImageUris(left, right, ::displayError)
                            },
                            ::displayError
                        )
                    },
                    backgroundColor = MaterialTheme.colors.background
                ) { contentPadding ->
                    depthMap?.let { bitmap ->
                        Image(
                            bitmap = bitmap,
                            contentDescription = "Depth map",
                            modifier = Modifier.padding(contentPadding)
                        )
                    }
                }
            }
        }
    }

    private fun processParamsUri(uri: Uri, onError: (String) -> Unit) {
        with(mStorageUtils) { uri.useTempCopy(".xml") { DepthEstimator(it.path) } }?.let {
            mDepthEstimator = it
        } ?: onError("Cannot open calibration parameters")
    }

    private fun processImageUris(
        leftUri: Uri,
        rightUri: Uri,
        onError: (String) -> Unit
    ): ImageBitmap =
        contentResolver.openInputStream(leftUri)?.use { leftStream ->
            contentResolver.openInputStream(rightUri)?.use { rightStream ->
                depthArrayToBitmap(
                    mDepthEstimator.estimateDepth(
                        leftStream.readBytes(),
                        rightStream.readBytes(),
                        WIDTH,
                        HEIGHT
                    ),
                    WIDTH,
                    HEIGHT
                ).asImageBitmap()
            }
        } ?: run {
            onError("Images processing failed")
            ImageBitmap(WIDTH, HEIGHT)
        }
}
