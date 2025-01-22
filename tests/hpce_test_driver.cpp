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
      pgn_reader.return_games("../data/pass_test_case.pgn");

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

/**
 * Factory method for creating games within the test driver.
 */
PGN_Chess_Game Game_Factory::create_pgn_chess_game(
    std::map<std::string, std::string> tag_pairs,
    std::vector<Move> move_sequence) {
  PGN_Chess_Game chess_game = PGN_Chess_Game(tag_pairs);
  chess_game.set_move_sequence(move_sequence);
  return chess_game;
}