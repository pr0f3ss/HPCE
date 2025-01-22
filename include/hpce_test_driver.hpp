#ifndef _HPCE_TEST_DRIVER_H // include guard
#define _HPCE_TEST_DRIVER_H

#include "../include/pgn_reader.hpp"

class HPCE_Test_Driver {

public:
  HPCE_Test_Driver(void);
  ~HPCE_Test_Driver(void);

private:
};

class Game_Factory {
public:
  static PGN_Chess_Game
  create_pgn_chess_game(std::map<std::string, std::string> tag_pairs,
                        std::vector<Move> move_sequence);
};

#endif