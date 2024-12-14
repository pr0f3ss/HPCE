#include "hpce_test_driver.h"
#include "../hpce.h"
#include "../pgn_reader.h"

/**
 * Default constructor. Initializes test driver class.
 */
HPCE_Test_Driver::HPCE_Test_Driver() {
  ChessBoard board = ChessBoard();
  PGN_Reader pgn_reader = PGN_Reader();

  std::vector<PGN_Chess_Game> test_games =
      pgn_reader.return_games("test/games/Abdusattorov.pgn");

  for (PGN_Chess_Game game : test_games) {
    
  }
}

/**
 * Default deconstructor.
 */
HPCE_Test_Driver::~HPCE_Test_Driver() {}