#include "filemind/runtime.hpp"

#include <jni.h>
#include <memory>
#include <string>

namespace {
filemind::FileMindRuntime* from_handle(jlong handle) {
    return reinterpret_cast<filemind::FileMindRuntime*>(handle);
}
std::string from_java(JNIEnv* env, jstring value) {
    if (!value) return {};
    const char* chars = env->GetStringUTFChars(value, nullptr);
    if (!chars) return {};
    std::string result(chars);
    env->ReleaseStringUTFChars(value, chars);
    return result;
}
jstring to_java(JNIEnv* env, const std::string& value) {
    return env->NewStringUTF(value.c_str());
}
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_example_filemind_NativeFileMind_nativeInitialize(JNIEnv* env, jclass, jstring checkpoint) {
    try {
        auto* runtime = new filemind::FileMindRuntime();
        if (!runtime->initialize(from_java(env, checkpoint))) {
            delete runtime;
            return 0;
        }
        return reinterpret_cast<jlong>(runtime);
    } catch (...) {
        return 0;
    }
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_filemind_NativeFileMind_nativeProcess(JNIEnv* env, jclass, jlong handle, jstring message) {
    try {
        auto* runtime = from_handle(handle);
        return runtime ? to_java(env, runtime->process(from_java(env, message))) : to_java(env, {});
    } catch (...) {
        return to_java(env, "Error interno de FileMind.");
    }
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_example_filemind_NativeFileMind_nativeIsReady(JNIEnv*, jclass, jlong handle) {
    auto* runtime = from_handle(handle);
    return runtime && runtime->is_ready() ? JNI_TRUE : JNI_FALSE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_filemind_NativeFileMind_nativeShutdown(JNIEnv*, jclass, jlong handle) {
    auto* runtime = from_handle(handle);
    if (runtime) {
        runtime->shutdown();
        delete runtime;
    }
}
