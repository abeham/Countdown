cmake_minimum_required(VERSION 3.15)
project(Countdown VERSION 1.0.0 LANGUAGES CXX)

# Options
option(BUILD_TESTS "Build the tests" ON)
option(BUILD_EXAMPLES "Build the examples" ON)
option(LIBRARY_ONLY "Library only build (if the library is included in another repo)" OFF)

# Add the main library
add_subdirectory(src/Countdown)

# Add tests if enabled
if(BUILD_TESTS AND NOT LIBRARY_ONLY)
    add_subdirectory(src/Tests)
endif()

# Add examples if enabled
if(BUILD_EXAMPLES AND NOT LIBRARY_ONLY)
    add_subdirectory(src/Example)
endif()