cmake_minimum_required(VERSION 3.15.0)

add_library(Countdown INTERFACE)
add_library(Countdown::Countdown ALIAS Countdown)

target_compile_features(Countdown INTERFACE cxx_std_20)
target_include_directories(Countdown INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)


