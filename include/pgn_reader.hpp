#ifndef _PGN_READER_H // include guard
#define _PGN_READER_H

#include "pgn_chess_game.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class PGN_Reader {

public:
  PGN_Reader(void);
  ~PGN_Reader(void);

  std::vector<PGN_Chess_Game> return_games(std::string file_path);

private:
  std::vector<std::string> seven_tag_roster = {
      "Event", "Site", "Date", "Round", "White", "Black", "Result"};
  int validate_tag_pair_map(std::map<std::string, std::string> tag_pair_map);
  static void trim_string(std::string &str);
};

#endif