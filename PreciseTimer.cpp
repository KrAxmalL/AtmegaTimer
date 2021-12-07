#include "PreciseTimer.h"

PreciseTimer::PreciseTimer():_execute(false) {}

PreciseTimer::~PreciseTimer()
{
    if( _execute.load(std::memory_order_acquire) ) {
        stop();
    }
}

void PreciseTimer::stop()
{
    _execute.store(false, std::memory_order_release);
    if( _thd.joinable() )
    {
        _thd.join();
    }
}

void PreciseTimer::start(AtmegaTimer& timer, std::function<void(AtmegaTimer&)> func)
{
    if( _execute.load(std::memory_order_acquire) ) {
        stop();
    }
    _execute.store(true, std::memory_order_release);
    _thd = std::thread([this, func, &timer]()
    {
        while (_execute.load(std::memory_order_acquire))
        {
            func(timer);
            std::this_thread::sleep_for(
            std::chrono::nanoseconds(_timeout));
        }
    });
}

void PreciseTimer::setTimeout(long timeout)
{
    _timeout = timeout;
}

bool PreciseTimer::isRunning() const noexcept
{
    return (_execute.load(std::memory_order_acquire) &&
            _thd.joinable() );
}
