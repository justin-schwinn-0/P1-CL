#ifndef SNAPSHOTTER_H
#define SNAPSHOTTER_H

#include <map>
#include <string>

#include "Node.h"
#include "Utils.h"
#include "MAP_Alg.h"

const int MARKER = 1000;
const int PARENT = 1001;
const int REPORT = 1002;


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
    void handleMarker(int uid);
    void handleAppMsg(int uid);
    bool anyRecording();

private:
    MAP_Alg mMap;
    Node& rNode;

    bool mChannelEmpty;

    std::map<int,bool> mWaitingMap;

    int mParent;
};

#endif

