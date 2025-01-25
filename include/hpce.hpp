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

struct Figure {
  int value;
  int type;
  int color;
  int empty;
  char symbol;
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

  Figure board[board_size][board_size];
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

private:
  void init_board();
  int is_legal_game(PGN_Chess_Game chess_game);
  int is_legal_move(std::string move, int &rank_from, int &file_from,
                    int &rank_to, int &file_to);

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

  int retrieve_knight_location(std::string move, int &rank_from, int &file_from,
                               int &rank_to, int &file_to);

  int king_into_check(int rank_from, int file_from, int rank_to, int file_to);
  int file_to_int(char file);
  int is_file(char char_notation);
  int figure_moves_into_check(int figure_type, int &rank_from, int &file_from,
                              int &rank_to, int &file_to);

  int check_diagonals(int figure_type, int &rank_from, int &file_from,
                      int &rank_to, int &file_to);

  int handle_bishop_disambiguation(std::string move, int &rank_from,
                                   int &file_from, int &rank_to, int &file_to,
                                   bool is_capture);
};

#endif