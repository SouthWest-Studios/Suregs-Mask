#ifndef __TIMER_H__
#define __TIMER_H__

#include "Defs.h"

class Timer
{
public:

    Timer();

    void Start(uint32 startingMS = 0);
    void Stop();
    uint32 ReadSec() const;
    float ReadMSec() const;

    uint32 CountDown(int total) const;
    bool IsStarted() const;

private:
    uint32 startTime;
    bool started; 
};

#endif //__TIMER_H__