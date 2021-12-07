#ifndef PRECISETIMER_H
#define PRECISETIMER_H

#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <functional>

#include "AtmegaTimer.h"

class PreciseTimer
{
public:

    PreciseTimer();
    ~PreciseTimer();

    void stop();

    void start(AtmegaTimer& timer, std::function<void(AtmegaTimer&r)> func);

    bool isRunning() const noexcept;

    void setTimeout(long timeout);

private:

    std::atomic<bool> _execute;
    std::thread _thd;

    long _timeout;

};

#endif // PRECISETIMER_H
