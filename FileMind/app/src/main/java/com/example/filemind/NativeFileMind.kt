package com.example.filemind

object NativeFileMind {
    init { System.loadLibrary("filemind_ai") }

    external fun nativeInitialize(checkpoint: String): Long
    external fun nativeProcess(handle: Long, message: String): String
    external fun nativeIsReady(handle: Long): Boolean
    external fun nativeShutdown(handle: Long)
}
