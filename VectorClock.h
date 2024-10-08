#ifndef VECTOR_CLOCK_H
#define VECTOR_CLOCK_H

#include <vector>
#include <string>
#include <map>

const std::string VC_DELIM = "|"

class VectorClock
{

public:
    VectorClock(std::vector<int> uids,int session);
    ~VectorClock();

    void rxVectorClock(std::string clk);

    void increment();

    void compare(int session);

    std::string to_string();

    void update(int sender, std::string str);

private:

    int mSession;

    std::map<int,int> mClockMap;
};

#endif
