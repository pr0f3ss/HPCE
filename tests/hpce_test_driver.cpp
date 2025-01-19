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

    // for (const auto &p : game.get_tag_pairs())
    // {
    //     std::cout << p.first    // key
    //               << ':'
    //               << p.second   // val 
    //               << std::endl;
    // }

    // std::vector<Move> move_set = game.get_move_sequence();
    // for(const Move& move : move_set){
    //   std::cout << move.move_notation << "\n";
    // }
  }
}

/**
 * Default deconstructor.
 */
HPCE_Test_Driver::~HPCE_Test_Driver() {}