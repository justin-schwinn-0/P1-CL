#ifndef SNAPSHOTTER_H
#define SNAPSHOTTER_H

#include <map>
#include <string>

#include "Node.h"
#include "Utils.h"
#include "MAP_Alg.h"

class Snapshotter
{
public:
    Snapshotter(NodeInfo& ni);
    ~Snapshotter();

    void handleMsg(std::string msg);

    void startSnapshot();

    void init();

private:
    MAP_Alg map;
    Node& rNode;

    bool mActive = false;

    std::map<int,bool> mRxMarkMap;
};

#endif

