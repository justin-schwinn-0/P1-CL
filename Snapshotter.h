#ifndef SNAPSHOTTER_H
#define SNAPSHOTTER_H

#include <map>
#include <string>

#include "Node.h"
#include "Utils.h"
#include "MAP_Alg.h"

const int MARKER = 1000;
const int PARENT = 1001;
const int CHILD = 1002;
const int REF = 1003;


class Snapshotter
{
public:
    Snapshotter(NodeInfo& ni);
    ~Snapshotter();

    void handleMsg(std::string msg);

    void startSnapshot();

    void init();

    std::string getCtrlStr(int ctrlMsgId);

    void createTree();

    void handleParent(int uid);
    void handleChild(int uid);
    void handleRef(int uid);
    void handleMarker(int uid);
    void handleAppMsg(int uid);
    bool converge();
    void convergeForChild();
    bool anyRecording();
    void snapshotTimer();

private:
    MAP_Alg mMap;
    Node& rNode;

    int mConvergesRemaining = -1;

    bool mChannelEmpty;

    std::map<int,bool> mRecordingMap;

    int mParent;
    int mSnapshotDelay = 10000;
    bool mIncomplete = true;

    int mChildren;
};

#endif

