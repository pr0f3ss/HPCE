#include "../include/pgn_reader.hpp"
#include "../include/hpce.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
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
int PGN_Chess_Game::addMove(Move move) {
  move_sequence.push_back(move);

  return 1;
}

/**
 * Retrieves the move sequence.
 * TODO: Add error logic
 */
std::vector<Move> PGN_Chess_Game::getMoveSequence(void){
  return move_sequence;
}

/**
 * Default constructor. Initializes PGN Reader class.
 */
PGN_Reader::PGN_Reader() {}

/**
 * Default deconstructor.
 */
PGN_Reader::~PGN_Reader() {}

/**
 * Returns all games contained in the specified PGN file as a vector of
 * PGN_Chess_Games.
 */
std::vector<PGN_Chess_Game> PGN_Reader::return_games(std::string file_path) {
  std::vector<PGN_Chess_Game> pgn_chess_games;
  std::ifstream if_reader(file_path);
  std::string tp;
  std::string curr_move;

  // Define regex for tag pair tokenizer
  std::regex re("\\[([^\\s]+) \\\"([^\\\"]+)\\\"]");
  std::smatch match;

  if (if_reader.is_open()) {
    while (!if_reader.eof()) {
      // Initialize current chess game
      std::vector<std::string> tag_pairs; // non-tokenized tag pairs
      std::map<std::string, std::string>
          curr_tp; // used to initialize chess game

      // Read current tag pairs and subsequently construct PGN_Chess_Game
      while (std::getline(if_reader, tp)) {
        if (tp.empty())
          break;
        tag_pairs.push_back(tp);
      }

      for (std::string s : tag_pairs) {
        if (std::regex_match(s, match, re)) {
          std::string key = match[1].str();
          std::string value = match[2].str();
          curr_tp[key] = value;
        } else {
          std::cerr << "The tag pair format is incorrect." << std::endl;
        }
      }

      PGN_Chess_Game curr_chess_game = PGN_Chess_Game(curr_tp);

      // Read current move sequence and add moves to PGN_Chess_Game
      while (std::getline(if_reader, curr_move)) {
        if (curr_move.empty())
          break;
        std::cout << curr_move << "\n";
        // Process move as Move and add to current chess game
      }

      pgn_chess_games.push_back(curr_chess_game);
    }
  }

  if_reader.close();

  return pgn_chess_games;
}
