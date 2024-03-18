
#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "movelist.h"
#include "bitboard.h"
#include "gamestate.h"
#include "board.h"

ForceInline inline Move* make_moves(Move* movelist, Square from, Bitboard to) {
  for (;to; pop_lsb(to))
    *movelist++ = make_move(from, lsb(to));
  return movelist;
}

template<Direction D>
ForceInline Move* make_pawn_moves(Move* movelist, Bitboard attacks) {
  for (;attacks; pop_lsb(attacks)) {
    Square to = lsb(attacks);
    *movelist++ = make_move(to - D, to);
  }
  return movelist;
}

template<Color Us>
CaptureList<Us>::CaptureList() :
  last(moves)
{
  constexpr Color Them           = !Us;
  constexpr Piece EnemyPawn      = make_piece(Them, PAWN);
  constexpr Piece EnemyKnight    = make_piece(Them, KNIGHT);
  constexpr Piece EnemyBishop    = make_piece(Them, BISHOP);
  constexpr Piece EnemyRook      = make_piece(Them, ROOK);
  constexpr Piece EnemyQueen     = make_piece(Them, QUEEN);
  constexpr Piece EnemyKing      = make_piece(Them, KING);
  constexpr Piece FriendlyPawn   = make_piece(Us,   PAWN);
  constexpr Piece FriendlyKnight = make_piece(Us,   KNIGHT);
  constexpr Piece FriendlyBishop = make_piece(Us,   BISHOP);
  constexpr Piece FriendlyRook   = make_piece(Us,   ROOK);
  constexpr Piece FriendlyQueen  = make_piece(Us,   QUEEN);
  constexpr Piece FriendlyKing   = make_piece(Us,   KING);

  Bitboard enemy_rook_queen   = bb(EnemyQueen) | bb(EnemyRook);
  Bitboard enemy_bishop_queen = bb(EnemyQueen) | bb(EnemyBishop);
  Square   ksq                = lsb(bb(FriendlyKing));
  Bitboard occupied           = occupied_bb() ^ square_bb(ksq);

  seen_by_enemy = PawnAttacks<Them>(bb(EnemyPawn)) | KingAttacks(lsb(bb(EnemyKing)));
  for (Bitboard b = bb(EnemyKnight); b; pop_lsb(b))
    seen_by_enemy |= KnightAttacks(lsb(b));
  for (Bitboard b = enemy_bishop_queen; b; pop_lsb(b))
    seen_by_enemy |= BishopAttacks(lsb(b), occupied);
  for (Bitboard b = enemy_rook_queen; b; pop_lsb(b))
    seen_by_enemy |= RookAttacks(lsb(b), occupied);

  toggle_square(occupied, ksq);

  Bitboard enemy_unprotected = bb(Them) &~ seen_by_enemy;

  checkmask = KnightAttacks(ksq) & bb(EnemyKnight) | PawnAttacks<Us>(ksq) & bb(EnemyPawn);
  for (Bitboard checkers = BishopAttacks(ksq, occupied) & enemy_bishop_queen | RookAttacks(ksq, occupied) & enemy_rook_queen; checkers; pop_lsb(checkers))
    checkmask |= CheckRay(ksq, lsb(checkers));
  if (more_than_one(checkmask & DoubleCheck(ksq))) {
    last = make_moves(last, ksq, KingAttacks(ksq) & enemy_unprotected);
    return;
  }
  if (checkmask == 0) checkmask = ALL_SQUARES;

  Bitboard pinned = 0;
  for (Bitboard pinners = BishopXray(ksq, occupied) & enemy_bishop_queen | RookXray(ksq, occupied) & enemy_rook_queen; pinners; pop_lsb(pinners))
    pinned |= CheckRay(ksq, lsb(pinners));

  constexpr Direction UpRight   = Us == WHITE ? NORTH_EAST : SOUTH_WEST;
  constexpr Direction UpLeft    = Us == WHITE ? NORTH_WEST : SOUTH_EAST;
  constexpr Bitboard  Start     = Us == WHITE ? RANK_2     : RANK_7;
  constexpr Bitboard  Promote   = Us == WHITE ? RANK_7     : RANK_2;
  constexpr Bitboard  NoPromote = ~Promote;

  Bitboard not_pinned = ~pinned;

  if (Bitboard promotable = bb(FriendlyPawn) & Promote)
  {
    for (Bitboard b = shift<UpRight>(promotable & (not_pinned | FDiag(ksq))) & bb(Them) & checkmask; b; pop_lsb(b)) {
      Square to = lsb(b);
      *last++ = make_move<PROMOTION>(to - UpRight, to);
    }
    for (Bitboard b = shift<UpLeft >(promotable & (not_pinned | BDiag(ksq))) & bb(Them) & checkmask; b; pop_lsb(b)) {
      Square to = lsb(b);
      *last++ = make_move<PROMOTION>(to - UpLeft, to);
    }
  }

  Bitboard pawns = bb(FriendlyPawn) & NoPromote;

  last = make_pawn_moves<UpRight>(last, shift<UpRight>(pawns & (not_pinned | FDiag(ksq))) & bb(Them) & checkmask);
  last = make_pawn_moves<UpLeft >(last, shift<UpLeft >(pawns & (not_pinned | BDiag(ksq))) & bb(Them) & checkmask);

  Bitboard minor_targets = (bb(Them) ^ bb(EnemyPawn) | enemy_unprotected) & checkmask;
  Bitboard rook_targets  = (bb(EnemyRook) | bb(EnemyQueen) | enemy_unprotected) & checkmask;
  Bitboard queen_targets = (bb(EnemyQueen) | enemy_unprotected) & checkmask;

  for (Bitboard b = bb(FriendlyKnight) & not_pinned; b; pop_lsb(b)) {
    Square from = lsb(b);
    last = make_moves(last, from, KnightAttacks(from) & minor_targets);
  }
  for (Bitboard b = bb(FriendlyBishop) & not_pinned; b; pop_lsb(b)) {
    Square from = lsb(b);
    last = make_moves(last, from, BishopAttacks(from, occupied) & minor_targets);
  }
  for (Bitboard b = bb(FriendlyBishop) & pinned; b; pop_lsb(b)) {
    Square from = lsb(b);
    last = make_moves(last, from, BishopAttacks(from, occupied) & minor_targets & PinMask(ksq, from));
  }
  for (Bitboard b = bb(FriendlyRook) & not_pinned; b; pop_lsb(b)) {
    Square from = lsb(b);
    last = make_moves(last, from, RookAttacks(from, occupied) & rook_targets);
  }
  for (Bitboard b = bb(FriendlyRook) & pinned; b; pop_lsb(b)) {
    Square from = lsb(b);
    last = make_moves(last, from, RookAttacks(from, occupied) & rook_targets & PinMask(ksq, from));
  }
  for (Bitboard b = bb(FriendlyQueen) & not_pinned; b; pop_lsb(b)) {
    Square from = lsb(b);
    last = make_moves(last, from, QueenAttacks(from, occupied) & queen_targets);
  }
  for (Bitboard b = bb(FriendlyQueen) & pinned; b; pop_lsb(b)) {
    Square from = lsb(b);
    last = make_moves(last, from, QueenAttacks(from, occupied) & queen_targets & PinMask(ksq, from));
  }

  last = make_moves(last, ksq, KingAttacks(ksq) & enemy_unprotected);

}

