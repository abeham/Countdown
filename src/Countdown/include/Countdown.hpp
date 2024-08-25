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

template<typename Callback, class Rep, class Period = std::ratio<1>>
class Countdown
{
    std::mutex _mutex;
    std::condition_variable _cv;
    std::thread _timer_thread;
    
    std::chrono::duration<Rep, Period> _duration;
    Callback _callback;
    bool _abort_requested, _callback_called;

    void timeout()
    {
        std::unique_lock lk(_mutex);
        _cv.wait_for(lk, _duration);
        if (!_abort_requested) _callback();
        _callback_called = true;
    }

public:
    Countdown(std::chrono::duration<Rep, Period> duration, Callback&& callback)
        : _duration(duration), _callback(callback), _abort_requested(false), _callback_called(false)
    {
        _timer_thread = std::thread{&Countdown::timeout, this};
    }
    ~Countdown()
    {
        if (!_callback_called && !_abort_requested)
        {
            std::lock_guard lk(_mutex);
            if (!_callback_called && !_abort_requested)
            {
                _abort_requested = true;
                _cv.notify_one();
            }
        }
        if (_timer_thread.joinable()) _timer_thread.join();
    }

    void Abort()
    {
        if (!_abort_requested && !_callback_called)
        {
            std::lock_guard lk(_mutex);
            if (!_abort_requested && !_callback_called)
            {
                _abort_requested = true;
                _cv.notify_one();
            }
        }
    }
};

