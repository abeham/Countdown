## Countdown

Countdown is a simple class that calls a function after a certain amount of time has passed. The countdown can be aborted at any time.

### Usage

```cpp
#include <iostream>
#include <thread>
#include <chrono>

#include "Countdown.hpp"

int main() {
    Countdown countdown(std::chrono::milliseconds{50}, []() {
        std::cout << "Countdown finished!" << std::endl;
    });

    this_thread::sleep_for(std::chrono::milliseconds{100});
    // countdown would be aborted if it would go out of scope
}
```

### Installation

You can either copy the `Countdown.hpp` file into your project or integrate the project, e.g. via [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake):
    
    ```cmake
    include(cmake/CPM.cmake)

    CPMAddPackage(
        NAME Countdown
        GITHUB_REPOSITORY abeham/Countdown
        VERSION 1.0
    )
    ```
