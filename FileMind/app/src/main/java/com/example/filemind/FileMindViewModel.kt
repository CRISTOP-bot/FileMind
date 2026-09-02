package com.example.filemind

import android.app.Application
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch
import java.io.File

class FileMindViewModel(app: Application) : AndroidViewModel(app) {
    private val _ready = MutableStateFlow(false)
    val ready: StateFlow<Boolean> = _ready.asStateFlow()
    private val _answer = MutableStateFlow("Cargando FileMind…")
    val answer: StateFlow<String> = _answer.asStateFlow()
    private var handle: Long = 0

    init {
        viewModelScope.launch(Dispatchers.IO) {
            val target = File(app.filesDir, "model.mai3")
            try {
                if (!target.exists()) app.assets.open("model.mai3").use { input -> target.outputStream().use { output -> input.copyTo(output) } }
                handle = NativeFileMind.nativeInitialize(target.absolutePath)
                _ready.value = handle != 0L && NativeFileMind.nativeIsReady(handle)
                if (!_ready.value) _answer.value = "No se pudo cargar el checkpoint."
            } catch (_: Exception) {
                _answer.value = "Falta app/src/main/assets/model.mai3."
            }
        }
    }

    fun process(message: String) {
        if (message.isBlank() || handle == 0L) return
        viewModelScope.launch(Dispatchers.IO) { _answer.value = NativeFileMind.nativeProcess(handle, message) }
    }

    override fun onCleared() {
        if (handle != 0L) NativeFileMind.nativeShutdown(handle)
        handle = 0
        super.onCleared()
    }
}
