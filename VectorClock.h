#ifndef VECTOR_CLOCK_H
#define VECTOR_CLOCK_H

#include <map>
#include <vector>
#include <string>


class VectorClock
{

public:
    VectorClock(int session);
    ~VectorClock();

    void rxVectorClock(std::string clk);

    void increment();

    void compare(int session);

    std::string to_string();

private:

    int mSession;

    std::map<int,int> mClockMap;
};

#endif
