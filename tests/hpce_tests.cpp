#define CATCH_CONFIG_MAIN

#include "../include/hpce.hpp"
#include "../include/hpce_test_driver.hpp"
#include "../include/pgn_reader.hpp"
#include "catch.hpp"
#include <iostream>
#include <string>

#define ILLEGAL_GAME 0
#define LEGAL_GAME 1

TEST_CASE("Read single-game, correct PGN file", "[pgn][single]") {
  Chess_Board board = Chess_Board();
  PGN_Reader pgn_reader = PGN_Reader();

  std::vector<PGN_Chess_Game> test_games =
      pgn_reader.return_games("../data/pgn_single.pgn");

  std::map<std::string, std::string> tag_pairs = {
      {"Event", "ch-UZB 1st League 2014"},
      {"Site", "Tashkent UZB"},
      {"Date", "2014.03.01"},
      {"Round", "1.5"},
      {"White", "Abdusattorov,Nodirbek"},
      {"Black", "Alikulov,Elbek"},
      {"Result", "1-0"},
      {"WhiteElo", "2024"},
      {"BlackElo", "2212"},
      {"ECO", "B28"}};

  std::vector<Move> move_sequence = {
      // {move_nr, turn, move_notation}
      {1, 0, "e4"},    {1, 1, "c5"},    {2, 0, "Nf3"},   {2, 1, "a6"},
      {3, 0, "d3"},    {3, 1, "g6"},    {4, 0, "g3"},    {4, 1, "Bg7"},
      {5, 0, "Bg2"},   {5, 1, "b5"},    {6, 0, "O-O"},   {6, 1, "Bb7"},
      {7, 0, "c3"},    {7, 1, "e5"},    {8, 0, "a3"},    {8, 1, "Ne7"},
      {9, 0, "b4"},    {9, 1, "d6"},    {10, 0, "Nbd2"}, {10, 1, "O-O"},
      {11, 0, "Nb3"},  {11, 1, "Nd7"},  {12, 0, "Be3"},  {12, 1, "Rc8"},
      {13, 0, "Rc1"},  {13, 1, "h6"},   {14, 0, "Nfd2"}, {14, 1, "f5"},
      {15, 0, "f4"},   {15, 1, "Kh7"},  {16, 0, "Qe2"},  {16, 1, "cxb4"},
      {17, 0, "axb4"}, {17, 1, "exf4"}, {18, 0, "Bxf4"}, {18, 1, "Rxc3"},
      {19, 0, "Rxc3"}, {19, 1, "Bxc3"}, {20, 0, "Bxd6"}, {20, 1, "Qb6+"},
      {21, 0, "Bc5"},  {21, 1, "Nxc5"}, {22, 0, "bxc5"}, {22, 1, "Qe6"},
      {23, 0, "d4"},   {23, 1, "Rd8"},  {24, 0, "Qd3"},  {24, 1, "Bxd2"},
      {25, 0, "Nxd2"}, {25, 1, "fxe4"}, {26, 0, "Nxe4"}, {26, 1, "Nf5"},
      {27, 0, "d5"},   {27, 1, "Qe5"},  {28, 0, "g4"},   {28, 1, "Ne7"},
      {29, 0, "Rf7+"}, {29, 1, "Kg8"},  {30, 0, "Qf1"},  {30, 1, "Nxd5"},
      {31, 0, "Rxb7"}, {31, 1, "Qd4+"}, {32, 0, "Kh1"},  {32, 1, "Rf8"},
      {33, 0, "Qg1"},  {33, 1, "Ne3"},  {34, 0, "Re7"},  {34, 1, "a5"},
      {35, 0, "c6"},   {35, 1, "a4"},   {36, 0, "Qxe3"}, {36, 1, "Qxe3"},
      {37, 0, "Nf6+"}, {37, 1, "Rxf6"}, {38, 0, "Rxe3"}, {38, 1, "Rd6"},
      {39, 0, "h4"},   {39, 1, "Rd1+"}, {40, 0, "Kh2"},  {40, 1, "b4"},
      {41, 0, "c7"},   {41, 1, "1-0"}};

  PGN_Chess_Game valid_chess_game =
      Game_Factory::create_pgn_chess_game(tag_pairs, move_sequence);

  CHECK(test_games.size() == 1);
  CHECK(test_games[0].get_tag_pairs() == valid_chess_game.get_tag_pairs());
  CHECK(test_games[0].get_move_sequence() ==
        valid_chess_game.get_move_sequence());
}

