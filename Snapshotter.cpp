#include "Snapshotter.h"
#include <thread>

Snapshotter::Snapshotter(NodeInfo& ni):
    mMap(   ni.n,
            ni.minPerActive,
            ni.maxPerActive,
            ni.maxNumber,
            ni.minSendDelay),
    rNode(ni.n),
    mParent(-1),
    mSnapshotDelay(ni.snapshotDelay),
    mChildren(0)
{

}

Snapshotter::~Snapshotter()
{
}


void Snapshotter::handleMsg(std::string msg)
{
    Utils::log("===================================");
    Utils::log("                         ",msg);
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
            case CHILD:
                handleChild(uid);
                break;
            case REF:
                handleRef(uid);
                break;
            case MARKER:
                handleMarker(uid);
                break;
            case REPORT_ACT:
                handleReport(uid,true);
                break;
            case REPORT_PASS:
                handleReport(uid,false);
                break;
            case DONE:
                handleDone();
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
    else
    {
        Utils::log("something went wrong with the message");
    }

}

void Snapshotter::startSnapshot()
{
    //Utils::log("sending marks");
    rNode.flood(getCtrlStr(MARKER));
    mConvergesRemaining = mChildren + 1;
    mReportActive = false;

    for(int uid : rNode.getConnectedUids())
    {
        mRecordingMap[uid] = true;
    }

    mClockStorage.push_back(mMap.getVectorClock());
    
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void Snapshotter::init()
{
    createTree();
    Utils::log("create tree");
}

void Snapshotter::handleParent(int uid)
{
    if(mParent == -1 && rNode.getUid() != 0)
    {
        mParent = uid;
        rNode.sendExcept(uid,getCtrlStr(PARENT));
        mConvergesRemaining = rNode.getNeighborsSize() -1;
        Utils::log("parent is",mParent);
    }
    else
    {
        // send ref
        rNode.sendTo(uid,getCtrlStr(REF));
    }
}

void Snapshotter::handleChild(int uid)
{
    Utils::log("child is",uid);
    mChildren++;
    convergeForChild();
}

void Snapshotter::handleRef(int uid)
{
    Utils::log("ref is", uid);
    convergeForChild();
}

void Snapshotter::handleMarker(int uid)
{
    //Utils::log("got mark from",uid);
    if(!anyRecording())
    {
        startSnapshot();
    }

    mRecordingMap[uid] = false;

    if(!anyRecording())
    {
        //Utils::log("own converge");
        convergeForReport();
    }
}

void Snapshotter::handleReport(int uid,bool isActive)
{
    Utils::log("child",uid,"reports active:",isActive);
    if(isActive)
    {
        mReportActive = true;
    }

    convergeForReport();
}

void Snapshotter::handleAppMsg(int uid)
{
    if(mRecordingMap[uid])
    {
        mReportActive = true;
        //Utils::log("message in channel",uid,"!");
    }
}

void Snapshotter::convergeForChild()
{
    if(converge())
    {
        rNode.sendTo(mParent,getCtrlStr(CHILD));
        Utils::log("converged",mChildren);

        if(rNode.getUid() == 0)
        {
            initSnapshotProtocol();
            mMap.init();
        }
    }
}

void Snapshotter::convergeForReport()
{
    //Utils::log("try converge for report");

    bool contSnapshots = mReportActive || mMap.isActive();
    if(converge())
    {
        if(rNode.getUid() == 0 )
        {
            if(contSnapshots)
            {
                initSnapshotProtocol();
            }
            else
            {
                Utils::log("Protocol is done!");
                rNode.flood(getCtrlStr(DONE));
            }
        }
        else
        {
            if(contSnapshots)
            {
                Utils::log("protocol still active!");
                rNode.sendTo(mParent,getCtrlStr(REPORT_ACT));
                //Utils::log("sent msg  ");
            }
            else
            {
                Utils::log("Protocol is passive");
                rNode.sendTo(mParent,getCtrlStr(REPORT_PASS));
                //Utils::log("sent msg");
            }
        }

        Utils::log(mMap.getVectorClock());
    }
    //Utils::log("exit converge for report");
}

void Snapshotter::handleDone()
{
    Utils::log("Protocol done!");
    for(auto& str : mClockStorage)
    {
        Utils::log(str);
    }

    //write the file of log storage
}

bool Snapshotter::converge()
{
    mConvergesRemaining--;
    if(mConvergesRemaining == 0)
    {
        return true;
    }
    return false;
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
    std::this_thread::sleep_for(std::chrono::milliseconds(6000));
    rNode.flood(getCtrlStr(PARENT));
    mConvergesRemaining = rNode.getNeighborsSize();
}

std::string Snapshotter::getCtrlStr(int ctrlMsgId)
{
    return std::to_string(rNode.getUid()) + APP_DELIM + std::to_string(ctrlMsgId);
}

void Snapshotter::snapshotTimer()
{
    if(mIncomplete)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(mSnapshotDelay));
        startSnapshot();
    }
}

void Snapshotter::initSnapshotProtocol()
{
    std::thread timerThrd(&Snapshotter::snapshotTimer, this);
    timerThrd.detach();
}