template<Color Us>
MoveList<Us>::MoveList(bool ep_enabled) :
  last(moves)
{
  constexpr Color Them           = !Us;
  constexpr Piece EnemyPawn      = make_piece(Them, PAWN);
  constexpr Piece EnemyKnight    = make_piece(Them, KNIGHT);
  constexpr Piece EnemyBishop    = make_piece(Them, BISHOP);
  constexpr Piece EnemyRook      = make_piece(Them, ROOK);
  constexpr Piece EnemyQueen     = make_piece(Them, QUEEN);
  constexpr Piece EnemyKing      = make_piece(Them, KING);
  constexpr Piece FriendlyPawn   = make_piece(Us,   PAWN);
  constexpr Piece FriendlyKnight = make_piece(Us,   KNIGHT);
  constexpr Piece FriendlyBishop = make_piece(Us,   BISHOP);
  constexpr Piece FriendlyRook   = make_piece(Us,   ROOK);
  constexpr Piece FriendlyQueen  = make_piece(Us,   QUEEN);
  constexpr Piece FriendlyKing   = make_piece(Us,   KING);

  Bitboard enemy_rook_queen   = bb(EnemyQueen) | bb(EnemyRook);
  Bitboard enemy_bishop_queen = bb(EnemyQueen) | bb(EnemyBishop);
  Square   ksq                = lsb(bb(FriendlyKing));
  Bitboard occupied           = occupied_bb() ^ square_bb(ksq);

  seen_by_enemy = PawnAttacks<Them>(bb(EnemyPawn)) | KingAttacks(lsb(bb(EnemyKing)));
  for (Bitboard b = bb(EnemyKnight); b; pop_lsb(b))
    seen_by_enemy |= KnightAttacks(lsb(b));
  for (Bitboard b = enemy_bishop_queen; b; pop_lsb(b))
    seen_by_enemy |= BishopAttacks(lsb(b), occupied);
  for (Bitboard b = enemy_rook_queen; b; pop_lsb(b))
    seen_by_enemy |= RookAttacks(lsb(b), occupied);

  toggle_square(occupied, ksq);

  checkmask = KnightAttacks(ksq) & bb(EnemyKnight) | PawnAttacks<Us>(ksq) & bb(EnemyPawn);
  for (Bitboard checkers = BishopAttacks(ksq, occupied) & enemy_bishop_queen | RookAttacks(ksq, occupied) & enemy_rook_queen; checkers; pop_lsb(checkers))
    checkmask |= CheckRay(ksq, lsb(checkers));
  if (more_than_one(checkmask & DoubleCheck(ksq))) {
    last = make_moves(last, ksq, KingAttacks(ksq) & ~(seen_by_enemy | bb(Us)));
    return;
  }
  if (checkmask == 0) checkmask = ALL_SQUARES;

  Bitboard pinned = 0;
  for (Bitboard pinners = BishopXray(ksq, occupied) & enemy_bishop_queen | RookXray(ksq, occupied) & enemy_rook_queen; pinners; pop_lsb(pinners))
    pinned |= CheckRay(ksq, lsb(pinners));

  constexpr Direction Up        = Us == WHITE ? NORTH      : SOUTH;
  constexpr Direction Up2       = Us == WHITE ? NORTHNORTH : SOUTHSOUTH;
  constexpr Direction UpRight   = Us == WHITE ? NORTH_EAST : SOUTH_WEST;
  constexpr Direction UpLeft    = Us == WHITE ? NORTH_WEST : SOUTH_EAST;
  constexpr Bitboard  FriendEP  = Us == WHITE ? RANK_3     : RANK_6;
  constexpr Bitboard  EnemyEP   = Us == WHITE ? RANK_6     : RANK_3;
  constexpr Bitboard  Start     = Us == WHITE ? RANK_2     : RANK_7;
  constexpr Bitboard  Promote   = Us == WHITE ? RANK_7     : RANK_2;
  constexpr Bitboard  NoPromote = ~Promote;

  Bitboard empty      = ~occupied;
  Bitboard not_pinned = ~pinned;
  Bitboard pawns      = bb(FriendlyPawn) & NoPromote;
  Bitboard e          = shift<Up>(FriendEP & empty) & empty;

  last = make_pawn_moves<UpRight>(last, shift<UpRight>(pawns & (not_pinned | FDiag(ksq))) & bb(Them) & checkmask);
  last = make_pawn_moves<UpLeft >(last, shift<UpLeft >(pawns & (not_pinned | BDiag(ksq))) & bb(Them) & checkmask);
  last = make_pawn_moves<Up     >(last, shift<Up     >(pawns & (not_pinned | File (ksq))) & empty    & checkmask);
  last = make_pawn_moves<Up2    >(last, shift<Up2    >(pawns & (not_pinned | File (ksq))) & e        & checkmask);

  if (Bitboard promotable = bb(FriendlyPawn) & Promote)
  {
    for (Bitboard b = shift<UpRight>(promotable & (not_pinned | FDiag(ksq))) & bb(Them) & checkmask; b; pop_lsb(b)) {
      Square to = lsb(b);
      *last++ = make_move<PROMOTION>(to - UpRight, to);
    }
    for (Bitboard b = shift<UpLeft >(promotable & (not_pinned | BDiag(ksq))) & bb(Them) & checkmask; b; pop_lsb(b)) {
      Square to = lsb(b);
      *last++ = make_move<PROMOTION>(to - UpLeft, to);
    }
    for (Bitboard b = shift<Up>(promotable & not_pinned) & empty & checkmask; b; pop_lsb(b)) {
      Square to = lsb(b);
      *last++ = make_move<PROMOTION>(to - Up, to);
    }
  }

  if (ep_enabled)
  {
    Bitboard ep_square = EnemyEP & GameState::current_ep_square();
    if (Bitboard b = shift<UpRight>(bb(FriendlyPawn)) & ep_square) {
      Square to = lsb(b);
      *last++ = make_move<ENPASSANT>(to - UpRight, to);
      Bitboard ep_toggle = b | shift<-UpRight>(b) | shift<-Up>(b);
      Bitboard o = occupied ^ ep_toggle;
      Bitboard slider_checks = BishopAttacks(ksq, o) & enemy_bishop_queen | RookAttacks(ksq, o) & enemy_rook_queen;
      if (slider_checks)
        last--;
    }
    if (Bitboard b = shift<UpLeft >(bb(FriendlyPawn)) & ep_square) {
      Square to = lsb(b);
      *last++ = make_move<ENPASSANT>(to - UpLeft, to);
      Bitboard ep_toggle = b | shift<-UpLeft>(b) | shift<-Up>(b);
      Bitboard o = occupied ^ ep_toggle;
      Bitboard slider_checks = BishopAttacks(ksq, o) & enemy_bishop_queen | RookAttacks(ksq, o) & enemy_rook_queen;
      if (slider_checks)
        last--;
    }
  }

  Bitboard friendly_rook_queen   = bb(FriendlyQueen) | bb(FriendlyRook);
  Bitboard friendly_bishop_queen = bb(FriendlyQueen) | bb(FriendlyBishop);
  Bitboard legal = checkmask &~ bb(Us);

  for (Bitboard b = bb(FriendlyKnight) & not_pinned; b; pop_lsb(b)) {
    Square from = lsb(b);
    last = make_moves(last, from, KnightAttacks(from) & legal);
  }
  for (Bitboard b = friendly_bishop_queen & not_pinned; b; pop_lsb(b)) {
    Square from = lsb(b);
    last = make_moves(last, from, BishopAttacks(from, occupied) & legal);
  }
  for (Bitboard b = friendly_bishop_queen & pinned; b; pop_lsb(b)) {
    Square from = lsb(b);
    last = make_moves(last, from, BishopAttacks(from, occupied) & legal & PinMask(ksq, from));
  }
  for (Bitboard b = friendly_rook_queen & not_pinned; b; pop_lsb(b)) {
    Square from = lsb(b);
    last = make_moves(last, from, RookAttacks(from, occupied) & legal);
  }
  for (Bitboard b = friendly_rook_queen & pinned; b; pop_lsb(b)) {
    Square from = lsb(b);
    last = make_moves(last, from, RookAttacks(from, occupied) & legal & PinMask(ksq, from));
  }

  last = make_moves(last, ksq, KingAttacks(ksq) & ~(seen_by_enemy | bb(Us)));

  if (~checkmask) return;

  constexpr Bitboard KingBan  = Us == WHITE ? square_bb(F1, G1)     : square_bb(F8, G8);
  constexpr Bitboard QueenOcc = Us == WHITE ? square_bb(B1, C1, D1) : square_bb(B8, C8, D8);
  constexpr Bitboard QueenAtk = Us == WHITE ? square_bb(C1, D1)     : square_bb(C8, D8);
  constexpr Bitboard KingKey  = Us == WHITE ? 0b1000                : 0b0010;
  constexpr Bitboard QueenKey = Us == WHITE ? 0b0100                : 0b0001;
  constexpr Move     SCASTLE  = Us == WHITE ? W_SCASTLE             : B_SCASTLE;
  constexpr Move     LCASTLE  = Us == WHITE ? W_LCASTLE             : B_LCASTLE;
            
  uint64_t hash;

  *last = SCASTLE;
  hash = (occupied | seen_by_enemy) & KingBan | GameState::kingside_rights<Us>();
  last += !(hash ^ KingKey);

  *last = LCASTLE;
  hash = occupied & QueenOcc | seen_by_enemy & QueenAtk | GameState::queenside_rights<Us>();
  last += !(hash ^ QueenKey);
  
}

#endif
