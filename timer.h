#ifndef TIMER_H
#define TIMER_H

#include <ctime>

class ClockTimer
{
public:
    ClockTimer();

    void reset();
    static std::size_t now();
    std::size_t get_elapsed() const;
private:
    std::size_t start;
};

inline ClockTimer::ClockTimer ()
{
    reset();
}

inline void ClockTimer::reset ()
{
    start = ClockTimer::now();
}

inline std::size_t ClockTimer::now ()
{
    return static_cast<std::size_t>(std::clock()) * 1000
        / static_cast<std::size_t>(CLOCKS_PER_SEC);
}

inline std::size_t ClockTimer::get_elapsed () const
{
    return ClockTimer::now() - start;
}

#endif // TIMER_H
