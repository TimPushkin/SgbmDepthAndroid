package me.timpushkin.sgbmandroidapp.ui

import android.graphics.Bitmap
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.setValue
import androidx.lifecycle.ViewModel
import me.timpushkin.sgbmandroid.DepthEstimator

class AppState : ViewModel() {
    var depthEstimator by mutableStateOf<DepthEstimator?>(null)
    var images by mutableStateOf<Pair<ByteArray, ByteArray>?>(null)
    var depthMap by mutableStateOf<Bitmap?>(null)
    var calculationTimeNanos by mutableStateOf<Long?>(null)
}
