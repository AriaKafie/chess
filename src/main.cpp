
#include "bitboard.h"
#include "search.h"
#include "position.h"
#include "uci.h"

int main() {

    Bitboards::init();
    Search::init();
    Position::init();

    UCI::loop();
}
