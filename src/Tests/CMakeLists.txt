add_executable(Tests main.cpp)

target_link_libraries(Tests PRIVATE Countdown)

# Enable testing
enable_testing()

# Add a test case
#add_test(NAME MyTest COMMAND test_main)
