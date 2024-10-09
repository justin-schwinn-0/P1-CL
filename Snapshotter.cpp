#include "Snapshotter.h"

Snapshotter::Snapshotter(NodeInfo& ni):
    mMap(   ni.n,
            ni.minPerActive,
            ni.maxPerActive,
            ni.maxNumber),
    rNode(ni.n),
    mParent(-1)
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
        case MARKER:
            handleMarker(uid);
            break;
        default:
            Utils::log("Unknown message!",msgId);
            break;
    }
}

void Snapshotter::startSnapshot()
{
    rNode.flood(getCtrlStr(MARKER));

    for(int uid : rNode.getConnectedUids())
    {
        mRxMarkMap[uid] = true;
    }
    
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
        rNode.sendExcept(uid,getCtrlStr(PARENT));
        Utils::log("parent is",mParent);
    }
}

void Snapshotter::handleMarker(int uid)
{
    if(!anyRecording())
    {
        startSnapshot();
    }
}

bool Snapshotter::anyRecording()
{
    bool out = false;

    for(auto it : mRxMarkMap)
    {
        out = it.second || out;
    }

    return out;
}

void Snapshotter::createTree()
{
    rNode.flood(getCtrlStr(PARENT));
}
std::string Snapshotter::getCtrlStr(int ctrlMsgId)
{
    return std::to_string(rNode.getUid()) + CTRL_DELIM + std::to_string(ctrlMsgId);
}
