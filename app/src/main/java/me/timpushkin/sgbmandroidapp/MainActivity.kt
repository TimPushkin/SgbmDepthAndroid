package me.timpushkin.sgbmandroidapp

import android.graphics.Bitmap
import android.net.Uri
import android.os.Bundle
import android.os.SystemClock
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.viewModels
import androidx.compose.foundation.layout.padding
import androidx.compose.material.MaterialTheme
import androidx.compose.material.Scaffold
import androidx.compose.material.rememberScaffoldState
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.asImageBitmap
import kotlinx.coroutines.launch
import me.timpushkin.sgbmandroid.DepthEstimator
import me.timpushkin.sgbmandroidapp.ui.AppState
import me.timpushkin.sgbmandroidapp.ui.Menu
import me.timpushkin.sgbmandroidapp.ui.ResultsScreen
import me.timpushkin.sgbmandroidapp.utils.StorageUtils
import me.timpushkin.sgbmandroidapp.utils.depthArrayToBitmap

private const val CACHED_PARAMS = "params.xml"

class MainActivity : ComponentActivity() {
    private val appState by viewModels<AppState>()
    private lateinit var mStorageUtils: StorageUtils

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        mStorageUtils = StorageUtils(this)

        if (appState.depthEstimator == null) {
            mStorageUtils.getFromCache(CACHED_PARAMS)?.let { params ->
                appState.depthEstimator = DepthEstimator(params.path)
            }
        }

        setContent {
            MaterialTheme {
                val scope = rememberCoroutineScope()
                val scaffoldState = rememberScaffoldState()

                Scaffold(
                    scaffoldState = scaffoldState,
                    bottomBar = {
                        fun displayError(message: String) =
                            scope.launch { scaffoldState.snackbarHostState.showSnackbar(message) }

                        Menu(
                            showPickImagesButton = appState.depthEstimator != null,
                            showRunControls = appState.depthEstimator != null &&
                                appState.images != null,
                            onParamsPick = { uri ->
                                getDepthEstimator(uri)?.let { appState.depthEstimator = it }
                                    ?: displayError("Cannot open calibration parameters")
                            },
                            onImagesPick = { images ->
                                mStorageUtils.readPair(images)?.let { appState.images = it }
                                    ?: displayError("Cannot open images")
                            },
                            onScaleFactorSet = { scaleFactor ->
                                appState.depthEstimator?.run { setImageScaleFactor(scaleFactor) }
                                    ?: throw IllegalStateException(
                                        "Scale factor set with no DepthEstimator"
                                    )
                            },
                            onRunClick = {
                                appState.images?.let { (left, right) ->
                                    val (bitmap, time) = getDepthMap(left, right)
                                    appState.depthMap = bitmap
                                    appState.calculationTimeNanos = time
                                } ?: throw IllegalStateException("Run clicked with no images")
                            },
                            onError = ::displayError
                        )
                    },
                    backgroundColor = MaterialTheme.colors.background
                ) { contentPadding ->
                    ResultsScreen(
                        modifier = Modifier.padding(contentPadding),
                        depthMap = appState.depthMap?.asImageBitmap(),
                        calculationTimeNanos = appState.calculationTimeNanos
                    )
                }
            }
        }
    }

    private fun getDepthEstimator(uri: Uri): DepthEstimator? =
        mStorageUtils.copyToCache(uri, CACHED_PARAMS)?.let { params -> DepthEstimator(params.path) }

    private fun getDepthMap(leftImage: ByteArray, rightImage: ByteArray): Pair<Bitmap, Long> {
        appState.depthEstimator?.run {
            val startTimeNanos = SystemClock.elapsedRealtimeNanos()
            val depthArray = estimateDepth(leftImage, rightImage)
            val calculationTimeNanos = SystemClock.elapsedRealtimeNanos() - startTimeNanos
            return depthArrayToBitmap(depthArray) to calculationTimeNanos
        } ?: throw IllegalStateException("Cannot get depth map without DepthEstimator")
    }
}
