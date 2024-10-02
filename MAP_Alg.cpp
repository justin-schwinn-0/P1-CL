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

    auto connections = rNode.getConnectedUids();

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> msgNum(mMinPerActive,mMaxPerActive);
    std::uniform_int_distribution<std::mt19937::result_type> picker(0,connections.size());

    int numMsgs = msgNum(rng);
    while(numMsgs > 0)
    {
        int uid = picker(rng);
        rNode.sendTo(uid,"hello");
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        numMsgs--;
    }
    

}

void MAP_Alg::init()
{
    becomeActive();
}
