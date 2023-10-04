
#include "WhiteCaptureGenerator.h"
#include "Board.h"
#include "UI.h"
#include "Defs.h"
#include "Lookup.h"
#include <intrin.h>
#include <iostream>

namespace Chess {

	WhiteCaptureGenerator::WhiteCaptureGenerator() {

		move_count = 0;
		init();
		if (checkmask == 0) checkmask = MAX_LONG;
		knight_and_bishop_targets &= checkmask;
		rook_targets &= checkmask;
		queen_targets &= checkmask;

	}

	void WhiteCaptureGenerator::generate_moves() {

		if (in_double_check) {
			generate_WK();
			return;
		}
		generate();
		if (Board::bitboards[WHITE_PAWN] & seventh_rank) gen_promotions();

	}

	void WhiteCaptureGenerator::init() {

		pinV = 0;
		pinFD = 0;
		pinH = 0;
		pinBD = 0;
		checkmask = 0;
		friendly_ksq = tzcnt(Board::bitboards[WHITE_KING]);
		checkmask |= Lookup::knight_masks[friendly_ksq] & Board::bitboards[BLACK_KNIGHT];
		checkmask |= Lookup::black_pawn_checkers[friendly_ksq] & Board::bitboards[BLACK_PAWN];
		occupied = Board::black_pieces | Board::white_pieces;
		seen_by_enemy = squares_controlled_by_black();
		empty = ~occupied;
		uint64_t black_unprotected = Board::black_pieces & ~seen_by_enemy;
		knight_and_bishop_targets = (Board::black_pieces & ~Board::bitboards[BLACK_PAWN]) | black_unprotected;
		rook_targets = Board::bitboards[BLACK_ROOK] | Board::bitboards[BLACK_QUEEN] | black_unprotected;
		queen_targets = Board::bitboards[BLACK_QUEEN] | black_unprotected;
		uint64_t checkers = 
		(BISHOP_ATTACKS(friendly_ksq, Board::black_pieces) & (Board::bitboards[BLACK_QUEEN] | Board::bitboards[BLACK_BISHOP])) 
		| (ROOK_ATTACKS(friendly_ksq, Board::black_pieces) & (Board::bitboards[BLACK_QUEEN] | Board::bitboards[BLACK_ROOK]));
		
		for (;checkers; checkers = blsr(checkers)) {
			uint64_t checkray = Lookup::checkmask[friendly_ksq][tzcnt(checkers)];
			uint64_t crossfire_whites = Board::white_pieces & checkray;
			checkmask |= checkray * (crossfire_whites == 0);
			uint64_t pinray = checkray * (popcnt(crossfire_whites) == 1);
			pinV  |= pinray & Lookup::vertical[friendly_ksq];
			pinFD |= pinray & Lookup::forward_diagonal[friendly_ksq];
			pinH  |= pinray & Lookup::horizontal[friendly_ksq];
			pinBD |= pinray & Lookup::back_diagonal[friendly_ksq];
		}

		pin_O = pinV | pinH;
		pin_D = pinFD | pinBD;
		not_pinned = ~(pin_O | pin_D);
		in_double_check = (popcnt(Lookup::double_check[friendly_ksq] & checkmask) > 1);

	}

