#ifndef VECTOR_CLOCK_H
#define VECTOR_CLOCK_H

#include <vector>
#include <string>
#include <map>

const std::string VC_DELIM = " ";

class VectorClock
{

public:
    VectorClock(std::vector<int> uids,int session);
    VectorClock(std::string str);
    ~VectorClock();

    void rxVectorClock(std::string clk);

    void increment();

    void hasHappenBefore(VectorClock vc);

    std::string to_string();

    void update(int sender, std::string str);

    std::map<int,int> getClocks()
    { return mClockMap; }

private:

    int mSession;

    std::map<int,int> mClockMap;
};

#endif
