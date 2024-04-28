
#include "movegen.h"
#include "uci.h"
#include "ui.h"
#include "search.h"
#include "transpositiontable.h"

#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

// 4q2r/2pk4/Q1n1bp1p/6p1/8/2NP4/PrPB1PPP/R5K1 w - - 0 1

extern RepInfo rep_table[];
extern int reductions[MAX_PLY][MAX_PLY];

int reduction_(int d, int mn) {
  return std::log(mn + 2) * std::log(std::min(14, d) + 2);
}

namespace Debug {

  void go() {
    for (int depth = 0; depth < 15; depth++) {
      for (int mn = 0; mn < MAX_PLY; mn++) {
        if (mn%10 == 0)
          std::cout << "\n";
        std::cout << std::setw(4) << (std::to_string(reduction_(depth, mn)) + ",");
      }
      std::cout << "\n\n";
    }

    for (int depth = 0; depth < MAX_PLY; depth++) {
      for (int mn = 0; mn < MAX_PLY; mn++) {
        if (mn % 10 == 0)
          std::cout << "\n";
        std::cout << std::setw(4) << std::to_string(int(std::log(mn + 2) / std::log(std::min(14, depth) + 2))) + ",";
      }
      std::cout << "\n\n";
    }
  }

}

std::string brd() {

  std::string board = Position::to_string();

  for (char& c : board)
    if (c == '\n')
      c = '?';

  return board;
}

void gameinfo() {

  if (RepetitionTable::has_repeated()) {
    std::cout << "draw\n";
    return;
  }

  int count = 0;
  for (int i = 0; i < (1 << 20); i++) {
    if (rep_table[i].occurrences)
      count++;
    if (count >= 100) {
      std::cout << "draw\n";
      return;
    }
  }

  if (Position::white_to_move()) {
    MoveList<WHITE> moves;
    if (moves.size())
      std::cout << "nonterminal\n";
    else if (moves.in_check())
      std::cout << "mate\n";
    else
      std::cout << "draw\n";
  } else {
    MoveList<BLACK> moves;
    if (moves.size())
      std::cout << "nonterminal\n";
    else if (moves.in_check())
      std::cout << "mate\n";
    else
      std::cout << "draw\n";
  }
}
