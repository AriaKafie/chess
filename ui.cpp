
#include "ui.h"
#include "position.h"
#include "util.h"

#include "search.h"
#include <string>
#include <iostream>

namespace UI {

  std::string move_to_string(Move move) {

    if (type_of(move) == SHORTCASTLE) return "O-O";
    if (type_of(move) == LONGCASTLE ) return "O-O-O";
                
    Square to = to_sq(move);

    return piece_to_char[piece_on(to)] + coords[to];

  }

  int movestring_to_int(std::string move) {
                
    if (move.length() < 4) return -1;
    int from = 0;
    int to = 0;
                
    for (int square = 0; square < 64; square++) {
      if (coords[square] == move.substr(0,2))
        from = square;
      if (coords[square] == move.substr(2,4))
        to = square;
    }
                
    int to_int = from + (to << 6);
                
    if (((board[from] == W_PAWN) || (board[from] == B_PAWN)) && ((std::abs(to - from) % 2) != 0) && (board[to] == NO_PIECE))
      to_int += ENPASSANT;
    if ((board[from] == W_PAWN && to > 55) || (board[from] == B_PAWN && to < 8))
      to_int += PROMOTION;
    if (move == "scastle") to_int = Position::white_to_move() ? W_SCASTLE : B_SCASTLE;
    if (move == "lcastle") to_int = Position::white_to_move() ? W_LCASTLE : B_LCASTLE;
                
    if (Position::white_to_move()) {
      MoveList<WHITE> moves;
      for (Move m : moves)
        if (m == to_int) return to_int;
      return -1;
    }
    else {
      MoveList<BLACK> moves;
      for (Move m : moves)
        if (m == to_int) return to_int;
      return -1;
    }

  }

  void move_prompt() {

    std::string move;
    std::cout << "enter a move:\n";
    std::cin >> move;
    int to_int = movestring_to_int(move);
    while (to_int == -1) {
      std::cout << "invalid\n";
      std::cin >> move;
      to_int = movestring_to_int(move);
    }
    Position::do_commit(to_int);

  }

  void clear_line() {

    for (int i = 0; i < 20; i++) std::cout << "\b";

  }

}