TEST_CASE("Read multi-game, correct PGN file", "[pgn][multi]") {
  Chess_Board board = Chess_Board();
  PGN_Reader pgn_reader = PGN_Reader();

  std::vector<PGN_Chess_Game> test_games =
      pgn_reader.return_games("../data/pgn_multi.pgn");

  CHECK(test_games.size() == 2671);
  int amt_legal_games = 0;
  for (PGN_Chess_Game game : test_games)
    if (board.is_legal_game(game))
      amt_legal_games++;

  CHECK(amt_legal_games == 2671);
}

TEST_CASE("Test Invalid Move: King Moves Into Check", "[unit-test]") {
  Chess_Board board = Chess_Board();
  PGN_Reader pgn_reader = PGN_Reader();

  std::vector<PGN_Chess_Game> test_games =
      pgn_reader.return_games("../data/move_into_check.pgn");

  std::map<std::string, std::string> tag_pairs = {
      {"Event", "Invalid Chess Game"},
      {"Site", "Test Suite"},
      {"Date", "2025.02.22"},
      {"Round", "1"},
      {"White", "Player1"},
      {"Black", "Player2"},
      {"Result", "*"}};

  std::vector<Move> move_sequence = {
      {1, 0, "e4"},    {1, 1, "e5"},
      {2, 0, "Qh5"},   {2, 1, "Nc6"},
      {3, 0, "Bc4"},   {3, 1, "Nf6"},
      {4, 0, "Qxf7+"}, {4, 1, "Ke7"}, // Illegal move: Black moves into check
      {5, 0, "Nc3"},   {5, 1, "d6"},
      {6, 0, "d3"},    {6, 1, "Be6"},
      {7, 0, "Nd5+"},  {7, 1, "Kd7"}, // Black's king stays in check
      {8, 0, "Bg5"},   {8, 1, "Be7"},
      {9, 0, "O-O-O"}, {9, 1, "*"}};

  PGN_Chess_Game invalid_chess_game =
      Game_Factory::create_pgn_chess_game(tag_pairs, move_sequence);

  CHECK(test_games.size() == 1);
  CHECK(test_games[0].get_tag_pairs() == invalid_chess_game.get_tag_pairs());
  CHECK(test_games[0].get_move_sequence() ==
        invalid_chess_game.get_move_sequence());
  CHECK(board.is_legal_game(test_games[0]) == ILLEGAL_GAME);
}

TEST_CASE("Read single-game, malformed tags PGN file", "[pgn][single][tags]") {
  Chess_Board board = Chess_Board();
  PGN_Reader pgn_reader = PGN_Reader();

  std::stringstream error_output; // Buffer to capture `std::cerr`
  std::streambuf *old_cerr =
      std::cerr.rdbuf(error_output.rdbuf()); // Redirect `cerr`

  std::vector<PGN_Chess_Game> test_games =
      pgn_reader.return_games("../data/pgn_single_tags.pgn");

  // Restore original `cerr`
  std::cerr.rdbuf(old_cerr);

  std::string captured_output = error_output.str();
  REQUIRE(captured_output.find(
              "Current Tag pair does not contain the seven tag roster.") !=
          std::string::npos); // Check for expected message
}

TEST_CASE("Test Invalid Move: Knight Moves Diagonally", "[unit-test]") {
  Chess_Board board = Chess_Board();
  PGN_Reader pgn_reader = PGN_Reader();

  std::vector<PGN_Chess_Game> test_games =
      pgn_reader.return_games("../data/knight_moves_diagonally.pgn");

  std::map<std::string, std::string> tag_pairs = {
      {"Event", "Invalid Chess Game"},
      {"Site", "Test Suite"},
      {"Date", "2025.02.22"},
      {"Round", "2"},
      {"White", "Player1"},
      {"Black", "Player2"},
      {"Result", "*"}};

  std::vector<Move> move_sequence = {
      {1, 0, "e4"},  {1, 1, "e5"},  {2, 0, "Nf3"},   {2, 1, "d6"},
      {3, 0, "Nc3"}, {3, 1, "Nc6"}, {4, 0, "Ng5"},   {4, 1, "Be7"},
      {5, 0, "Nd5"}, {5, 1, "Bf6"}, // Illegal move: Knight moves diagonally
      {6, 0, "Bb5"}, {6, 1, "Ne7"}, {7, 0, "Nxe7+"}, {7, 1, "Kxe7"},
      {8, 0, "O-O"}, {8, 1, "a6"},  {9, 0, "Ba4"},   {9, 1, "*"}};

  PGN_Chess_Game invalid_chess_game =
      Game_Factory::create_pgn_chess_game(tag_pairs, move_sequence);

  CHECK(test_games.size() == 1);
  CHECK(test_games[0].get_tag_pairs() == invalid_chess_game.get_tag_pairs());
  CHECK(test_games[0].get_move_sequence() ==
        invalid_chess_game.get_move_sequence());
  CHECK(board.is_legal_game(test_games[0]) == ILLEGAL_GAME);
}
