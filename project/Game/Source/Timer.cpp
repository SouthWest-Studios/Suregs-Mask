// ----------------------------------------------------
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "Timer.h"
#include "SDL\include\SDL_timer.h"

// L1: DONE 1: Fill Start(), Read(), ReadSec() methods
// they are simple, one line each!
	
Timer::Timer()
{
	Start();
}

void Timer::Start(uint32 startingMS)
{
	startTime = SDL_GetTicks() - startingMS;
}

uint32 Timer::ReadSec() const
{
	return (SDL_GetTicks() - startTime) / 1000;
}

float Timer::ReadMSec() const
{
	return (float)(SDL_GetTicks() - startTime);
}

uint32 Timer::CountDown(int total) const
{
	if (total == 0) {
		total = 0;
	}
	return total - (SDL_GetTicks() - startTime) / 1000;
}