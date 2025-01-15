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
#include <cctype>

// Helps trim the getline strings from pgn files.
void trimString(std::string& str) {
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !std::isspace(ch) && ch != '[' && ch != ']';
    }));
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !std::isspace(ch) && ch != '[' && ch != ']';
    }).base(), str.end());
}

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
 * Retrieves the tag pairs.
 */
std::map<std::string, std::string> PGN_Chess_Game::get_tag_pairs(void){
  return tag_pairs;
}


/**
 * Retrieves the move sequence.
 * TODO: Add error logic
 */
std::vector<Move> PGN_Chess_Game::get_move_sequence(void){
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
  std::regex re("(\\w+)\\s+\"([^\"]+)\"");
  std::smatch match;

  if (if_reader.is_open()) {
    while (!if_reader.eof()) {
      // Initialize current chess game
      std::vector<std::string> str_tag_pairs; // non-tokenized tag pairs
      std::map<std::string, std::string>
          curr_tp; // used to initialize chess game

      // Read current tag pairs and subsequently construct PGN_Chess_Game
      while (std::getline(if_reader, tp)) {
        if (tp.empty() || tp.find_first_not_of(" \t\r\n") == std::string::npos)
          break;

        str_tag_pairs.push_back(tp);
      }

      // Match key and value from each tag pair string and add into tag pair map
      for (std::string s : str_tag_pairs) {
        trimString(s);
        if (std::regex_match(s, match, re)) {
          std::string key = match[1].str();
          std::string value = match[2].str();
          curr_tp.insert({key, value});
        } else {
          std::cerr << "The tag pair format is incorrect." << std::endl;
        }
      }

      // Initialize game from current tag pair map
      PGN_Chess_Game curr_chess_game = PGN_Chess_Game(curr_tp);

      // Continue reading moves and add them to PGN_Chess_Game
      while (std::getline(if_reader, curr_move)) {
        if (curr_move.empty() || curr_move.find_first_not_of(" \t\r\n") == std::string::npos)
          break;

        trimString(curr_move);

        std::regex move_regex(R"((\d+)\.([^\s]+)(?:\s+([^\d]+))?)");
        std::sregex_iterator it(curr_move.begin(), curr_move.end(), move_regex);
        std::sregex_iterator end;

        while (it != end) {
            std::smatch match = *it;
            int move_number = std::stoi(match[1]);   // Move number
            std::string white_move = match[2];       // White move
            std::string black_move = match[3];       // Black move (may be empty)

            Move white = {move_number, 0, white_move};
            Move black = {move_number, 1, black_move};

            curr_chess_game.addMove(white);
            curr_chess_game.addMove(black);

            ++it;
        }
      }

      pgn_chess_games.push_back(curr_chess_game);
    }
  }

  if_reader.close();

  return pgn_chess_games;
}
