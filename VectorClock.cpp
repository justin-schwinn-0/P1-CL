#include "VectorClock.h"

#include "Utils.h"

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
        out += std::to_string(it->second);
        it++;
        if(it != mClockMap.end())
        {
            out += VC_DELIM;
        }
    }while (it != mClockMap.end());

    return out;
}


void VectorClock::update(int sender, std::string str)
{
    auto splits = Utils::split(str,VC_DELIM);

    //Utils::log("m",to_string());
    for(int i = 0; i < splits.size() ; i++)
    {
        mClockMap[i] = Utils::strToint(splits[i]);
    }
    //Utils::log("o",str);
}
