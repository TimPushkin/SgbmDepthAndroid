package me.timpushkin.sgbmandroidapp.ui

import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.ImageBitmap

@Composable
fun ResultsScreen(
    modifier: Modifier = Modifier,
    depthMap: ImageBitmap? = null,
    calculationTimeNanos: Long? = null
) {
    Column(
        modifier = Modifier
            .fillMaxSize()
            .then(modifier),
        verticalArrangement = Arrangement.SpaceBetween,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        depthMap?.let { bitmap ->
            Image(
                bitmap = bitmap,
                contentDescription = "Depth map",
                modifier = Modifier.weight(1f)
            )
        }

        calculationTimeNanos?.let { time -> Text(text = "Calculation time: ${time * 1e-9} sec") }
    }
}
