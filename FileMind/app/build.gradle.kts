plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.android")
    id("org.jetbrains.kotlin.plugin.compose")
}

android { namespace = "com.example.filemind"; compileSdk = 35
    defaultConfig { applicationId = "com.example.filemind"; minSdk = 26; targetSdk = 35; versionCode = 1; versionName = "0.1" }
    externalNativeBuild { cmake { path = file("CMakeLists.txt"); version = "3.22.1" } }
    buildTypes { release { isMinifyEnabled = false } }
}

kotlin { jvmToolchain(17) }

// Keep only ARM64 for the first local-first prototype.
android.defaultConfig.ndk { abiFilters += "arm64-v8a" }

dependencies {
    implementation(platform("androidx.compose:compose-bom:2024.09.03"))
    implementation("androidx.activity:activity-compose:1.9.2")
    implementation("androidx.compose.ui:ui")
    implementation("androidx.compose.foundation:foundation")
    implementation("androidx.compose.material3:material3")
    implementation("androidx.lifecycle:lifecycle-viewmodel-compose:2.8.6")
    implementation("org.jetbrains.kotlinx:kotlinx-coroutines-android:1.9.0")
}
