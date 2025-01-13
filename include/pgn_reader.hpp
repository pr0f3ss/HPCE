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
  int is_terminator;
};

class PGN_Chess_Game {

public:
  PGN_Chess_Game(std::map<std::string, std::string> tag_pairs);
  ~PGN_Chess_Game(void);

  int addMove(Move move);
  std::map<std::string, std::string> get_tag_pairs(void);
  std::vector<Move> getMoveSequence(void);

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
};

#endif