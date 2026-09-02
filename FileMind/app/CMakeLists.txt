cmake_minimum_required(VERSION 3.22.1)
project(filemind_android LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-fexceptions -frtti)

# Reuse the original mini_ai source and FileMind core; do not duplicate inference.
set(MINI_AI_ROOT "${CMAKE_CURRENT_LIST_DIR}/../..")
set(FILEMIND_ROOT "${MINI_AI_ROOT}/filemind")

add_library(mini_ai_core STATIC "${MINI_AI_ROOT}/src/mini_ai.cpp")
target_include_directories(mini_ai_core PUBLIC "${MINI_AI_ROOT}/include")
set_target_properties(mini_ai_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(filemind_core STATIC
    "${FILEMIND_ROOT}/src/mini_ai_service.cpp"
    "${FILEMIND_ROOT}/src/context_engine.cpp"
    "${FILEMIND_ROOT}/src/file_engine.cpp"
    "${FILEMIND_ROOT}/src/intent_parser.cpp"
    "${FILEMIND_ROOT}/src/orchestrator.cpp"
    "${FILEMIND_ROOT}/src/runtime.cpp"
)
target_include_directories(filemind_core PUBLIC "${FILEMIND_ROOT}/include")
set_target_properties(filemind_core PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(filemind_core PUBLIC mini_ai_core)

find_library(log-lib log)
add_library(filemind_ai SHARED src/main/cpp/jni/filemind_jni.cpp)
target_include_directories(filemind_ai PRIVATE "${FILEMIND_ROOT}/include")
target_link_libraries(filemind_ai PRIVATE filemind_core ${log-lib})
