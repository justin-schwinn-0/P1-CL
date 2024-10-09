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
    Utils::log("tunnel through snapshotter");

    mMap.handleMsg(msg);
}

void Snapshotter::startSnapshot()
{
}

void init()
{
    mMap.init();
}
