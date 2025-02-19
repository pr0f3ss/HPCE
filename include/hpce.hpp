#ifndef _HPCE_H // include guard
#define _HPCE_H

#include "pgn_reader.hpp"
#include <array>
#include <string>
#include <vector>

#define BOARD_SIZE 8
#define AMT_PLAYERS 2
#define DIMENSION 2
#define AMT_ROOK 2

#define WHITE 0
#define BLACK 1

#define EMPTY_TYPE -1
#define PAWN_TYPE 0
#define BISHOP_TYPE 1
#define KNIGHT_TYPE 2
#define ROOK_TYPE 3
#define QUEEN_TYPE 4
#define KING_TYPE 5

#define POS_LENGTH 8
#define NUM_FIGURES 6
#define INPUT_TOKEN_LENGTH 112

struct Figure {
  int value;
  int type;
  int color;
  int empty;
  char symbol;

  bool operator==(const Figure &other) const {
    return value == other.value && type == other.type && color == other.color &&
           empty == other.empty && symbol == other.symbol;
  }
};

struct Input_Sequence {
  std::vector<std::array<
      std::array<std::array<int, INPUT_TOKEN_LENGTH>, BOARD_SIZE>, BOARD_SIZE>>
      board_tokens;
};

class Chess_Board {
public:
  Chess_Board(void);
  ~Chess_Board(void);

  static constexpr Figure w_pawn = {1, 0, 0, 0, 'p'};
  static constexpr Figure b_pawn = {1, 0, 1, 0, 'P'};
  static constexpr Figure w_bishop = {3, 1, 0, 0, 'b'};
  static constexpr Figure b_bishop = {3, 1, 1, 0, 'B'};
  static constexpr Figure w_knight = {3, 2, 0, 0, 'n'};
  static constexpr Figure b_knight = {3, 2, 1, 0, 'N'};
  static constexpr Figure w_rook = {5, 3, 0, 0, 'r'};
  static constexpr Figure b_rook = {5, 3, 1, 0, 'R'};
  static constexpr Figure w_queen = {9, 4, 0, 0, 'q'};
  static constexpr Figure b_queen = {9, 4, 1, 0, 'Q'};
  static constexpr Figure w_king = {0, 5, 0, 0, 'k'};
  static constexpr Figure b_king = {0, 5, 1, 0, 'K'};
  static constexpr Figure empty = {0, -1, 0, 1, ' '};

  int turn;
  std::array<std::array<Figure, BOARD_SIZE>, BOARD_SIZE> board;
  std::array<std::array<int, DIMENSION>, AMT_PLAYERS> king_pos;
  std::array<int, AMT_PLAYERS> king_moved;
  std::array<std::array<int, AMT_ROOK>, AMT_PLAYERS>
      rook_moved; // 0: Queenside rook, 1: Kingside rook

  int play_move(std::string move);
  int print_board();
  int get_score();

  Input_Sequence get_input_sequence(PGN_Chess_Game &game);

private:
  std::vector<std::array<std::array<Figure, BOARD_SIZE>, BOARD_SIZE>>
      board_history;
  std::array<int, DIMENSION> en_passant_target; // Stores the rank and file of
                                                // the en passant target square

  void init_board();
  int is_legal_game(PGN_Chess_Game chess_game);
  int is_legal_move(std::string move, int &rank_from, int &file_from,
                    int &rank_to, int &file_to);

  int play_move(std::string move, int &rank_from, int &file_from, int &rank_to,
                int &file_to);

  void update_move_flags(char piece, int file_from);
  void update_board(int rank_from, int file_from, int rank_to, int file_to);
  void promote_piece(char figure_char, int rank_from, int file_from,
                     int rank_to, int file_to);
  void handle_castling_update(std::string move);

  int is_legal_figure_move(int figure_type, int &rank_from, int &file_from,
                           int &rank_to, int &file_to);

  int handle_pawn(std::string move, int &rank_from, int &file_from,
                  int &rank_to, int &file_to);
  int handle_knight(std::string move, int &rank_from, int &file_from,
                    int &rank_to, int &file_to);
  int handle_bishop(std::string move, int &rank_from, int &file_from,
                    int &rank_to, int &file_to);
  int handle_rook(std::string move, int &rank_from, int &file_from,
                  int &rank_to, int &file_to);
  int handle_queen(std::string move, int &rank_from, int &file_from,
                   int &rank_to, int &file_to);
  int handle_king(std::string move, int &rank_from, int &file_from,
                  int &rank_to, int &file_to);
  int handle_castling(std::string move, int &rank_from, int &file_from,
                      int &rank_to, int &file_to);

  void update_en_passant_target(int rank, int file);
  void reset_en_passant_target();
  int is_en_passant_target(int rank, int file);

  int retrieve_knight_location(std::string move, int &rank_from, int &file_from,
                               int &rank_to, int &file_to);

  int king_into_check(int rank_from, int file_from, int rank_to, int file_to);
  int is_under_straight_attack(int rank, int file, int delta_rank,
                               int delta_file);
  int is_under_diagonal_attack(int rank, int file, int delta_rank,
                               int delta_file);
  int is_under_pawn_attack(int rank, int file);
  int is_under_knight_attack(int rank, int file);

  int figure_move_is_legal(int figure_type, int &rank_from, int &file_from,
                           int &rank_to, int &file_to);

  int check_straight_line(int figure_type, int &rank_from, int &file_from,
                          int rank_to, int file_to);
  int check_diagonals(int figure_type, int &rank_from, int &file_from,
                      int &rank_to, int &file_to);

  int find_queen_on_file(int file_from, int &rank_from, int rank_to,
                         int file_to);
  int find_queen_on_rank(int rank_from, int &file_from, int rank_to,
                         int file_to);
  int find_queen(int &rank_from, int &file_from, int rank_to, int file_to);
  int check_queen_path(int &rank_from, int &file_from, int rank_to,
                       int file_to);

  int handle_bishop_disambiguation(std::string move, int &rank_from,
                                   int &file_from, int &rank_to, int &file_to,
                                   bool is_capture);

  int handle_knight_disambiguation(std::string move, int &rank_from,
                                   int &file_from, int &rank_to, int &file_to,
                                   bool is_capture);

  std::array<std::array<std::array<int, NUM_FIGURES * 2>, BOARD_SIZE>,
             BOARD_SIZE>
  get_board_snapshot();
  std::array<int, NUM_FIGURES * 2> get_input_token(int i, int j, int k);

  static int file_to_int(char file);
  static int is_file(char char_notation);
  static bool is_special(const std::string &move);
};

#endif