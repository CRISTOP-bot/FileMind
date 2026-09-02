package com.example.filemind

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.material3.TextField
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.lifecycle.viewmodel.compose.viewModel

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent { FileMindScreen() }
    }
}

@Composable
private fun FileMindScreen(model: FileMindViewModel = viewModel()) {
    val answer by model.answer.collectAsState()
    val ready by model.ready.collectAsState()
    var message by remember { mutableStateOf("") }
    MaterialTheme {
        Column(Modifier.fillMaxSize().padding(20.dp), verticalArrangement = Arrangement.spacedBy(16.dp)) {
            Text("FileMind", style = MaterialTheme.typography.headlineMedium)
            Text(if (ready) "● IA lista" else "○ Cargando checkpoint…")
            Text(answer, Modifier.weight(1f))
            Row(Modifier.fillMaxWidth(), horizontalArrangement = Arrangement.spacedBy(8.dp)) {
                TextField(message, { message = it }, Modifier.weight(1f), placeholder = { Text("Escribe una consulta…") })
                Button(onClick = { model.process(message) }, enabled = ready && message.isNotBlank()) { Text("➤") }
            }
        }
    }
}
