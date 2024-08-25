cmake_minimum_required(VERSION 3.15.0)

include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)
CPMAddPackage(
    NAME date
    GITHUB_REPOSITORY HowardHinnant/date
    VERSION 3.0.1
)

add_executable(Example main.cpp)

target_link_libraries(Example PRIVATE Countdown date::date)

