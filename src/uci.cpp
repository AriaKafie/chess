
#include "uci.h"

#include <iostream>
#include <sstream>

#include "debug.h"
#include "movegen.h"
#include "position.h"
#include "search.h"
#include "transpositiontable.h"

void position(std::istringstream& is)
{
    std::string fen, token;
    is >> token;

    if (token == "startpos")
    {
        fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -";
        is >> token;
    }

    else if (token == "fen")
        for (;is >> token && token != "moves"; fen += token + " ");

    Position::set(fen);

    for (Move m; is >> token && (m = uci_to_move(token)); Position::commit_move(m));
}

void moves(std::istringstream& is)
{
    std::string token;

    for (Move m; is >> token && (m = uci_to_move(token)); Position::commit_move(m));
}

void go(std::istringstream& is)
{
    std::string token;
    int         depth;
    uint64_t    thinktime;

    is >> token;

    if (token == "nodes")
    {
        is >> depth;
        Search::count_nodes(depth);
    } 
    else if (token == "movetime")
    {
        is >> thinktime;
        Search::go(thinktime);
    }
    else Search::go();
}

void UCI::loop()
{
    Position::set("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");

    std::string cmd, token;

    do
    {
        std::getline(std::cin, cmd);

        std::istringstream is(cmd);
        is >> token;

        if      (token == "uci")     std::cout << "uciok"               << std::endl;
        else if (token == "isready") std::cout << "readyok"             << std::endl;
        else if (token == "d")       std::cout << Position::to_string() << std::endl;
        else if (token == "fen")     std::cout << Position::fen()       << std::endl;

        else if (token == "ucinewgame")
        {
            Search::clear();
            TranspositionTable::clear();
            RepetitionTable::clear();
        }

        else if (token == "position") position(is);
        else if (token == "go")       go(is);
        else if (token == "moves")    moves(is);
        else if (token == "perft")    Debug::perft(is);
        else if (token == "debug")    Debug::go();
        else if (token == "gameinfo") Debug::gameinfo();
        
    } while (cmd != "quit");
}

Move uci_to_move(const std::string& uci)
{
    Move list[MAX_MOVES];

    for (Move *m = list, *end = get_moves(list); m != end; m++)
        if (move_to_uci(*m) == uci) return *m;

    return NO_MOVE;
}
