#ifndef SNAPSHOTTER_H
#define SNAPSHOTTER_H

#include <map>
#include <string>

class Snapshotter
{
public:
    Snapshotter();
    ~Snapshotter();

    void handleMsg(std::string msg);

    void startSnapshot();

private:
    Node& rNode;

    bool mActive = false;

    std::map<int,bool> mRxMarkMap;
};

#endif

