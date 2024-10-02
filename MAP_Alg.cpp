#include "MAP_Alg.h"

#include <random>
#include <thread>

MAP_Alg::MAP_Alg(Node& n,int minActive, int maxActive,int maxNum):
    rNode(n),
    mMinPerActive(minActive),
    mMaxPerActive(maxActive),
    mMaxNum(maxNum),
    mMsgsSent(0)
{
}

MAP_Alg::~MAP_Alg()
{
}

void MAP_Alg::handleMsg(std::string str)
{
    Utils::log(str);

    int mid = Utils::strToInt(str);
    if(mid == ACTIVE)
    {
        becomeActive();
    }
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

    auto connections = rNode.getConnectedUids();

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> msgNum(mMinPerActive,mMaxPerActive);
    std::uniform_int_distribution<std::mt19937::result_type> picker(0,connections.size()-1);

    int numMsgs = msgNum(rng);
    numMsgs = std::min(numMsgs, mMaxNum-mMsgsSent);
    Utils::log("num sent:",numMsgs);
    while(numMsgs > 0)
    {
        int uidIndex = picker(rng);
        int uid = connections[uidIndex];
        Utils::log("send to",uid);
        rNode.sendTo(uid,std::to_string(ACTIVE));
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        numMsgs--;
        mMsgsSent++;
    }
    mActive = false;
}

void MAP_Alg::init()
{
    becomeActive();
}
