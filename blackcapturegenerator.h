
#ifndef BLACKCAPTUREGENERATOR_H
#define BLACKCAPTUREGENERATOR_H

#include <cstdint>
#include <string>

class BlackCaptureGenerator {

public:
	int moves[16];
	int move_count;
	int friendly_ksq;
	void init();
	uint64_t occupied;
	uint64_t empty;
	uint64_t checkmask;
	uint64_t pinH;
	uint64_t pinV;
	uint64_t pinFD;
	uint64_t pinBD;
	uint64_t pin_O;
	uint64_t pin_D;
	uint64_t not_pinned;
	uint64_t seen_by_enemy;
	uint64_t knight_and_bishop_targets;
	uint64_t rook_targets;
	uint64_t queen_targets;
	bool in_double_check;
	BlackCaptureGenerator();
	void generate_moves();
	uint64_t squares_seen_by_white();
	void generate_BK();
	void gen_promotions();
	void generate();

};

#endif
