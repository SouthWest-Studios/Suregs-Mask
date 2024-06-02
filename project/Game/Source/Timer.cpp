// ----------------------------------------------------
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "Timer.h"
#include "SDL\include\SDL_timer.h"

// L1: DONE 1: Fill Start(), Read(), ReadSec() methods
// they are simple, one line each!

Timer::Timer() : started(false) // Inicializa el indicador started
{
    Start();
}

void Timer::Start(uint32 startingMS)
{
    started = true;
    startTime = SDL_GetTicks() - startingMS;
}

void Timer::Stop()
{
    started = false;
}

uint32 Timer::ReadSec() const
{
    return started ? (SDL_GetTicks() - startTime) / 1000 : 0;
}

float Timer::ReadMSec() const
{
    return started ? (float)(SDL_GetTicks() - startTime) : 0.0f;
}

uint32 Timer::CountDown(int total) const
{
    if (started) {
        total = total - (SDL_GetTicks() - startTime) / 1000;
        if (total <= 0) {
            total = 0;
        }
        return total;
    }
    else {
        return total;
    }
}

bool Timer::IsStarted() const
{
    return started;
}