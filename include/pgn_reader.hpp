#ifndef _PGN_READER_H // include guard
#define _PGN_READER_H

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Move {
  int move_nr;
  int turn;
  std::string move_notation;

  bool operator==(const Move &other) const {
    return move_nr == other.move_nr && turn == other.turn &&
           move_notation == other.move_notation;
  }
};

class PGN_Chess_Game {

public:
  PGN_Chess_Game(std::map<std::string, std::string> tag_pairs);
  ~PGN_Chess_Game(void);

  int add_move(Move move);
  std::map<std::string, std::string> get_tag_pairs(void);
  std::vector<Move> get_move_sequence(void);
  void set_move_sequence(std::vector<Move> &p_move_sequence);

private:
  std::map<std::string, std::string> tag_pairs;
  std::vector<Move> move_sequence;
};

class PGN_Reader {

public:
  PGN_Reader(void);
  ~PGN_Reader(void);

  std::vector<PGN_Chess_Game> return_games(std::string file_path);

private:
  std::vector<std::string> seven_tag_roster = {
      "Event", "Site", "Date", "Round", "White", "Black", "Result"};
  int validate_tag_pair_map(std::map<std::string, std::string> tag_pair_map);
};

#endif