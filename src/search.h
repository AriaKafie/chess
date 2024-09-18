
#ifndef SEARCH_H
#define SEARCH_H

#include <chrono>
#include <iostream>
#include <string>

#include "types.h"

typedef int NodeType;

enum { ROOT, NONROOT };

inline constexpr int root_reductions[MAX_PLY] =
{
    0,1,1,1,1,2,2,2,2,2,
    2,2,2,2,2,3,3,3,3,3,
    3,3,3,3,3,3,3,3,3,3,
    3,3,3,3,3,3,3,3,3,3,
    4,4,4,4,4,4,4,4,4,4,
    5,5,5,5,5,5,5,5,5,5,
    6,6,6,6,6,6,6,6,6,6,
    7,7,7,7,7,7,7,7,7,7,
    8,8,8,8,8,8,8,8,8,8,
};

inline unsigned long long curr_time_millis() {
    return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
}

constexpr int matescore = 100000;

namespace Search {

void init();
void go(uint64_t thinktime = 0);
void count_nodes(int depth);

} // namespace Search

inline bool search_cancelled;

inline void start_timer(uint64_t thinktime) {

    auto start_time = curr_time_millis();

    while (true)
    {
        if (curr_time_millis() - start_time > thinktime)
        {
            search_cancelled = true;
            return;
        }
    }
}

inline void await_stop() {

    std::string in;

    do
    {
        std::getline(std::cin, in);
    } while (in != "stop");

    search_cancelled = true;
}

#endif
