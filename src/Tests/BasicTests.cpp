#include <catch2/catch_test_macros.hpp>

#include "Countdown.hpp"

TEST_CASE( "Basic usage of Countdown", "[Countdown]" )
{
    const int timeoutms = 100;
    bool called = false;
    Countdown cd{std::chrono::milliseconds{timeoutms}, [&called] () {
        called = true;
    }};
    SECTION("Wait for callback")
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeoutms * 2));
        REQUIRE(called);
    }
    SECTION("Abort")
    {
        cd.Abort();
        std::this_thread::sleep_for(std::chrono::milliseconds(timeoutms * 2));
        REQUIRE(!called);
    }
    SECTION("Just quit without waiting")
    {
        REQUIRE(!called);
    }
    SECTION("Just Abort without waiting")
    {
        cd.Abort();
        REQUIRE(!called);
    }
}

TEST_CASE( "Countdown with different durations", "[Countdown]" )
{
    const int timeoutms = 100;
    bool called = false;
    Countdown cd{std::chrono::milliseconds{timeoutms}, [&called] () {
        called = true;
    }};
    std::this_thread::sleep_for(std::chrono::milliseconds(timeoutms / 2));
    REQUIRE(!called);
    std::this_thread::sleep_for(std::chrono::milliseconds(timeoutms));
    REQUIRE(called);
}

TEST_CASE( "Multiple Countdowns", "[Countdown]" )
{
    const int timeoutms = 100;
    bool called1 = false, called2 = false;
    Countdown cd1{std::chrono::milliseconds{timeoutms}, [&called1] () {
        called1 = true;
    }};
    Countdown cd2{std::chrono::milliseconds{timeoutms}, [&called2] () {
        called2 = true;
    }};
    std::this_thread::sleep_for(std::chrono::milliseconds(timeoutms * 2));
    REQUIRE(called1);
    REQUIRE(called2);
}

TEST_CASE( "Aborting multiple times has no effect", "[Countdown]" )
{
    const int timeoutms = 100;
    bool called = false;
    Countdown cd{std::chrono::milliseconds{timeoutms}, [&called] () {
        called = true;
    }};
    cd.Abort();
    cd.Abort();
    std::this_thread::sleep_for(std::chrono::milliseconds(timeoutms * 2));
    REQUIRE(!called);
}

TEST_CASE( "Aborting after callback has no effect", "[Countdown]" )
{
    const int timeoutms = 100;
    bool called = false;
    Countdown cd{std::chrono::milliseconds{timeoutms}, [&called] () {
        called = true;
    }};
    std::this_thread::sleep_for(std::chrono::milliseconds(timeoutms * 2));
    cd.Abort();
    REQUIRE(called);
}

TEST_CASE( "Going out of scope also aborts the countdown", "[Countdown]" )
{
    const int timeoutms = 100;
    bool called = false;
    {
        Countdown cd{std::chrono::milliseconds{timeoutms}, [&called] () {
            called = true;
        }};
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(timeoutms * 2));
    REQUIRE(!called);
}