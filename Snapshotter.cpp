#include "Snapshotter.h"

Snapshotter::Snapshotter(NodeInfo& ni):
    mMap(   ni.n,
            ni.minPerActive,
            ni.maxPerActive,
            ni.maxNumber),
    rNode(ni.n),
    mParent(-1),
    mSnapshotDelay(ni.snapshotDelay)
{

}

Snapshotter::~Snapshotter()
{
}


void Snapshotter::handleMsg(std::string msg)
{

    //mMap.handleMsg(msg);
    auto splits = Utils::split(msg, APP_DELIM);

    if(splits.size() == 2)
    {
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
    else if(splits.size() == 3)
    {
        int uid = Utils::strToInt(splits[0]);
        handleAppMsg(uid);
        mMap.handleMsg(msg);
    }
}

void Snapshotter::startSnapshot()
{
    rNode.flood(getCtrlStr(MARKER));
    mChannelEmpty = true;

    for(int uid : rNode.getConnectedUids())
    {
        mRecordingMap[uid] = true;
    }
    
}

void Snapshotter::init()
{

    createTree();
    startSnapshot();
    mMap.init();
}

void Snapshotter::handleParent(int uid)
{
    if(mParent == -1)
    {
        mParent = uid;
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
    else
    {
        Utils::log("got mark from",uid);
    }

    //any mark means we have recieved a mark from that uid
    mRecordingMap[uid] = false;

    if(!anyRecording())
    {
        //Utils::log("got all marks! done with snapshot!");
        if(!mChannelEmpty || mMap.isActive())
        {
            Utils::log("protocol still active!");
        }

        Utils::log(mMap.getVectorClock());
    }
}

void Snapshotter::handleAppMsg(int uid)
{
    if(mRecordingMap[uid])
    {
        mChannelEmpty = false;
        Utils::log("message in channel!");
    }
}

bool Snapshotter::anyRecording()
{
    bool out = false;

    for(auto it : mRecordingMap)
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
    return std::to_string(rNode.getUid()) + APP_DELIM + std::to_string(ctrlMsgId);
}

void Snapshotter::snapshotTimer()
{
    while(mIncomplete)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(mSnapshotDelay));
        startSnapshot();
    }
}
