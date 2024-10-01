#include "MAP_Alg.h"


MAP_Alg::MAP_Alg(Node& n,int minActive, int maxActive,int maxNum):
    rNode(n),
    mMinPerActive(minActive),
    mMaxPerActive(maxActive),
    mMaxNumber(maxNum)
{
}

MAP_Alg::~MAP_Alg()
{
}

void MAP_Alg::handleMsg(std::string str)
{

}

void MAP_Alg::init()
{
    Utils::log("init here");
}
