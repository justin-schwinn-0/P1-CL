#include "VectorClock.h"

VectorClock::VectorClock(std::vector<int> uids,int session):
    mSession(session)
{
    for(int uid : uids)
    {
        mClockMap[uid] = 0;
    }
}

VectorClock::~VectorClock()
{
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
    std::string out ="";

    auto it = mClockMap.begin();
    do
    {
        out += std::to_string(it->first) + "," + 
                std::to_string(it->second);
        if((it+1) != mClockMap.end())
        {
            out += "||";
        }
        it++;
    }while (it != mClockMap.end());

    return "()";
}
