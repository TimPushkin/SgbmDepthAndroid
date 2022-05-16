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
import me.timpushkin.sgbmandroidapp.utils.StorageUtils
import me.timpushkin.sgbmandroidapp.utils.depthArrayToBitmap

private const val CACHED_PARAMS = "params.xml"
private const val CACHED_DEPTHMAP = "depth.png"

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
                var calculationTimeNanos by rememberSaveable { mutableStateOf<Long?>(null) }

                Scaffold(
                    scaffoldState = scaffoldState,
                    bottomBar = {
                        var canEstimateDepth by remember {
                            mutableStateOf(this::mDepthEstimator.isInitialized)
                        }

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
                                getDepthMap(left, right)?.let { (bitmap, time) ->
                                    depthMap = bitmap
                                    calculationTimeNanos = time
                                } ?: displayError("Cannot open images")
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
                        depthMap?.let { bitmap ->
                            Image(
                                bitmap = bitmap.asImageBitmap(),
                                contentDescription = "Depth map",
                                modifier = Modifier.weight(1f)
                            )
                        }

                        calculationTimeNanos?.let { time ->
                            Text(text = "Calculation time: ${time * 1e-9} sec")
                        }
                    }
                }
            }
        }
    }

    private fun getDepthEstimator(uri: Uri): DepthEstimator? =
        with(mStorageUtils) { uri.copyToCache(CACHED_PARAMS) }?.let { params ->
            DepthEstimator(params.path)
        }

    private fun getDepthMap(leftUri: Uri, rightUri: Uri): Pair<Bitmap, Long>? {
        val (leftBytes, rightBytes) =
            contentResolver.openInputStream(leftUri)?.use { leftStream ->
                contentResolver.openInputStream(rightUri)?.use { rightStream ->
                    leftStream.readBytes() to rightStream.readBytes()
                }
            } ?: return null

        val startTimeNanos = SystemClock.elapsedRealtimeNanos()
        val depthArray = mDepthEstimator.estimateDepth(leftBytes, rightBytes)
        val calculationTimeNanos = SystemClock.elapsedRealtimeNanos() - startTimeNanos

        val depthMap = depthArrayToBitmap(depthArray)
        with(mStorageUtils) { depthMap.writeToCache(CACHED_DEPTHMAP) }

        return depthMap to calculationTimeNanos
    }
}
