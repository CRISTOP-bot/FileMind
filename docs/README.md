# Documentación de mini_ai C++20

// DOCUMENTACIÓN_AQUÍ: Este reemplazo C++20 contiene los módulos de tokenizer, Dataset,
// Tensor, Model y Adam. El modelo ejecuta embeddings, atención causal (sin mirar el futuro),
// MLP residual y softmax cross-entropy.

Consulta `../README.md` para compilar, entrenar y generar. Los checkpoints MAI3 incluyen
parámetros, momentos de Adam y contadores para reanudar de forma consistente.

## Lecturas de la consola

// DOCUMENTACIÓN_AQUÍ: `step ... loss ...` es el **train loss** de los mini-lotes actuales.
// Esta versión educativa todavía no separa un conjunto de validación; por ello no imprime
// **val loss**. No debe confundirse una ausencia de val loss con un entrenamiento exitoso.
// `saved` y `resumed` son estados informativos; errores de entrada/checkpoint deben producir
// código 1 (FAIL), mientras que compilación, tests y ejecución correcta son PASS.