	void WhiteCaptureGenerator::generate() {

		uint64_t pawns = Board::bitboards[WHITE_PAWN] & not_7_or_h & ~(pin_O | pinBD);
		for (uint64_t upright = (pawns << 7) & Board::black_pieces & checkmask; upright; upright = blsr(upright)) {
			int to = tzcnt(upright);
			moves[move_count++] = to - 7 + (to << 6);
		}
		pawns = Board::bitboards[WHITE_PAWN] & not_7_or_a & ~(pin_O | pinFD);
		for (uint64_t upleft = (pawns << 9) & Board::black_pieces & checkmask; upleft; upleft = blsr(upleft)) {
			int to = tzcnt(upleft);
			moves[move_count++] = to - 9 + (to << 6);
		}
		for (uint64_t from_map = Board::bitboards[WHITE_KNIGHT] & not_pinned; from_map; from_map = blsr(from_map)) {
			int from = tzcnt(from_map);
			for (uint64_t to_map = Lookup::knight_masks[from] & knight_and_bishop_targets; to_map; to_map = blsr(to_map))
				moves[move_count++] = from + (tzcnt(to_map) << 6);
		}
		for (uint64_t from_map = Board::bitboards[WHITE_BISHOP] & not_pinned; from_map; from_map = blsr(from_map)) {
			int from = tzcnt(from_map);
			for (uint64_t to_map = BISHOP_ATTACKS(from, occupied) & knight_and_bishop_targets; to_map; to_map = blsr(to_map))
				moves[move_count++] = from + (tzcnt(to_map) << 6);
		}
		for (uint64_t from_map = Board::bitboards[WHITE_BISHOP] & ~not_pinned; from_map; from_map = blsr(from_map)) {
			int from = tzcnt(from_map);
			for (uint64_t to_map = BISHOP_ATTACKS(from, occupied) & knight_and_bishop_targets & Lookup::pinmask[friendly_ksq][from]; to_map; to_map = blsr(to_map))
				moves[move_count++] = from + (tzcnt(to_map) << 6);
		}
		for (uint64_t from_map = Board::bitboards[WHITE_ROOK] & not_pinned; from_map; from_map = blsr(from_map)) {
			int from = tzcnt(from_map);
			for (uint64_t to_map = ROOK_ATTACKS(from, occupied) & rook_targets; to_map; to_map = blsr(to_map))
				moves[move_count++] = from + (tzcnt(to_map) << 6);
		}
		for (uint64_t from_map = Board::bitboards[WHITE_ROOK] & ~not_pinned; from_map; from_map = blsr(from_map)) {
			int from = tzcnt(from_map);
			for (uint64_t to_map = ROOK_ATTACKS(from, occupied) & rook_targets & Lookup::pinmask[friendly_ksq][from]; to_map; to_map = blsr(to_map))
				moves[move_count++] = from + (tzcnt(to_map) << 6);
		}
		for (uint64_t from_map = Board::bitboards[WHITE_QUEEN] & not_pinned; from_map; from_map = blsr(from_map)) {
			int from = tzcnt(from_map);
			for (uint64_t to_map = QUEEN_ATTACKS(from, occupied) & queen_targets; to_map; to_map = blsr(to_map))
				moves[move_count++] = from + (tzcnt(to_map) << 6);
		}
		for (uint64_t from_map = Board::bitboards[WHITE_QUEEN] & ~not_pinned; from_map; from_map = blsr(from_map)) {
			int from = tzcnt(from_map);
			for (uint64_t to_map = QUEEN_ATTACKS(from, occupied) & queen_targets & Lookup::pinmask[friendly_ksq][from]; to_map; to_map = blsr(to_map))
				moves[move_count++] = from + (tzcnt(to_map) << 6);
		}
		for (uint64_t to_map = Lookup::king_masks[friendly_ksq] & Board::black_pieces & ~seen_by_enemy; to_map; to_map = blsr(to_map))
			moves[move_count++] = friendly_ksq + (tzcnt(to_map) << 6);

	}

	void WhiteCaptureGenerator::gen_promotions() {

		uint64_t pawns_on_7 = Board::bitboards[WHITE_PAWN] & seventh_rank;
		uint64_t pawns = pawns_on_7 & ~(hfile | pin_O | pinBD);
		for (uint64_t upright = (pawns << 7) & Board::black_pieces & checkmask; upright; upright = blsr(upright)) {
			int to = tzcnt(upright);
			moves[move_count++] = to - 7 + (to << 6) + promotion_flag;
		}
		pawns = pawns_on_7 & ~(afile | pin_O | pinFD);
		for (uint64_t upleft = (pawns << 9) & Board::black_pieces & checkmask; upleft; upleft = blsr(upleft)) {
			int to = tzcnt(upleft);
			moves[move_count++] = to - 9 + (to << 6) + promotion_flag;
		}
		
	}

	void WhiteCaptureGenerator::generate_WK() {

		for (uint64_t to_map = Lookup::king_masks[friendly_ksq] & Board::black_pieces & ~seen_by_enemy; to_map; to_map = blsr(to_map))
			moves[move_count++] = friendly_ksq + (tzcnt(to_map) << 6);

	}

	uint64_t WhiteCaptureGenerator::squares_controlled_by_black() {

		uint64_t occ_kingless = occupied ^ (1ull << friendly_ksq);
		uint64_t protected_squares = ((Board::bitboards[BLACK_PAWN] >> 7) & NOT_FILE_H) | ((Board::bitboards[BLACK_PAWN] >> 9) & NOT_FILE_A);

		for (uint64_t piece_map = Board::bitboards[BLACK_KNIGHT]; piece_map; piece_map = blsr(piece_map))
			protected_squares |= Lookup::knight_masks[tzcnt(piece_map)];
		for (uint64_t piece_map = Board::bitboards[BLACK_BISHOP] | Board::bitboards[BLACK_QUEEN]; piece_map; piece_map = blsr(piece_map))
			protected_squares |= BISHOP_ATTACKS(tzcnt(piece_map), occ_kingless);
		for (uint64_t piece_map = Board::bitboards[BLACK_ROOK] | Board::bitboards[BLACK_QUEEN]; piece_map; piece_map = blsr(piece_map))
			protected_squares |= ROOK_ATTACKS(tzcnt(piece_map), occ_kingless);
		return protected_squares | Lookup::king_masks[tzcnt(Board::bitboards[BLACK_KING])];

	}

}
