#include "MAP_Alg.h"

#include <random>
#include <thread>

MAP_Alg::MAP_Alg(Node& n,int minActive, int maxActive,int maxNum,int delay):
    rNode(n),
    mMinPerActive(minActive),
    mMaxPerActive(maxActive),
    mMaxNum(maxNum),
    mDelay(delay),
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

    auto splitMsg = Utils::split(str,APP_DELIM);

    int otherUid =Utils::strToInt(splitMsg[0]);
    std::string strVc =splitMsg[1];
    std::string msg =splitMsg[2];


    mVc.update(otherUid,strVc);

    int mid = Utils::strToInt(msg);
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
    //Utils::log("Active! sent:",mMsgsSent, "Max:",mMaxNum);
    int msgsToSend;

    auto connections = rNode.getConnectedUids();

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> msgNum(mMinPerActive,mMaxPerActive);
    std::uniform_int_distribution<std::mt19937::result_type> picker(0,connections.size()-1);

    int numMsgs = msgNum(rng);
    numMsgs = std::min(numMsgs, mMaxNum-mMsgsSent);
    //Utils::log("num sent:",numMsgs);
    while(numMsgs > 0)
    {
        int uidIndex = picker(rng);
        int uid = connections[uidIndex];
        std::this_thread::sleep_for(std::chrono::milliseconds(mDelay));
        sendProtocolMessage(uid,std::to_string(ACTIVE));
        numMsgs--;
        mMsgsSent++;
        mVc.increment();
    }
    //Utils::log("Passive! sent:",mMsgsSent, "Max:",mMaxNum);
    //Utils::log("Passive! VC:",mVc.to_string());
    mActive = false;
}

bool MAP_Alg::isActive()
{
    return mActive;
}

std::string MAP_Alg::getVectorClock()
{
    return mVc.to_string();
}

void MAP_Alg::sendProtocolMessage(int uid,std::string str)
{
    std::string msg = std::to_string(rNode.getUid()) + APP_DELIM + mVc.to_string() + APP_DELIM + str;
    rNode.sendTo(uid,msg);
}

void MAP_Alg::init()
{
    becomeActive();
}
