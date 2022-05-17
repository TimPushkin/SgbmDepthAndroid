package me.timpushkin.sgbmandroidapp.ui

import androidx.compose.foundation.text.KeyboardActions
import androidx.compose.foundation.text.KeyboardOptions
import androidx.compose.material.OutlinedTextField
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.runtime.setValue
import androidx.compose.ui.text.input.KeyboardType

@Composable
fun ScaleFactorField(onScaleFactorSet: (Float) -> Unit) {
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
