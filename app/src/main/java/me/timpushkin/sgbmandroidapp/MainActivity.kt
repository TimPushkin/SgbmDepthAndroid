package me.timpushkin.sgbmandroidapp

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.net.Uri
import android.os.Bundle
import android.os.SystemClock
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material.MaterialTheme
import androidx.compose.material.Scaffold
import androidx.compose.material.Text
import androidx.compose.material.rememberScaffoldState
import androidx.compose.runtime.*
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.asImageBitmap
import kotlinx.coroutines.launch
import me.timpushkin.sgbmandroid.DepthEstimator
import me.timpushkin.sgbmandroidapp.ui.MenuButtons
import me.timpushkin.sgbmandroidapp.ui.ScaleFactorField
import me.timpushkin.sgbmandroidapp.utils.StorageUtils
import me.timpushkin.sgbmandroidapp.utils.depthArrayToBitmap

private const val CACHED_PARAMS = "params.xml"
private const val CACHED_LEFT_IMAGE = "left.png"
private const val CACHED_RIGHT_IMAGE = "right.png"
private const val CACHED_DEPTH_MAP = "depth.png"

class MainActivity : ComponentActivity() {
    private lateinit var mStorageUtils: StorageUtils
    private lateinit var mDepthEstimator: DepthEstimator

    private var mImages: Pair<ByteArray, ByteArray>? = null
    private var mDepthMap by mutableStateOf<Bitmap?>(null)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        mStorageUtils = StorageUtils(this)
        with(mStorageUtils) {
            getFromCache(CACHED_PARAMS)?.run { mDepthEstimator = DepthEstimator(path) }
            getFromCache(CACHED_LEFT_IMAGE)?.let { left ->
                getFromCache(CACHED_RIGHT_IMAGE)?.let { right ->
                    mImages = left.readBytes() to right.readBytes()
                }
            }
            getFromCache(CACHED_DEPTH_MAP)?.run { mDepthMap = BitmapFactory.decodeFile(path) }
        }

        setContent {
            MaterialTheme {
                val scope = rememberCoroutineScope()
                val scaffoldState = rememberScaffoldState()
                var depthEstimatorReady by remember {
                    mutableStateOf(this::mDepthEstimator.isInitialized)
                }
                var imagesReady by remember { mutableStateOf(mImages != null) }
                var calculationTimeNanos by rememberSaveable { mutableStateOf<Long?>(null) }

                Scaffold(
                    scaffoldState = scaffoldState,
                    bottomBar = {
                        fun displayError(message: String) =
                            scope.launch { scaffoldState.snackbarHostState.showSnackbar(message) }

                        MenuButtons(
                            showPickImagesButton = depthEstimatorReady,
                            showRunButton = depthEstimatorReady && imagesReady,
                            onParamsPick = { uri ->
                                getDepthEstimator(uri)?.let {
                                    mDepthEstimator = it
                                    depthEstimatorReady = true
                                } ?: displayError("Cannot open calibration parameters")
                            },
                            onImagesPick = { left, right ->
                                with(contentResolver) {
                                    openInputStream(left)?.use { leftStream ->
                                        openInputStream(right)?.use { rightStream ->
                                            leftStream.readBytes() to rightStream.readBytes()
                                        }
                                    }
                                }?.let {
                                    mImages = it
                                    imagesReady = true
                                } ?: displayError("Cannot open images")
                            },
                            onRunClick = {
                                mImages?.let { (left, right) ->
                                    val (bitmap, time) = getDepthMap(left, right)
                                    mDepthMap = bitmap
                                    calculationTimeNanos = time
                                } ?: throw IllegalStateException("Run clicked with no images")
                            },
                            onError = ::displayError
                        )
                    },
                    backgroundColor = MaterialTheme.colors.background
                ) { contentPadding ->
                    Column(
                        modifier = Modifier
                            .fillMaxSize()
                            .padding(contentPadding),
                        verticalArrangement = Arrangement.SpaceBetween,
                        horizontalAlignment = Alignment.CenterHorizontally
                    ) {
                        mDepthMap?.run {
                            Image(
                                bitmap = asImageBitmap(),
                                contentDescription = "Depth map",
                                modifier = Modifier.weight(1f)
                            )
                        }

                        calculationTimeNanos?.let { time ->
                            Text(text = "Calculation time: ${time * 1e-9} sec")
                        }

                        if (depthEstimatorReady && imagesReady) {
                            ScaleFactorField { mDepthEstimator.setImageScaleFactor(it) }
                        }
                    }
                }
            }
        }
    }

    private fun getDepthEstimator(uri: Uri): DepthEstimator? =
        with(mStorageUtils) { uri.copyToCache(CACHED_PARAMS) }?.run { DepthEstimator(path) }

    private fun getDepthMap(leftImage: ByteArray, rightImage: ByteArray): Pair<Bitmap, Long> {
        val startTimeNanos = SystemClock.elapsedRealtimeNanos()
        val depthArray = mDepthEstimator.estimateDepth(leftImage, rightImage)
        val calculationTimeNanos = SystemClock.elapsedRealtimeNanos() - startTimeNanos
        return depthArrayToBitmap(depthArray) to calculationTimeNanos
    }

    override fun onDestroy() {
        super.onDestroy()

        with(mStorageUtils) {
            mImages?.let { (left, right) ->
                left.writeToCache(CACHED_LEFT_IMAGE)
                right.writeToCache(CACHED_RIGHT_IMAGE)
            }
            mDepthMap?.writeToCache(CACHED_DEPTH_MAP)
        }
    }
}
