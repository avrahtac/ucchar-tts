cmake_minimum_required(VERSION 3.16)
project(ucchar-tts VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# includes
include_directories(include)
include_directories(libs/nlohmann)
include_directories(libs/onnxruntime/include)

# engine library (reusable core)
add_library(ucchar_engine
    engine/swara/swara_table.cpp
    engine/prosody/prosody.cpp
    engine/ornament/ornament.cpp
    engine/vocoder/vocoder.cpp
    engine/audio/audio_writer.cpp
)

# main binary
add_executable(ucchar
    src/main.cpp
    src/cli.cpp
    src/json_input.cpp
)
target_link_libraries(ucchar ucchar_engine)

# tests
enable_testing()
add_executable(ucchar_tests
    tests/test_swara.cpp
    tests/test_prosody.cpp
    tests/test_ornament.cpp
    tests/test_json.cpp
)
target_link_libraries(ucchar_tests ucchar_engine gtest gtest_main)
add_test(NAME ucchar_tests COMMAND ucchar_tests)