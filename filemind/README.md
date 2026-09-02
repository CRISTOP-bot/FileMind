# FileMind C++ MVP

Capa superior mínima alrededor de `mini_ai::Model`. No duplica la inferencia.

## Componentes

- `MiniAIService`: carga el checkpoint una vez y protege el modelo con mutex.
- `ContextEngine`: historial limitado a 32 bytes, priorizando el mensaje actual.
- `FileEngine`: operaciones locales con rutas confinadas al directorio raíz.
- `IntentParser`: acepta únicamente objetos con `action` explícita y campos acotados.
- `FileMindOrchestrator`: conecta inferencia, intención y operaciones de archivos.

El modelo actual es un LM de bytes pequeño y todavía no produce tool calls fiables. Por
eso el orquestador usa un fallback léxico pequeño y seguro para consultas básicas de
archivos cuando la salida del modelo no es una intención estructurada válida. No hay web,
Wikipedia, JNI, Kotlin ni lectura de documentos en esta fase.

## CLI

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
./build/mini_ai_filemind mini_ai.ckpt "Busca mis archivos .cpp"
```
