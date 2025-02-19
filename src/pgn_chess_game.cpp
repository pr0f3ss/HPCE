#include "../include/pgn_chess_game.hpp"
#include "../include/hpce.hpp"
#include <algorithm>
#include <map>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

/**
 * Default constructor. Initializes PGN Chess Game class.
 */
PGN_Chess_Game::PGN_Chess_Game(std::map<std::string, std::string> tag_pairs)
    : tag_pairs{tag_pairs} {}

/**
 * Default deconstructor.
 */
PGN_Chess_Game::~PGN_Chess_Game() {}

/**
 * Adds move to chess game. Returns 1 if operation was successful.
 * TODO: Add error logic
 */
int PGN_Chess_Game::add_move(Move move) {
  move_sequence.push_back(move);

  return 1;
}

/**
 * Retrieves the tag pairs.
 */
std::map<std::string, std::string> PGN_Chess_Game::get_tag_pairs(void) {
  return tag_pairs;
}

/**
 * Retrieves the move sequence.
 * TODO: Add error logic
 */
std::vector<Move> PGN_Chess_Game::get_move_sequence(void) {
  return move_sequence;
}

/**
 * Set move sequence to p_move_sequence by value.
 * TODO: Add error logic
 */
void PGN_Chess_Game::set_move_sequence(std::vector<Move> &p_move_sequence) {
  move_sequence.clear();
  move_sequence.insert(move_sequence.end(), p_move_sequence.begin(),
                       p_move_sequence.end());
}