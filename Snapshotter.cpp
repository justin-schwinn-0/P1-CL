#include "Snapshotter.h"

Snapshotter::Snapshotter(NodeInfo& ni):
    mMap(   ni.n,
            ni.minPerActive,
            ni.maxPerActive,
            ni.maxNumber),
    rNode(ni.n)
{

}

Snapshotter::~Snapshotter()
{
}


void Snapshotter::handleMsg(std::string msg)
{

    //mMap.handleMsg(msg);
    auto splits = Utils::split(msg, CTRL_DELIM);

    int uid = Utils::strToInt(splits[0]);
    int msgId = Utils::strToInt(splits[1]); 

    switch(msgId)
    {
        case PARENT:
            handleParent(uid);
            break;
        default:
            Utils::log("Unknown message!",msgId);
            break;
    }
}

void Snapshotter::startSnapshot()
{
    
}

void Snapshotter::init()
{
    //mMap.init();

    createTree();
}

void Snapshotter::handleParent(int uid)
{
    if(mParent == -1)
    {
        mParent = uid;
        // send child ack to parent
        rNode.sendExcept(uid,getParentStr());
    }
    else
    {
        // send ref ack to parent

    }
}

void Snapshotter::createTree()
{
    rNode.flood(getParentStr());
}
std::string Snapshotter::getParentStr()
{
    return std::to_string(rNode.getUid()) + CTRL_DELIM + std::to_string(PARENT);
}
