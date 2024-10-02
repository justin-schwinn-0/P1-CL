#include "MAP_Alg.h"

#include <random>
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
    if(!mActive && mMsgsSent < mMaxNum )
    {
        Utils::log("Active! sent:",mMsgsSent, "Max:",mMaxNum);
        std::thread activeThrd(&MAP_Alg::active, this);
        activeThrd.detach();
    }

}

void MAP_Alg::active()
{
    int msgsToSend;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,rNode.getConnectedUids().size());

    rNode.flood("hello");
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
}

void MAP_Alg::init()
{
    becomeActive();
}
