#ifndef MAP_PROTOCOL_H
#define MAP_PROTOCOL_H

#include <vector>
#include <string>
#include <functional>

#include "Node.h"
#include "Utils.h"


class MAP_Alg
{
public:
    MAP_Alg(Node& n,int minActive, int maxActive,int maxNum);
    ~MAP_Alg();

    void init();

    void handleMsg(std::string str);

private:
    Node& rNode;

    bool mActive = false;
    int mMinPerActive;
    int mMaxPerActive;

    int mMaxNum;



};

#endif
