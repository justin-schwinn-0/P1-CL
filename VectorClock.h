#ifndef VECTOR_CLOCK_H
#define VECTOR_CLOCK_H

#include <vector>
#include <string>


class VectorClock
{

public:
    VectorClock(std::vector<int> uids,int session);
    ~VectorClock();

    void rxVectorClock(std::string clk);

    void increment();

    void compare(int session);

    std::string to_string();

private:

    int mSession;

    std::vector<std::pair<int,int>> mClockMap;
};

#endif
