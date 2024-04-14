#ifndef __TIMER_H__
#define __TIMER_H__

#include "Defs.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	uint32 ReadSec() const;
	float ReadMSec() const;

	uint32 CountDown(int total) const;

private:
	uint32 startTime;
};

#endif //__TIMER_H__