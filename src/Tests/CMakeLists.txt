include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

CPMAddPackage("gh:catchorg/Catch2@3.7.0")
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)

add_executable(Tests BasicTests.cpp)

target_link_libraries(Tests PUBLIC Countdown Catch2::Catch2WithMain)
target_compile_features(Tests PRIVATE cxx_std_20)

include(CTest)
include(Catch)

catch_discover_tests(Tests)

# Add a test case
#add_test(NAME MyTest COMMAND test_main)
