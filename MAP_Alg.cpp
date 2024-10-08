#include "MAP_Alg.h"

#include <random>
#include <thread>

MAP_Alg::MAP_Alg(Node& n,int minActive, int maxActive,int maxNum):
    rNode(n),
    mMinPerActive(minActive),
    mMaxPerActive(maxActive),
    mMaxNum(maxNum),
    mMsgsSent(0),
    mVc({0,1,2,3,4},rNode.getUid())
{
}

MAP_Alg::~MAP_Alg()
{
}

void MAP_Alg::handleMsg(std::string str)
{
    //Utils::log(str);

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
        std::thread activeThrd(&MAP_Alg::active, this);
        activeThrd.detach();
    }

}

void MAP_Alg::active()
{
    mActive = true;
    Utils::log("Active! sent:",mMsgsSent, "Max:",mMaxNum);
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
        rNode.sendTo(uid,std::to_string(ACTIVE));
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        numMsgs--;
        mMsgsSent++;
        mVc.increment();
    }
    Utils::log("Passive! sent:",mMsgsSent, "Max:",mMaxNum);
    Utils::log("VC:",mVc.to_string());
    mActive = false;
}

void MAP_Alg::init()
{
    becomeActive();
}
