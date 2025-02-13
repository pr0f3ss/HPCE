#ifndef _HPCE_H // include guard
#define _HPCE_H

#include "pgn_reader.hpp"
#include <string>
#include <vector>

#define board_size 8
#define players 2
#define dimension 2
#define rook_amount 2

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
      std::array<std::array<int, INPUT_TOKEN_LENGTH>, board_size>, board_size>>
      board_tokens;
};

class Chess_Board {

  struct Figure w_pawn = {1, 0, 0, 0, 'p'};
  struct Figure b_pawn = {1, 0, 1, 0, 'P'};
  struct Figure w_bishop = {3, 1, 0, 0, 'b'};
  struct Figure b_bishop = {3, 1, 1, 0, 'B'};
  struct Figure w_knight = {3, 2, 0, 0, 'n'};
  struct Figure b_knight = {3, 2, 1, 0, 'N'};
  struct Figure w_rook = {5, 3, 0, 0, 'r'};
  struct Figure b_rook = {5, 3, 1, 0, 'R'};
  struct Figure w_queen = {9, 4, 0, 0, 'q'};
  struct Figure b_queen = {9, 4, 1, 0, 'Q'};
  struct Figure w_king = {0, 5, 0, 0, 'k'};
  struct Figure b_king = {0, 5, 1, 0, 'K'};
  struct Figure empty = {0, -1, 0, 1, ' '};

  std::array<std::array<Figure, board_size>, board_size> board;
  int turn;
  int king_pos[players][dimension];
  int king_moved[players];
  int rook_moved[players][rook_amount]; // 0: queenside rook, 1: kingside rook

public:
  Chess_Board(void);
  ~Chess_Board(void);
  int play_move(std::string move);
  int print_board();
  int get_score();

  Input_Sequence get_input_sequence(PGN_Chess_Game &game);

private:
  std::vector<std::array<std::array<Figure, board_size>, board_size>>
      board_history;

  int en_passant_target[2]; // Stores the rank and file of the en
                            // passant target square

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

  int file_to_int(char file);
  int is_file(char char_notation);
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

  std::array<std::array<std::array<int, NUM_FIGURES * 2>, board_size>,
             board_size>
  get_board_snapshot();
  std::array<int, NUM_FIGURES * 2> get_input_token(int i, int j, int k);
  bool is_special(const std::string &move);
};

#endif