#include "../include/hpce_test_driver.hpp"
#include "../include/hpce.hpp"
#include "../include/pgn_reader.hpp"

/**
 * Default constructor. Initializes test driver class.
 */
HPCE_Test_Driver::HPCE_Test_Driver() {
  ChessBoard board = ChessBoard();
  PGN_Reader pgn_reader = PGN_Reader();


  std::vector<PGN_Chess_Game> test_games =
      pgn_reader.return_games("../data/Abdusattorov.pgn");

  for (PGN_Chess_Game game : test_games) {

    std::map<std::string, std::string>::iterator it;

    for (it = game.get_tag_pairs().begin(); it != game.get_tag_pairs().end(); it++)
    {
        std::cout << it->first    // key
                  << ':'
                  << it->second   // val 
                  << std::endl;
    }
  }
}

/**
 * Default deconstructor.
 */
HPCE_Test_Driver::~HPCE_Test_Driver() {}