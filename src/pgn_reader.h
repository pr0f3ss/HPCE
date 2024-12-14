#ifndef _PGN_READER_H // include guard
#define _PGN_READER_H

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class PGN_Chess_Game {

public:
  PGN_Chess_Game(std::map<std::string, std::string> tag_pairs);
  ~PGN_Chess_Game(void);

private:
  std::map<std::string, std::string> tag_pairs;
};

class PGN_Reader {

public:
  PGN_Reader(void);
  ~PGN_Reader(void);

  std::vector<PGN_Chess_Game> return_games(std::string file_path);

private:
};

#endif