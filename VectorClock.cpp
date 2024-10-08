#include "VectorClock.h"

VectorClock::VectorClock(std::vector<int> uids,int session):
    mSession(session)
{
    for(int uid : uids)
    {
        mClockMap[uid] = 0;
    }
}

void VectorClock::increment()
{
    mClockMap[mSession]++;
}

void VectorClock::compare(int session)
{
    // do the compare
}

std::string VectorClock::to_string()
{
    return "()";
}
