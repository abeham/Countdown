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

