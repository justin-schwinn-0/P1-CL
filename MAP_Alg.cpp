#include "MAP_Alg.h"

#include <thread>

MAP_Alg::MAP_Alg(Node& n,int minActive, int maxActive,int maxNum):
    rNode(n),
    mMinPerActive(minActive),
    mMaxPerActive(maxActive),
    mMaxNum(maxNum)
{
}

MAP_Alg::~MAP_Alg()
{
}

void MAP_Alg::handleMsg(std::string str)
{
    Utils::log(str);
}

void MAP_Alg::becomeActive()
{
    if(!mActive)
    {
        std::thread activeThrd(&MAP_Alg::active, this);
        activeThrd.detach();
    }

}

void MAP_Alg::active()
{
    rNode.flood("hello");
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    rNode.flood("hello");
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    rNode.flood("hello");
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    rNode.flood("hello");
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
}

void MAP_Alg::init()
{
    Utils::log("init here");
}
