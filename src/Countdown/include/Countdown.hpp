//The MIT License
//
//Copyright © 2024 Andreas Beham
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the “Software”), to deal
//in the Software without restriction, including without limitation the right
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in
//all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <array>
#include <iostream>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <thread>
#include <functional>

class [[nodiscard]] Countdown
{
    std::mutex _mutex;
    std::condition_variable _cv;
    std::jthread _timer_thread;
    
    std::function<void()> _callback;
    bool _abort_requested;

    template<class Rep, class Period = std::ratio<1>>
    void timeout(std::chrono::duration<Rep, Period> duration)
    {
        std::unique_lock lk(_mutex);
        _cv.wait_for(lk, duration, [&abort_requested = _abort_requested]() { return abort_requested; }); // wakeup after duration isn't dependent on the predicate
        if (!_abort_requested) _callback();
    }

public:
    template<class Rep, class Period = std::ratio<1>>
    Countdown(std::chrono::duration<Rep, Period> duration, std::function<void()>&& callback)
        : _callback(std::forward<std::function<void()>>(callback)), _abort_requested(false)
    {
        _timer_thread = std::jthread{std::bind_front(&Countdown::timeout<Rep, Period>, this, duration)};
    }
    Countdown(const Countdown&) = delete;
    Countdown& operator=(const Countdown&) = delete;
    ~Countdown()
    {
        Abort();
        // join the thread to ensure that the other members are not destroyed before the thread has finished
        if (_timer_thread.joinable()) _timer_thread.join();
    }

    void Abort()
    {
        bool notify = false;
        {
            std::lock_guard lk(_mutex);
            if (!_abort_requested)
            {
                _abort_requested = true;
                notify = true;
            }
        }
        if (notify) _cv.notify_one();
    }
};

