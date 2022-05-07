package me.timpushkin.sgbmandroidapp

import android.graphics.Bitmap
import android.graphics.BitmapFactory
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
import androidx.compose.ui.graphics.asImageBitmap
import kotlinx.coroutines.launch
import me.timpushkin.sgbmandroid.DepthEstimator
import me.timpushkin.sgbmandroidapp.ui.MenuButtons
import me.timpushkin.sgbmandroidapp.utils.StorageUtils
import me.timpushkin.sgbmandroidapp.utils.depthArrayToBitmap

private const val CACHED_PARAMS = "params.xml"
private const val CACHED_DEPTHMAP = "depth.png"
private const val WIDTH = 640
private const val HEIGHT = 360

class MainActivity : ComponentActivity() {
    private lateinit var mStorageUtils: StorageUtils
    private lateinit var mDepthEstimator: DepthEstimator

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        mStorageUtils = StorageUtils(this)
        mStorageUtils.getFromCache(CACHED_PARAMS)?.let { params ->
            mDepthEstimator = DepthEstimator(params.path)
        }

        val cachedDepthMap = mStorageUtils.getFromCache(CACHED_DEPTHMAP)?.let { file ->
            BitmapFactory.decodeFile(file.path)
        }

        setContent {
            MaterialTheme {
                val scope = rememberCoroutineScope()
                val scaffoldState = rememberScaffoldState()
                var depthMap by remember { mutableStateOf(cachedDepthMap) }
                var canEstimateDepth by remember {
                    mutableStateOf(this::mDepthEstimator.isInitialized)
                }

                Scaffold(
                    scaffoldState = scaffoldState,
                    bottomBar = {
                        fun displayError(message: String) =
                            scope.launch { scaffoldState.snackbarHostState.showSnackbar(message) }

                        MenuButtons(
                            showPickImagesButton = canEstimateDepth,
                            onParamsPick = { uri ->
                                getDepthEstimator(uri)?.let { depthEstimator ->
                                    mDepthEstimator = depthEstimator
                                    canEstimateDepth = true
                                } ?: displayError("Cannot open calibration parameters")
                            },
                            onImagesPick = { left, right ->
                                getDepthMap(left, right)?.let { bitmap -> depthMap = bitmap }
                                    ?: displayError("Cannot open images")
                            },
                            onError = ::displayError
                        )
                    },
                    backgroundColor = MaterialTheme.colors.background
                ) { contentPadding ->
                    depthMap?.let { bitmap ->
                        Image(
                            bitmap = bitmap.asImageBitmap(),
                            contentDescription = "Depth map",
                            modifier = Modifier.padding(contentPadding)
                        )
                    }
                }
            }
        }
    }

    private fun getDepthEstimator(uri: Uri): DepthEstimator? =
        with(mStorageUtils) { uri.copyToCache(CACHED_PARAMS) }?.let { params ->
            DepthEstimator(params.path)
        }

    private fun getDepthMap(leftUri: Uri, rightUri: Uri): Bitmap? =
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
                ).also { bitmap -> with(mStorageUtils) { bitmap.writeToCache(CACHED_DEPTHMAP) } }
            }
        }
}
