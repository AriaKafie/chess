
#ifndef LOOKUP_H
#define LOOKUP_H

#include <cstdint>

namespace Chess {

namespace Lookup {

	int chebyshev_distance_between(int square1, int square2);
	uint64_t generate_checkmask(int king, int queen);
	uint64_t bit_reverse(uint64_t num);
	uint64_t generate_rook_map(int square, uint64_t occupancy);
	uint64_t generate_bishop_map(int square, uint64_t occupancy);
	uint64_t generate_occupancy(uint64_t mask, int iteration);
	int score_white_pawnshield(int kingsq, uint64_t occupancy);
	int score_black_pawnshield(int kingsq, uint64_t occupancy);

	void init();
	void init_checkmasks();
	void init_pinmasks();
	void init_chebyshev_table();
	void init_bishop_attacks();
	void init_rook_attacks();
	void init_white_pawnshield_scores();
	void init_black_pawnshield_scores();

	inline int chebyshev[64][64];
	inline uint64_t checkmask[64][64];
	inline uint64_t pinmask[64][64];
	inline uint64_t rook_attacks[102400];
	inline uint64_t bishop_attacks[5248];
	inline int white_pawnshield_scores[64][64];
	inline int black_pawnshield_scores[64][64];

	inline constexpr uint64_t white_pawnshield[64] = {
		0x70700ull,0x70700ull,0xe0e00ull,0x1c1c00ull,0x383800ull,0x707000ull,0xe0e000ull,0xe0e000ull,
		0x7070000ull,0x7070000ull,0xe0e0000ull,0x1c1c0000ull,0x38380000ull,0x70700000ull,0xe0e00000ull,0xe0e00000ull,
		0x707000000ull,0x707000000ull,0xe0e000000ull,0x1c1c000000ull,0x3838000000ull,0x7070000000ull,0xe0e0000000ull,0xe0e0000000ull,
		0x70700000000ull,0x70700000000ull,0xe0e00000000ull,0x1c1c00000000ull,0x383800000000ull,0x707000000000ull,0xe0e000000000ull,0xe0e000000000ull,
		0x7070000000000ull,0x7070000000000ull,0xe0e0000000000ull,0x1c1c0000000000ull,0x38380000000000ull,0x70700000000000ull,0xe0e00000000000ull,0xe0e00000000000ull,
		0x707000000000000ull,0x707000000000000ull,0xe0e000000000000ull,0x1c1c000000000000ull,0x3838000000000000ull,0x7070000000000000ull,0xe0e0000000000000ull,0xe0e0000000000000ull,
		0x700000000000000ull,0x700000000000000ull,0xe00000000000000ull,0x1c00000000000000ull,0x3800000000000000ull,0x7000000000000000ull,0xe000000000000000ull,0xe000000000000000ull,
		0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,
	};
	inline constexpr uint64_t black_pawnshield[64] = {
		0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,
		0x7ull,0x7ull,0xeull,0x1cull,0x38ull,0x70ull,0xe0ull,0xe0ull,
		0x707ull,0x707ull,0xe0eull,0x1c1cull,0x3838ull,0x7070ull,0xe0e0ull,0xe0e0ull,
		0x70700ull,0x70700ull,0xe0e00ull,0x1c1c00ull,0x383800ull,0x707000ull,0xe0e000ull,0xe0e000ull,
		0x7070000ull,0x7070000ull,0xe0e0000ull,0x1c1c0000ull,0x38380000ull,0x70700000ull,0xe0e00000ull,0xe0e00000ull,
		0x707000000ull,0x707000000ull,0xe0e000000ull,0x1c1c000000ull,0x3838000000ull,0x7070000000ull,0xe0e0000000ull,0xe0e0000000ull,
		0x70700000000ull,0x70700000000ull,0xe0e00000000ull,0x1c1c00000000ull,0x383800000000ull,0x707000000000ull,0xe0e000000000ull,0xe0e000000000ull,
		0x7070000000000ull,0x7070000000000ull,0xe0e0000000000ull,0x1c1c0000000000ull,0x38380000000000ull,0x70700000000000ull,0xe0e00000000000ull,0xe0e00000000000ull,
	};
	inline constexpr uint64_t white_pawn_checkers[64] = {
		0x0ULL,0x0ULL,0x0ULL,0x0ULL,0x0ULL,0x0ULL,0x0ULL,0x0ULL,
		0x0ULL,0x0ULL,0x0ULL,0x0ULL,0x0ULL,0x0ULL,0x0ULL,0x0ULL,
		0x200ULL,0x500ULL,0xa00ULL,0x1400ULL,0x2800ULL,0x5000ULL,0xa000ULL,0x4000ULL,
		0x20000ULL,0x50000ULL,0xa0000ULL,0x140000ULL,0x280000ULL,0x500000ULL,0xa00000ULL,0x400000ULL,
		0x2000000ULL,0x5000000ULL,0xa000000ULL,0x14000000ULL,0x28000000ULL,0x50000000ULL,0xa0000000ULL,0x40000000ULL,
		0x200000000ULL,0x500000000ULL,0xa00000000ULL,0x1400000000ULL,0x2800000000ULL,0x5000000000ULL,0xa000000000ULL,0x4000000000ULL,
		0x20000000000ULL,0x50000000000ULL,0xa0000000000ULL,0x140000000000ULL,0x280000000000ULL,0x500000000000ULL,0xa00000000000ULL,0x400000000000ULL,
		0x2000000000000ULL,0x5000000000000ULL,0xa000000000000ULL,0x14000000000000ULL,0x28000000000000ULL,0x50000000000000ULL,0xa0000000000000ULL,0x40000000000000ULL,
	};
	inline constexpr uint64_t black_pawn_checkers[64] = {
		0x200ULL,0x500ULL,0xa00ULL,0x1400ULL,0x2800ULL,0x5000ULL,0xa000ULL,0x4000ULL,
		0x20000ULL,0x50000ULL,0xa0000ULL,0x140000ULL,0x280000ULL,0x500000ULL,0xa00000ULL,0x400000ULL,
		0x2000000ULL,0x5000000ULL,0xa000000ULL,0x14000000ULL,0x28000000ULL,0x50000000ULL,0xa0000000ULL,0x40000000ULL,
		0x200000000ULL,0x500000000ULL,0xa00000000ULL,0x1400000000ULL,0x2800000000ULL,0x5000000000ULL,0xa000000000ULL,0x4000000000ULL,
		0x20000000000ULL,0x50000000000ULL,0xa0000000000ULL,0x140000000000ULL,0x280000000000ULL,0x500000000000ULL,0xa00000000000ULL,0x400000000000ULL,
		0x2000000000000ULL,0x5000000000000ULL,0xa000000000000ULL,0x14000000000000ULL,0x28000000000000ULL,0x50000000000000ULL,0xa0000000000000ULL,0x40000000000000ULL,
		0x0ULL,0x0ULL,0x0ULL,0x0ULL,0x0ULL,0x0ULL,0x0ULL,0x0ULL,
		0x0ULL,0x0ULL,0x0ULL,0x0ULL,0x0ULL,0x0ULL,0x0ULL,0x0ULL,
	};
	inline constexpr uint64_t double_check[64] = {
		0x20702ULL,0x50f05ULL,0xa1f0aULL,0x143e14ULL,0x287c28ULL,0x50f850ULL,0xa0f0a0ULL,0x40e040ULL,
		0x2070207ULL,0x50f050fULL,0xa1f0a1fULL,0x143e143eULL,0x287c287cULL,0x50f850f8ULL,0xa0f0a0f0ULL,0x40e040e0ULL,
		0x207020702ULL,0x50f050f05ULL,0xa1f0a1f0aULL,0x143e143e14ULL,0x287c287c28ULL,0x50f850f850ULL,0xa0f0a0f0a0ULL,0x40e040e040ULL,
		0x20702070200ULL,0x50f050f0500ULL,0xa1f0a1f0a00ULL,0x143e143e1400ULL,0x287c287c2800ULL,0x50f850f85000ULL,0xa0f0a0f0a000ULL,0x40e040e04000ULL,
		0x2070207020000ULL,0x50f050f050000ULL,0xa1f0a1f0a0000ULL,0x143e143e140000ULL,0x287c287c280000ULL,0x50f850f8500000ULL,0xa0f0a0f0a00000ULL,0x40e040e0400000ULL,
		0x207020702000000ULL,0x50f050f05000000ULL,0xa1f0a1f0a000000ULL,0x143e143e14000000ULL,0x287c287c28000000ULL,0x50f850f850000000ULL,0xa0f0a0f0a0000000ULL,0x40e040e040000000ULL,
		0x702070200000000ULL,0xf050f0500000000ULL,0x1f0a1f0a00000000ULL,0x3e143e1400000000ULL,0x7c287c2800000000ULL,0xf850f85000000000ULL,0xf0a0f0a000000000ULL,0xe040e04000000000ULL,
		0x207020000000000ULL,0x50f050000000000ULL,0xa1f0a0000000000ULL,0x143e140000000000ULL,0x287c280000000000ULL,0x50f8500000000000ULL,0xa0f0a00000000000ULL,0x40e0400000000000ULL,
	};
	inline constexpr uint64_t forward_diagonal[64] = {
		0x1ull,0x102ull,0x10204ull,0x1020408ull,0x102040810ull,0x10204081020ull,0x1020408102040ull,0x102040810204080ull,
		0x102ull,0x10204ull,0x1020408ull,0x102040810ull,0x10204081020ull,0x1020408102040ull,0x102040810204080ull,0x204081020408000ull,
		0x10204ull,0x1020408ull,0x102040810ull,0x10204081020ull,0x1020408102040ull,0x102040810204080ull,0x204081020408000ull,0x408102040800000ull,
		0x1020408ull,0x102040810ull,0x10204081020ull,0x1020408102040ull,0x102040810204080ull,0x204081020408000ull,0x408102040800000ull,0x810204080000000ull,
		0x102040810ull,0x10204081020ull,0x1020408102040ull,0x102040810204080ull,0x204081020408000ull,0x408102040800000ull,0x810204080000000ull,0x1020408000000000ull,
		0x10204081020ull,0x1020408102040ull,0x102040810204080ull,0x204081020408000ull,0x408102040800000ull,0x810204080000000ull,0x1020408000000000ull,0x2040800000000000ull,
		0x1020408102040ull,0x102040810204080ull,0x204081020408000ull,0x408102040800000ull,0x810204080000000ull,0x1020408000000000ull,0x2040800000000000ull,0x4080000000000000ull,
		0x102040810204080ull,0x204081020408000ull,0x408102040800000ull,0x810204080000000ull,0x1020408000000000ull,0x2040800000000000ull,0x4080000000000000ull,0x8000000000000000ull,
	};
	inline constexpr uint64_t back_diagonal[64] = {
		0x8040201008040201ull,0x80402010080402ull,0x804020100804ull,0x8040201008ull,0x80402010ull,0x804020ull,0x8040ull,0x80ull,
		0x4020100804020100ull,0x8040201008040201ull,0x80402010080402ull,0x804020100804ull,0x8040201008ull,0x80402010ull,0x804020ull,0x8040ull,
		0x2010080402010000ull,0x4020100804020100ull,0x8040201008040201ull,0x80402010080402ull,0x804020100804ull,0x8040201008ull,0x80402010ull,0x804020ull,
		0x1008040201000000ull,0x2010080402010000ull,0x4020100804020100ull,0x8040201008040201ull,0x80402010080402ull,0x804020100804ull,0x8040201008ull,0x80402010ull,
		0x804020100000000ull,0x1008040201000000ull,0x2010080402010000ull,0x4020100804020100ull,0x8040201008040201ull,0x80402010080402ull,0x804020100804ull,0x8040201008ull,
		0x402010000000000ull,0x804020100000000ull,0x1008040201000000ull,0x2010080402010000ull,0x4020100804020100ull,0x8040201008040201ull,0x80402010080402ull,0x804020100804ull,
		0x201000000000000ull,0x402010000000000ull,0x804020100000000ull,0x1008040201000000ull,0x2010080402010000ull,0x4020100804020100ull,0x8040201008040201ull,0x80402010080402ull,
		0x100000000000000ull,0x201000000000000ull,0x402010000000000ull,0x804020100000000ull,0x1008040201000000ull,0x2010080402010000ull,0x4020100804020100ull,0x8040201008040201ull,
	};
	inline constexpr uint64_t vertical[64] = {
		0x101010101010101ull,0x202020202020202ull,0x404040404040404ull,0x808080808080808ull,0x1010101010101010ull,0x2020202020202020ull,0x4040404040404040ull,0x8080808080808080ull,
		0x101010101010101ull,0x202020202020202ull,0x404040404040404ull,0x808080808080808ull,0x1010101010101010ull,0x2020202020202020ull,0x4040404040404040ull,0x8080808080808080ull,
		0x101010101010101ull,0x202020202020202ull,0x404040404040404ull,0x808080808080808ull,0x1010101010101010ull,0x2020202020202020ull,0x4040404040404040ull,0x8080808080808080ull,
		0x101010101010101ull,0x202020202020202ull,0x404040404040404ull,0x808080808080808ull,0x1010101010101010ull,0x2020202020202020ull,0x4040404040404040ull,0x8080808080808080ull,
		0x101010101010101ull,0x202020202020202ull,0x404040404040404ull,0x808080808080808ull,0x1010101010101010ull,0x2020202020202020ull,0x4040404040404040ull,0x8080808080808080ull,
		0x101010101010101ull,0x202020202020202ull,0x404040404040404ull,0x808080808080808ull,0x1010101010101010ull,0x2020202020202020ull,0x4040404040404040ull,0x8080808080808080ull,
		0x101010101010101ull,0x202020202020202ull,0x404040404040404ull,0x808080808080808ull,0x1010101010101010ull,0x2020202020202020ull,0x4040404040404040ull,0x8080808080808080ull,
		0x101010101010101ull,0x202020202020202ull,0x404040404040404ull,0x808080808080808ull,0x1010101010101010ull,0x2020202020202020ull,0x4040404040404040ull,0x8080808080808080ull,
	};
	inline constexpr uint64_t horizontal[64] = {
		0xffull,0xffull,0xffull,0xffull,0xffull,0xffull,0xffull,0xffull,
		0xff00ull,0xff00ull,0xff00ull,0xff00ull,0xff00ull,0xff00ull,0xff00ull,0xff00ull,
		0xff0000ull,0xff0000ull,0xff0000ull,0xff0000ull,0xff0000ull,0xff0000ull,0xff0000ull,0xff0000ull,
		0xff000000ull,0xff000000ull,0xff000000ull,0xff000000ull,0xff000000ull,0xff000000ull,0xff000000ull,0xff000000ull,
		0xff00000000ull,0xff00000000ull,0xff00000000ull,0xff00000000ull,0xff00000000ull,0xff00000000ull,0xff00000000ull,0xff00000000ull,
		0xff0000000000ull,0xff0000000000ull,0xff0000000000ull,0xff0000000000ull,0xff0000000000ull,0xff0000000000ull,0xff0000000000ull,0xff0000000000ull,
		0xff000000000000ull,0xff000000000000ull,0xff000000000000ull,0xff000000000000ull,0xff000000000000ull,0xff000000000000ull,0xff000000000000ull,0xff000000000000ull,
		0xff00000000000000ull,0xff00000000000000ull,0xff00000000000000ull,0xff00000000000000ull,0xff00000000000000ull,0xff00000000000000ull,0xff00000000000000ull,0xff00000000000000ull,
	};
	inline constexpr uint64_t knight_masks[64] = {
		0x20400ull,0x50800ull,0xa1100ull,0x142200ull,0x284400ull,0x508800ull,0xa01000ull,0x402000ull,
		0x2040004ull,0x5080008ull,0xa110011ull,0x14220022ull,0x28440044ull,0x50880088ull,0xa0100010ull,0x40200020ull,
		0x204000402ull,0x508000805ull,0xa1100110aull,0x1422002214ull,0x2844004428ull,0x5088008850ull,0xa0100010a0ull,0x4020002040ull,
		0x20400040200ull,0x50800080500ull,0xa1100110a00ull,0x142200221400ull,0x284400442800ull,0x508800885000ull,0xa0100010a000ull,0x402000204000ull,
		0x2040004020000ull,0x5080008050000ull,0xa1100110a0000ull,0x14220022140000ull,0x28440044280000ull,0x50880088500000ull,0xa0100010a00000ull,0x40200020400000ull,
		0x204000402000000ull,0x508000805000000ull,0xa1100110a000000ull,0x1422002214000000ull,0x2844004428000000ull,0x5088008850000000ull,0xa0100010a0000000ull,0x4020002040000000ull,
		0x400040200000000ull,0x800080500000000ull,0x1100110a00000000ull,0x2200221400000000ull,0x4400442800000000ull,0x8800885000000000ull,0x100010a000000000ull,0x2000204000000000ull,
		0x4020000000000ull,0x8050000000000ull,0x110a0000000000ull,0x22140000000000ull,0x44280000000000ull,0x88500000000000ull,0x10a00000000000ull,0x20400000000000ull,
	};
	inline constexpr uint64_t king_masks[64] = {
		0x302ull,0x705ull,0xe0aull,0x1c14ull,0x3828ull,0x7050ull,0xe0a0ull,0xc040ull,
		0x30203ull,0x70507ull,0xe0a0eull,0x1c141cull,0x382838ull,0x705070ull,0xe0a0e0ull,0xc040c0ull,
		0x3020300ull,0x7050700ull,0xe0a0e00ull,0x1c141c00ull,0x38283800ull,0x70507000ull,0xe0a0e000ull,0xc040c000ull,
		0x302030000ull,0x705070000ull,0xe0a0e0000ull,0x1c141c0000ull,0x3828380000ull,0x7050700000ull,0xe0a0e00000ull,0xc040c00000ull,
		0x30203000000ull,0x70507000000ull,0xe0a0e000000ull,0x1c141c000000ull,0x382838000000ull,0x705070000000ull,0xe0a0e0000000ull,0xc040c0000000ull,
		0x3020300000000ull,0x7050700000000ull,0xe0a0e00000000ull,0x1c141c00000000ull,0x38283800000000ull,0x70507000000000ull,0xe0a0e000000000ull,0xc040c000000000ull,
		0x302030000000000ull,0x705070000000000ull,0xe0a0e0000000000ull,0x1c141c0000000000ull,0x3828380000000000ull,0x7050700000000000ull,0xe0a0e00000000000ull,0xc040c00000000000ull,
		0x203000000000000ull,0x507000000000000ull,0xa0e000000000000ull,0x141c000000000000ull,0x2838000000000000ull,0x5070000000000000ull,0xa0e0000000000000ull,0x40c0000000000000ull,
	};
	inline constexpr uint64_t bishop_masks[64] = {
		0x40201008040200ull,0x402010080400ull,0x4020100a00ull,0x40221400ull,0x2442800ull,0x204085000ull,0x20408102000ull,0x2040810204000ull,
		0x20100804020000ull,0x40201008040000ull,0x4020100a0000ull,0x4022140000ull,0x244280000ull,0x20408500000ull,0x2040810200000ull,0x4081020400000ull,
		0x10080402000200ull,0x20100804000400ull,0x4020100a000a00ull,0x402214001400ull,0x24428002800ull,0x2040850005000ull,0x4081020002000ull,0x8102040004000ull,
		0x8040200020400ull,0x10080400040800ull,0x20100a000a1000ull,0x40221400142200ull,0x2442800284400ull,0x4085000500800ull,0x8102000201000ull,0x10204000402000ull,
		0x4020002040800ull,0x8040004081000ull,0x100a000a102000ull,0x22140014224000ull,0x44280028440200ull,0x8500050080400ull,0x10200020100800ull,0x20400040201000ull,
		0x2000204081000ull,0x4000408102000ull,0xa000a10204000ull,0x14001422400000ull,0x28002844020000ull,0x50005008040200ull,0x20002010080400ull,0x40004020100800ull,
		0x20408102000ull,0x40810204000ull,0xa1020400000ull,0x142240000000ull,0x284402000000ull,0x500804020000ull,0x201008040200ull,0x402010080400ull,
		0x2040810204000ull,0x4081020400000ull,0xa102040000000ull,0x14224000000000ull,0x28440200000000ull,0x50080402000000ull,0x20100804020000ull,0x40201008040200ull,
	};
	inline constexpr uint64_t rook_masks[64] = {
		0x101010101017eull,0x202020202027cull,0x404040404047aull,0x8080808080876ull,0x1010101010106eull,0x2020202020205eull,0x4040404040403eull,0x8080808080807eull,
		0x1010101017e00ull,0x2020202027c00ull,0x4040404047a00ull,0x8080808087600ull,0x10101010106e00ull,0x20202020205e00ull,0x40404040403e00ull,0x80808080807e00ull,
		0x10101017e0100ull,0x20202027c0200ull,0x40404047a0400ull,0x8080808760800ull,0x101010106e1000ull,0x202020205e2000ull,0x404040403e4000ull,0x808080807e8000ull,
		0x101017e010100ull,0x202027c020200ull,0x404047a040400ull,0x8080876080800ull,0x1010106e101000ull,0x2020205e202000ull,0x4040403e404000ull,0x8080807e808000ull,
		0x1017e01010100ull,0x2027c02020200ull,0x4047a04040400ull,0x8087608080800ull,0x10106e10101000ull,0x20205e20202000ull,0x40403e40404000ull,0x80807e80808000ull,
		0x17e0101010100ull,0x27c0202020200ull,0x47a0404040400ull,0x8760808080800ull,0x106e1010101000ull,0x205e2020202000ull,0x403e4040404000ull,0x807e8080808000ull,
		0x7e010101010100ull,0x7c020202020200ull,0x7a040404040400ull,0x76080808080800ull,0x6e101010101000ull,0x5e202020202000ull,0x3e404040404000ull,0x7e808080808000ull,
		0x7e01010101010100ull,0x7c02020202020200ull,0x7a04040404040400ull,0x7608080808080800ull,0x6e10101010101000ull,0x5e20202020202000ull,0x3e40404040404000ull,0x7e80808080808000ull,
	};
	inline constexpr int bishop_hash[64] = {
		0, 64, 96, 128, 160, 192, 224, 256,
		320, 352, 384, 416, 448, 480, 512, 544,
		576, 608, 640, 768, 896, 1024, 1152, 1184,
		1216, 1248, 1280, 1408, 1920, 2432, 2560, 2592,
		2624, 2656, 2688, 2816, 3328, 3840, 3968, 4000,
		4032, 4064, 4096, 4224, 4352, 4480, 4608, 4640,
		4672, 4704, 4736, 4768, 4800, 4832, 4864, 4896,
		4928, 4992, 5024, 5056, 5088, 5120, 5152, 5184,
	};
	inline constexpr int rook_hash[64] = {
		0, 4096, 6144, 8192, 10240, 12288, 14336, 16384,
		20480, 22528, 23552, 24576, 25600, 26624, 27648, 28672,
		30720, 32768, 33792, 34816, 35840, 36864, 37888, 38912,
		40960, 43008, 44032, 45056, 46080, 47104, 48128, 49152,
		51200, 53248, 54272, 55296, 56320, 57344, 58368, 59392,
		61440, 63488, 64512, 65536, 66560, 67584, 68608, 69632,
		71680, 73728, 74752, 75776, 76800, 77824, 78848, 79872,
		81920, 86016, 88064, 90112, 92160, 94208, 96256, 98304,
	};
	inline constexpr int dist_from_center[64] = {
		6, 5, 4, 3, 3, 4, 5, 6,
		5, 4, 3, 2, 2, 3, 4, 5,
		4, 3, 2, 1, 1, 2, 3, 4,
		3, 2, 1, 0, 0, 1, 2, 3,
		3, 2, 1, 0, 0, 1, 2, 3,
		4, 3, 2, 1, 1, 2, 3, 4,
		5, 4, 3, 2, 2, 3, 4, 5,
		6, 5, 4, 3, 3, 4, 5, 6,
	};
	inline constexpr int piece_weights[13] = {
		100,300,300,500,900,1500,100,300,300,500,900,1500,0,
	};

}

}

#endif
