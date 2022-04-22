package me.timpushkin.sgbmandroid_app

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
import me.timpushkin.sgbmandroid_app.ui.elements.MenuButtons
import me.timpushkin.sgbmandroid_app.ui.theme.MainTheme
import me.timpushkin.sgbmandroid_app.utils.depthArrayToBitmap
import me.timpushkin.sgbmandroid.SgbmAndroid.DepthEstimator

private const val WIDTH = 640
private const val HEIGHT = 360

class MainActivity : ComponentActivity() {
    private lateinit var depthEstimator: DepthEstimator

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContent {
            MainTheme {
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

    private fun processParamsUri(uri: Uri, onError: (String) -> Unit) {
        val inputStream = contentResolver.openInputStream(uri)
        if (inputStream == null) {
            onError("Cannot open calibration parameters")
            return
        }

        // val params = inputStream.use { it.readBytes() }
        depthEstimator = DepthEstimator("" /* TODO: pass calibration file path */)
    }

    private fun processImageUris(
        leftUri: Uri,
        rightUri: Uri,
        onError: (String) -> Unit
    ): ImageBitmap =
        contentResolver.openInputStream(leftUri)?.use { leftStream ->
            contentResolver.openInputStream(rightUri)?.use { rightStream ->
                depthArrayToBitmap(
                    depthEstimator.estimateDepth(
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
