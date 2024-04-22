
#ifndef TRANSPOSITIONTABLE_H
#define TRANSPOSITIONTABLE_H

#include <cstdint>

//   entries    ~Megabytes
// 
//   4194304    67
//   16777216   268

namespace RepetitionTable {
  bool has_repeated();
  void increment();
  void decrement();
  void clear();
}

struct RepInfo {
  uint64_t key;
  uint8_t occurrences;
};

using HashFlag = uint8_t;

enum HashFlags {
  EXACT,
  UPPER_BOUND,
  LOWER_BOUND
};

constexpr int FAIL = 0x7fffffff;

struct Entry {
  uint64_t key;
  uint8_t depth;
  HashFlag flag;
  int eval;
  uint16_t best_move;
  void set(uint64_t k, uint8_t d, HashFlag f, int e,  uint16_t b) {
    key = k; depth = d; flag = f; eval = e; best_move = b;
  }
};

namespace TranspositionTable {

  int lookup(int depth, int alpha, int beta, int ply_from_root);
  void record(uint8_t depth, HashFlag flag, int eval, uint16_t best_move, int ply_from_root);
  int lookup_move();
  void clear();

}

#endif
