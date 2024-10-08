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
        it++;
        if(it != mClockMap.end())
        {
            out += "||";
        }
    }while (it != mClockMap.end());

    return out;
}


void VectorClock::update(int sender, std::string str)
{
    auto splits = Utils::split(str,"||");

    for(auto pairStr : splits)
    {
        auto splitPair = Utils::split(pairStr,",");

        int uid = Utils::strToInt(splitPair[0]);
        int val = Utils::strToInt(splitPair[1]) + (uid == sender);

        mClockMap[uid] = std::max(val, mClockMap[uid]);
    }
}
