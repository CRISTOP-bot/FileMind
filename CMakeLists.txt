cmake_minimum_required(VERSION 3.20)

project(mini_ai_cpp20 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(MINI_AI_SANITIZERS "Enable address/undefined behavior sanitizers" OFF)
if(MINI_AI_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

add_library(mini_ai STATIC
    src/mini_ai.cpp
)
target_include_directories(mini_ai PUBLIC include)
target_compile_options(mini_ai PRIVATE -Wall -Wextra -Wpedantic)

add_executable(mini_ai_train src/apps/train.cpp)
target_link_libraries(mini_ai_train PRIVATE mini_ai)

add_executable(mini_ai_test src/apps/test.cpp)
target_link_libraries(mini_ai_test PRIVATE mini_ai)

enable_testing()
add_executable(mini_ai_unit tests/unit.cpp)
target_link_libraries(mini_ai_unit PRIVATE mini_ai)
add_test(NAME unit COMMAND mini_ai_unit)

add_library(filemind STATIC
    filemind/src/mini_ai_service.cpp
    filemind/src/context_engine.cpp
    filemind/src/file_engine.cpp
    filemind/src/intent_parser.cpp
    filemind/src/orchestrator.cpp
    filemind/src/runtime.cpp
)
target_include_directories(filemind PUBLIC filemind/include)
target_link_libraries(filemind PUBLIC mini_ai)
target_compile_features(filemind PUBLIC cxx_std_20)
target_compile_options(filemind PRIVATE -Wall -Wextra -Wpedantic)

add_executable(mini_ai_filemind filemind/src/main.cpp)
target_link_libraries(mini_ai_filemind PRIVATE filemind)

add_executable(filemind_unit filemind/tests/unit.cpp)
target_link_libraries(filemind_unit PRIVATE filemind)
add_test(NAME filemind_unit COMMAND filemind_unit)
