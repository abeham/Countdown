#include <iostream>
#include <chrono>
#include <thread>

#include "date/date.h"

#include "Countdown.hpp"


void print(std::string);

int main()
{
    const int timeoutms = 100;
    print(std::format("Starting 1st countdown in {}ms", timeoutms));
    Countdown cd{std::chrono::milliseconds{timeoutms}, [] () {
        print("Callback!");
    }};
    print("Waiting for the callback");
    std::this_thread::sleep_for(std::chrono::milliseconds(timeoutms * 2));
    std::cout << std::endl;
    {
        print(std::format("Starting 2nd countdown in {}ms", timeoutms));
        Countdown cd3{std::chrono::milliseconds{timeoutms}, [] () {
            print("Callback!");
        }};
        print("Going out of scope wihtout waiting for the callback");
    }
    std::cout << std::endl;
    print(std::format("Starting 3rd countdown in {}ms", timeoutms));
    Countdown cd2{std::chrono::milliseconds{timeoutms}, [] () {
        print("Callback!");
    }};
    print("Aborting the 3rd countdown");
    cd2.Abort();
    print("Waiting to be sure callback isn't called");
    std::this_thread::sleep_for(std::chrono::milliseconds(timeoutms * 2));
    std::cout << std::endl;
    print("Done");
    return 0;
}

void print(std::string msg)
{
    auto now = std::chrono::system_clock::now();
    std::cout << now << " " << msg << "\n";
}