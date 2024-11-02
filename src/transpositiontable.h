
#ifndef TRANSPOSITIONTABLE_H
#define TRANSPOSITIONTABLE_H

#include "types.h"

#include <cstdint>

enum BoundType : uint8_t { EXACT, UPPER_BOUND, LOWER_BOUND };

constexpr int NO_EVAL = 0x7fffffff;

constexpr int TT_SIZE = 1 << 24; // 256 MiB
constexpr int RT_SIZE = 1 << 15; // 512 KiB

struct Entry
{
    uint64_t  key;
    int       eval;
    uint16_t  best_move;
    uint8_t   depth;
    BoundType flag;
};

struct RepInfo
{
    uint64_t key;
    uint8_t  occurrences;
};

namespace TranspositionTable
{
    int lookup(int depth, int alpha, int beta, int ply_from_root);
    void record(uint8_t depth, BoundType flag, int eval, Move best_move, int ply_from_root);
    Move lookup_move();
    void clear();
}

namespace RepetitionTable
{
    bool draw();
    void push();
    void pop();
    void clear();
}

#endif
