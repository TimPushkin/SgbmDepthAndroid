package me.timpushkin.sgbmandroidapp.ui

import android.net.Uri
import android.webkit.MimeTypeMap
import androidx.activity.compose.rememberLauncherForActivityResult
import androidx.activity.result.contract.ActivityResultContracts
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.text.KeyboardActions
import androidx.compose.foundation.text.KeyboardOptions
import androidx.compose.material.Button
import androidx.compose.material.OutlinedTextField
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.input.KeyboardType

private val XML_MIME = MimeTypeMap.getSingleton().getMimeTypeFromExtension("xml") ?: "*/*"
private const val IMG_MIME = "image/*"

@Composable
fun Menu(
    showPickImagesButton: Boolean,
    showRunControls: Boolean,
    onParamsPick: (Uri) -> Unit,
    onImagesPick: (Pair<Uri, Uri>) -> Unit,
    onScaleFactorSet: (Float) -> Unit,
    onRunClick: () -> Unit,
    onError: (String) -> Unit
) {
    Column(
        modifier = Modifier.fillMaxWidth(),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        val paramsPicker =
            rememberLauncherForActivityResult(ActivityResultContracts.OpenDocument()) { uri ->
                if (uri != null) onParamsPick(uri) else onError("No parameters picked")
            }

        val imagesPicker =
            rememberLauncherForActivityResult(
                ActivityResultContracts.OpenMultipleDocuments()
            ) { uris ->
                if (uris.size == 2) {
                    onImagesPick(uris[0] to uris[1])
                } else {
                    onError("Expected two images but was ${uris.size}")
                }
            }

        if (showRunControls) ScaleFactorField(onScaleFactorSet)

        Row(
            modifier = Modifier.fillMaxWidth(),
            horizontalArrangement = Arrangement.SpaceEvenly
        ) {
            Button(onClick = { paramsPicker.launch(arrayOf(XML_MIME)) }) {
                Text("Load parameters")
            }

            if (showPickImagesButton) {
                Button(onClick = { imagesPicker.launch(arrayOf(IMG_MIME)) }) {
                    Text("Load images")
                }
            }

            if (showRunControls) {
                Button(onClick = onRunClick) {
                    Text("Run")
                }
            }
        }
    }
}

@Composable
private fun ScaleFactorField(onScaleFactorSet: (Float) -> Unit) {
    var text by rememberSaveable { mutableStateOf("1") }

    OutlinedTextField(
        value = text,
        onValueChange = { text = it },
        label = { Text(text = "Scale factor") },
        singleLine = true,
        keyboardOptions = KeyboardOptions(keyboardType = KeyboardType.Decimal),
        keyboardActions = KeyboardActions {
            val scaleFactor = text.toFloatOrNull()?.coerceIn(0.1f, 1f) ?: 1f
            text = scaleFactor.toString()
            onScaleFactorSet(scaleFactor)
        }
    )
}
